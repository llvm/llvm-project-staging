//=-- InstrProf.h - Instrumented profiling format support ---------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Instrumentation-based profiling data is generated by instrumented
// binaries through library functions in compiler-rt, and read by the clang
// frontend to feed PGO.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_PROFILEDATA_INSTRPROF_H_
#define LLVM_PROFILEDATA_INSTRPROF_H_

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/ProfileData/InstrProfData.inc"
#include "llvm/Support/Endian.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MD5.h"
#include <cstdint>
#include <list>
#include <map>
#include <system_error>
#include <vector>

namespace llvm {

class Function;
class GlobalVariable;
class Module;

/// Return the name of data section containing profile counter variables.
inline StringRef getInstrProfCountersSectionName(bool AddSegment) {
  return AddSegment ? "__DATA," INSTR_PROF_CNTS_SECT_NAME_STR
                    : INSTR_PROF_CNTS_SECT_NAME_STR;
}

/// Return the name of data section containing names of instrumented
/// functions.
inline StringRef getInstrProfNameSectionName(bool AddSegment) {
  return AddSegment ? "__DATA," INSTR_PROF_NAME_SECT_NAME_STR
                    : INSTR_PROF_NAME_SECT_NAME_STR;
}

/// Return the name of the data section containing per-function control
/// data.
inline StringRef getInstrProfDataSectionName(bool AddSegment) {
  return AddSegment ? "__DATA," INSTR_PROF_DATA_SECT_NAME_STR
                    : INSTR_PROF_DATA_SECT_NAME_STR;
}

/// Return the name profile runtime entry point to do value profiling
/// for a given site.
inline StringRef getInstrProfValueProfFuncName() {
  return INSTR_PROF_VALUE_PROF_FUNC_STR;
}

/// Return the name of the section containing function coverage mapping
/// data.
inline StringRef getInstrProfCoverageSectionName(bool AddSegment) {
  return AddSegment ? "__DATA," INSTR_PROF_COVMAP_SECT_NAME_STR
                    : INSTR_PROF_COVMAP_SECT_NAME_STR;
}

/// Return the name prefix of variables containing instrumented function names.
inline StringRef getInstrProfNameVarPrefix() { return "__profn_"; }

/// Return the name prefix of variables containing per-function control data.
inline StringRef getInstrProfDataVarPrefix() { return "__profd_"; }

/// Return the name prefix of profile counter variables.
inline StringRef getInstrProfCountersVarPrefix() { return "__profc_"; }

/// Return the name prefix of the COMDAT group for instrumentation variables
/// associated with a COMDAT function.
inline StringRef getInstrProfComdatPrefix() { return "__profv_"; }

/// Return the name of the variable holding the strings (possibly compressed)
/// of all function's PGO names.
inline StringRef getInstrProfNamesVarName() {
  return "__llvm_prf_nm";
}

/// Return the name of a covarage mapping variable (internal linkage)
/// for each instrumented source module. Such variables are allocated
/// in the __llvm_covmap section.
inline StringRef getCoverageMappingVarName() {
  return "__llvm_coverage_mapping";
}

/// Return the name of the internal variable recording the array
/// of PGO name vars referenced by the coverage mapping. The owning
/// functions of those names are not emitted by FE (e.g, unused inline
/// functions.)
inline StringRef getCoverageUnusedNamesVarName() {
  return "__llvm_coverage_names";
}

/// Return the name of function that registers all the per-function control
/// data at program startup time by calling __llvm_register_function. This
/// function has internal linkage and is called by  __llvm_profile_init
/// runtime method. This function is not generated for these platforms:
/// Darwin, Linux, and FreeBSD.
inline StringRef getInstrProfRegFuncsName() {
  return "__llvm_profile_register_functions";
}

/// Return the name of the runtime interface that registers per-function control
/// data for one instrumented function.
inline StringRef getInstrProfRegFuncName() {
  return "__llvm_profile_register_function";
}

/// Return the name of the runtime interface that registers the PGO name strings.
inline StringRef getInstrProfNamesRegFuncName() {
  return "__llvm_profile_register_names_function";
}

/// Return the name of the runtime initialization method that is generated by
/// the compiler. The function calls __llvm_profile_register_functions and
/// __llvm_profile_override_default_filename functions if needed. This function
/// has internal linkage and invoked at startup time via init_array.
inline StringRef getInstrProfInitFuncName() { return "__llvm_profile_init"; }

/// Return the name of the hook variable defined in profile runtime library.
/// A reference to the variable causes the linker to link in the runtime
/// initialization module (which defines the hook variable).
inline StringRef getInstrProfRuntimeHookVarName() {
  return "__llvm_profile_runtime";
}

/// Return the name of the compiler generated function that references the
/// runtime hook variable. The function is a weak global.
inline StringRef getInstrProfRuntimeHookVarUseFuncName() {
  return "__llvm_profile_runtime_user";
}

/// Return the name of the profile runtime interface that overrides the default
/// profile data file name.
inline StringRef getInstrProfFileOverriderFuncName() {
  return "__llvm_profile_override_default_filename";
}

/// Return the marker used to separate PGO names during serialization.
inline StringRef getInstrProfNameSeparator() { return "\01"; }

/// Return the modified name for function \c F suitable to be
/// used the key for profile lookup. Variable \c InLTO indicates if this
/// is called in LTO optimization passes.
std::string getPGOFuncName(const Function &F, bool InLTO = false,
                           uint64_t Version = INSTR_PROF_INDEX_VERSION);

/// Return the modified name for a function suitable to be
/// used the key for profile lookup. The function's original
/// name is \c RawFuncName and has linkage of type \c Linkage.
/// The function is defined in module \c FileName.
std::string getPGOFuncName(StringRef RawFuncName,
                           GlobalValue::LinkageTypes Linkage,
                           StringRef FileName,
                           uint64_t Version = INSTR_PROF_INDEX_VERSION);

/// Return the name of the global variable used to store a function
/// name in PGO instrumentation. \c FuncName is the name of the function
/// returned by the \c getPGOFuncName call.
std::string getPGOFuncNameVarName(StringRef FuncName,
                                  GlobalValue::LinkageTypes Linkage);

/// Create and return the global variable for function name used in PGO
/// instrumentation. \c FuncName is the name of the function returned
/// by \c getPGOFuncName call.
GlobalVariable *createPGOFuncNameVar(Function &F, StringRef PGOFuncName);

/// Create and return the global variable for function name used in PGO
/// instrumentation.  /// \c FuncName is the name of the function
/// returned by \c getPGOFuncName call, \c M is the owning module,
/// and \c Linkage is the linkage of the instrumented function.
GlobalVariable *createPGOFuncNameVar(Module &M,
                                     GlobalValue::LinkageTypes Linkage,
                                     StringRef PGOFuncName);
/// Return the initializer in string of the PGO name var \c NameVar.
StringRef getPGOFuncNameVarInitializer(GlobalVariable *NameVar);

/// Given a PGO function name, remove the filename prefix and return
/// the original (static) function name.
StringRef getFuncNameWithoutPrefix(StringRef PGOFuncName,
                                   StringRef FileName = "<unknown>");

/// Given a vector of strings (function PGO names) \c NameStrs, the
/// method generates a combined string \c Result thatis ready to be
/// serialized.  The \c Result string is comprised of three fields:
/// The first field is the legnth of the uncompressed strings, and the
/// the second field is the length of the zlib-compressed string.
/// Both fields are encoded in ULEB128.  If \c doCompress is false, the
///  third field is the uncompressed strings; otherwise it is the
/// compressed string. When the string compression is off, the
/// second field will have value zero.
int collectPGOFuncNameStrings(const std::vector<std::string> &NameStrs,
                              bool doCompression, std::string &Result);
/// Produce \c Result string with the same format described above. The input
/// is vector of PGO function name variables that are referenced.
int collectPGOFuncNameStrings(const std::vector<GlobalVariable *> &NameVars,
                              std::string &Result, bool doCompression = true);
class InstrProfSymtab;
/// \c NameStrings is a string composed of one of more sub-strings encoded in
/// the format described above. The substrings are seperated by 0 or more zero
/// bytes. This method decodes the string and populates the \c Symtab.
int readPGOFuncNameStrings(StringRef NameStrings, InstrProfSymtab &Symtab);

enum InstrProfValueKind : uint32_t {
#define VALUE_PROF_KIND(Enumerator, Value) Enumerator = Value,
#include "llvm/ProfileData/InstrProfData.inc"
};

struct InstrProfRecord;

/// Get the value profile data for value site \p SiteIdx from \p InstrProfR
/// and annotate the instruction \p Inst with the value profile meta data.
/// Annotate up to \p MaxMDCount (default 3) number of records per value site.
void annotateValueSite(Module &M, Instruction &Inst,
                       const InstrProfRecord &InstrProfR,
                       InstrProfValueKind ValueKind, uint32_t SiteIndx,
                       uint32_t MaxMDCount = 3);
/// Same as the above interface but using an ArrayRef, as well as \p Sum.
void annotateValueSite(Module &M, Instruction &Inst,
                       ArrayRef<InstrProfValueData> VDs,
                       uint64_t Sum, InstrProfValueKind ValueKind,
                       uint32_t MaxMDCount);

/// Extract the value profile data from \p Inst which is annotated with
/// value profile meta data. Return false if there is no value data annotated,
/// otherwise  return true.
bool getValueProfDataFromInst(const Instruction &Inst,
                              InstrProfValueKind ValueKind,
                              uint32_t MaxNumValueData,
                              InstrProfValueData ValueData[],
                              uint32_t &ActualNumValueData, uint64_t &TotalC);

const std::error_category &instrprof_category();

enum class instrprof_error {
  success = 0,
  eof,
  unrecognized_format,
  bad_magic,
  bad_header,
  unsupported_version,
  unsupported_hash_type,
  too_large,
  truncated,
  malformed,
  unknown_function,
  hash_mismatch,
  count_mismatch,
  counter_overflow,
  value_site_count_mismatch
};

inline std::error_code make_error_code(instrprof_error E) {
  return std::error_code(static_cast<int>(E), instrprof_category());
}

inline instrprof_error MergeResult(instrprof_error &Accumulator,
                                   instrprof_error Result) {
  // Prefer first error encountered as later errors may be secondary effects of
  // the initial problem.
  if (Accumulator == instrprof_error::success &&
      Result != instrprof_error::success)
    Accumulator = Result;
  return Accumulator;
}

namespace object {
class SectionRef;
}

namespace IndexedInstrProf {
uint64_t ComputeHash(StringRef K);
}

/// A symbol table used for function PGO name look-up with keys
/// (such as pointers, md5hash values) to the function. A function's
/// PGO name or name's md5hash are used in retrieving the profile
/// data of the function. See \c getPGOFuncName() method for details
/// on how PGO name is formed.
class InstrProfSymtab {
public:
  typedef std::vector<std::pair<uint64_t, uint64_t>> AddrHashMap;

private:
  StringRef Data;
  uint64_t Address;
  // Unique name strings.
  StringSet<> NameTab;
  // A map from MD5 keys to function name strings.
  std::vector<std::pair<uint64_t, StringRef>> MD5NameMap;
  // A map from MD5 keys to function define. We only populate this map
  // when build the Symtab from a Module.
  std::vector<std::pair<uint64_t, Function *>> MD5FuncMap;
  // A map from function runtime address to function name MD5 hash.
  // This map is only populated and used by raw instr profile reader.
  AddrHashMap AddrToMD5Map;

public:
  InstrProfSymtab()
      : Data(), Address(0), NameTab(), MD5NameMap(), MD5FuncMap(),
      AddrToMD5Map() {}

  /// Create InstrProfSymtab from an object file section which
  /// contains function PGO names. When section may contain raw
  /// string data or string data in compressed form. This method
  /// only initialize the symtab with reference to the data and
  /// the section base address. The decompression will be delayed
  /// until before it is used. See also \c create(StringRef) method.
  std::error_code create(object::SectionRef &Section);
  /// This interface is used by reader of CoverageMapping test
  /// format.
  inline std::error_code create(StringRef D, uint64_t BaseAddr);
  /// \c NameStrings is a string composed of one of more sub-strings
  ///  encoded in the format described in \c collectPGOFuncNameStrings.
  /// This method is a wrapper to \c readPGOFuncNameStrings method.
  inline std::error_code create(StringRef NameStrings);
  /// A wrapper interface to populate the PGO symtab with functions
  /// decls from module \c M. This interface is used by transformation
  /// passes such as indirect function call promotion. Variable \c InLTO
  /// indicates if this is called from LTO optimization passes.
  void create(Module &M, bool InLTO = false);
  /// Create InstrProfSymtab from a set of names iteratable from
  /// \p IterRange. This interface is used by IndexedProfReader.
  template <typename NameIterRange> void create(const NameIterRange &IterRange);
  // If the symtab is created by a series of calls to \c addFuncName, \c
  // finalizeSymtab needs to be called before looking up function names.
  // This is required because the underlying map is a vector (for space
  // efficiency) which needs to be sorted.
  inline void finalizeSymtab();
  /// Update the symtab by adding \p FuncName to the table. This interface
  /// is used by the raw and text profile readers.
  void addFuncName(StringRef FuncName) {
    auto ins = NameTab.insert(FuncName);
    if (ins.second)
      MD5NameMap.push_back(std::make_pair(
          IndexedInstrProf::ComputeHash(FuncName), ins.first->getKey()));
  }
  /// Map a function address to its name's MD5 hash. This interface
  /// is only used by the raw profiler reader.
  void mapAddress(uint64_t Addr, uint64_t MD5Val) {
    AddrToMD5Map.push_back(std::make_pair(Addr, MD5Val));
  }
  AddrHashMap &getAddrHashMap() { return AddrToMD5Map; }
  /// Return function's PGO name from the function name's symbol
  /// address in the object file. If an error occurs, return
  /// an empty string.
  StringRef getFuncName(uint64_t FuncNameAddress, size_t NameSize);
  /// Return function's PGO name from the name's md5 hash value.
  /// If not found, return an empty string.
  inline StringRef getFuncName(uint64_t FuncMD5Hash);
  /// Return function from the name's md5 hash. Return nullptr if not found.
  inline Function *getFunction(uint64_t FuncMD5Hash);
  /// Return the function's original assembly name by stripping off
  /// the prefix attached (to symbols with priviate linkage). For
  /// global functions, it returns the same string as getFuncName.
  inline StringRef getOrigFuncName(uint64_t FuncMD5Hash);
  /// Return the name section data.
  inline StringRef getNameData() const { return Data; }
};

std::error_code InstrProfSymtab::create(StringRef D, uint64_t BaseAddr) {
  Data = D;
  Address = BaseAddr;
  return std::error_code();
}

std::error_code InstrProfSymtab::create(StringRef NameStrings) {
  if (readPGOFuncNameStrings(NameStrings, *this))
    return make_error_code(instrprof_error::malformed);
  return std::error_code();
}

template <typename NameIterRange>
void InstrProfSymtab::create(const NameIterRange &IterRange) {
  for (auto Name : IterRange)
    addFuncName(Name);

  finalizeSymtab();
}

void InstrProfSymtab::finalizeSymtab() {
  std::sort(MD5NameMap.begin(), MD5NameMap.end(), less_first());
  std::sort(MD5FuncMap.begin(), MD5FuncMap.end(), less_first());
  std::sort(AddrToMD5Map.begin(), AddrToMD5Map.end(), less_first());
  AddrToMD5Map.erase(std::unique(AddrToMD5Map.begin(), AddrToMD5Map.end()),
                     AddrToMD5Map.end());
}

StringRef InstrProfSymtab::getFuncName(uint64_t FuncMD5Hash) {
  auto Result =
      std::lower_bound(MD5NameMap.begin(), MD5NameMap.end(), FuncMD5Hash,
                       [](const std::pair<uint64_t, std::string> &LHS,
                          uint64_t RHS) { return LHS.first < RHS; });
  if (Result != MD5NameMap.end() && Result->first == FuncMD5Hash)
    return Result->second;
  return StringRef();
}

Function* InstrProfSymtab::getFunction(uint64_t FuncMD5Hash) {
  auto Result =
      std::lower_bound(MD5FuncMap.begin(), MD5FuncMap.end(), FuncMD5Hash,
                       [](const std::pair<uint64_t, Function*> &LHS,
                          uint64_t RHS) { return LHS.first < RHS; });
  if (Result != MD5FuncMap.end() && Result->first == FuncMD5Hash)
    return Result->second;
  return nullptr;
}

// See also getPGOFuncName implementation. These two need to be
// matched.
StringRef InstrProfSymtab::getOrigFuncName(uint64_t FuncMD5Hash) {
  StringRef PGOName = getFuncName(FuncMD5Hash);
  size_t S = PGOName.find_first_of(':');
  if (S == StringRef::npos)
    return PGOName;
  return PGOName.drop_front(S + 1);
}

struct InstrProfValueSiteRecord {
  /// Value profiling data pairs at a given value site.
  std::list<InstrProfValueData> ValueData;

  InstrProfValueSiteRecord() { ValueData.clear(); }
  template <class InputIterator>
  InstrProfValueSiteRecord(InputIterator F, InputIterator L)
      : ValueData(F, L) {}

  /// Sort ValueData ascending by Value
  void sortByTargetValues() {
    ValueData.sort(
        [](const InstrProfValueData &left, const InstrProfValueData &right) {
          return left.Value < right.Value;
        });
  }
  /// Sort ValueData Descending by Count
  inline void sortByCount();

  /// Merge data from another InstrProfValueSiteRecord
  /// Optionally scale merged counts by \p Weight.
  instrprof_error merge(InstrProfValueSiteRecord &Input, uint64_t Weight = 1);
  /// Scale up value profile data counts.
  instrprof_error scale(uint64_t Weight);
};

/// Profiling information for a single function.
struct InstrProfRecord {
  InstrProfRecord() {}
  InstrProfRecord(StringRef Name, uint64_t Hash, std::vector<uint64_t> Counts)
      : Name(Name), Hash(Hash), Counts(std::move(Counts)) {}
  StringRef Name;
  uint64_t Hash;
  std::vector<uint64_t> Counts;

  typedef std::vector<std::pair<uint64_t, uint64_t>> ValueMapType;

  /// Return the number of value profile kinds with non-zero number
  /// of profile sites.
  inline uint32_t getNumValueKinds() const;
  /// Return the number of instrumented sites for ValueKind.
  inline uint32_t getNumValueSites(uint32_t ValueKind) const;
  /// Return the total number of ValueData for ValueKind.
  inline uint32_t getNumValueData(uint32_t ValueKind) const;
  /// Return the number of value data collected for ValueKind at profiling
  /// site: Site.
  inline uint32_t getNumValueDataForSite(uint32_t ValueKind,
                                         uint32_t Site) const;
  /// Return the array of profiled values at \p Site.
  inline std::unique_ptr<InstrProfValueData[]>
  getValueForSite(uint32_t ValueKind, uint32_t Site,
                  uint64_t (*ValueMapper)(uint32_t, uint64_t) = 0) const;
  inline void
  getValueForSite(InstrProfValueData Dest[], uint32_t ValueKind, uint32_t Site,
                  uint64_t (*ValueMapper)(uint32_t, uint64_t) = 0) const;
  /// Reserve space for NumValueSites sites.
  inline void reserveSites(uint32_t ValueKind, uint32_t NumValueSites);
  /// Add ValueData for ValueKind at value Site.
  void addValueData(uint32_t ValueKind, uint32_t Site,
                    InstrProfValueData *VData, uint32_t N,
                    ValueMapType *ValueMap);

  /// Merge the counts in \p Other into this one.
  /// Optionally scale merged counts by \p Weight.
  instrprof_error merge(InstrProfRecord &Other, uint64_t Weight = 1);

  /// Scale up profile counts (including value profile data) by
  /// \p Weight.
  instrprof_error scale(uint64_t Weight);

  /// Sort value profile data (per site) by count.
  void sortValueData() {
    for (uint32_t Kind = IPVK_First; Kind <= IPVK_Last; ++Kind) {
      std::vector<InstrProfValueSiteRecord> &SiteRecords =
          getValueSitesForKind(Kind);
      for (auto &SR : SiteRecords)
        SR.sortByCount();
    }
  }
  /// Clear value data entries
  void clearValueData() {
    for (uint32_t Kind = IPVK_First; Kind <= IPVK_Last; ++Kind)
      getValueSitesForKind(Kind).clear();
  }

private:
  std::vector<InstrProfValueSiteRecord> IndirectCallSites;
  const std::vector<InstrProfValueSiteRecord> &
  getValueSitesForKind(uint32_t ValueKind) const {
    switch (ValueKind) {
    case IPVK_IndirectCallTarget:
      return IndirectCallSites;
    default:
      llvm_unreachable("Unknown value kind!");
    }
    return IndirectCallSites;
  }

  std::vector<InstrProfValueSiteRecord> &
  getValueSitesForKind(uint32_t ValueKind) {
    return const_cast<std::vector<InstrProfValueSiteRecord> &>(
        const_cast<const InstrProfRecord *>(this)
            ->getValueSitesForKind(ValueKind));
  }

  // Map indirect call target name hash to name string.
  uint64_t remapValue(uint64_t Value, uint32_t ValueKind,
                      ValueMapType *HashKeys);

  // Merge Value Profile data from Src record to this record for ValueKind.
  // Scale merged value counts by \p Weight.
  instrprof_error mergeValueProfData(uint32_t ValueKind, InstrProfRecord &Src,
                                     uint64_t Weight);
  // Scale up value profile data count.
  instrprof_error scaleValueProfData(uint32_t ValueKind, uint64_t Weight);
};

uint32_t InstrProfRecord::getNumValueKinds() const {
  uint32_t NumValueKinds = 0;
  for (uint32_t Kind = IPVK_First; Kind <= IPVK_Last; ++Kind)
    NumValueKinds += !(getValueSitesForKind(Kind).empty());
  return NumValueKinds;
}

uint32_t InstrProfRecord::getNumValueData(uint32_t ValueKind) const {
  uint32_t N = 0;
  const std::vector<InstrProfValueSiteRecord> &SiteRecords =
      getValueSitesForKind(ValueKind);
  for (auto &SR : SiteRecords) {
    N += SR.ValueData.size();
  }
  return N;
}

uint32_t InstrProfRecord::getNumValueSites(uint32_t ValueKind) const {
  return getValueSitesForKind(ValueKind).size();
}

uint32_t InstrProfRecord::getNumValueDataForSite(uint32_t ValueKind,
                                                 uint32_t Site) const {
  return getValueSitesForKind(ValueKind)[Site].ValueData.size();
}

std::unique_ptr<InstrProfValueData[]> InstrProfRecord::getValueForSite(
    uint32_t ValueKind, uint32_t Site,
    uint64_t (*ValueMapper)(uint32_t, uint64_t)) const {
  uint32_t N = getNumValueDataForSite(ValueKind, Site);
  if (N == 0)
    return std::unique_ptr<InstrProfValueData[]>(nullptr);

  auto VD = llvm::make_unique<InstrProfValueData[]>(N);
  getValueForSite(VD.get(), ValueKind, Site, ValueMapper);

  return VD;
}

void InstrProfRecord::getValueForSite(InstrProfValueData Dest[],
                                      uint32_t ValueKind, uint32_t Site,
                                      uint64_t (*ValueMapper)(uint32_t,
                                                              uint64_t)) const {
  uint32_t I = 0;
  for (auto V : getValueSitesForKind(ValueKind)[Site].ValueData) {
    Dest[I].Value = ValueMapper ? ValueMapper(ValueKind, V.Value) : V.Value;
    Dest[I].Count = V.Count;
    I++;
  }
}

void InstrProfRecord::reserveSites(uint32_t ValueKind, uint32_t NumValueSites) {
  std::vector<InstrProfValueSiteRecord> &ValueSites =
      getValueSitesForKind(ValueKind);
  ValueSites.reserve(NumValueSites);
}

inline support::endianness getHostEndianness() {
  return sys::IsLittleEndianHost ? support::little : support::big;
}

// Include definitions for value profile data
#define INSTR_PROF_VALUE_PROF_DATA
#include "llvm/ProfileData/InstrProfData.inc"

void InstrProfValueSiteRecord::sortByCount() {
  ValueData.sort(
      [](const InstrProfValueData &left, const InstrProfValueData &right) {
        return left.Count > right.Count;
      });
  // Now truncate
  size_t max_s = INSTR_PROF_MAX_NUM_VAL_PER_SITE;
  if (ValueData.size() > max_s)
    ValueData.resize(max_s);
}

/*
* Initialize the record for runtime value profile data.
* Return 0 if the initialization is successful, otherwise
* return 1.
*/
int initializeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord,
                                     const uint16_t *NumValueSites,
                                     ValueProfNode **Nodes);

/* Release memory allocated for the runtime record.  */
void finalizeValueProfRuntimeRecord(ValueProfRuntimeRecord *RuntimeRecord);

/* Return the size of ValueProfData structure that can be used to store
   the value profile data collected at runtime. */
uint32_t getValueProfDataSizeRT(const ValueProfRuntimeRecord *Record);

/* Return a ValueProfData instance that stores the data collected at runtime. */
ValueProfData *
serializeValueProfDataFromRT(const ValueProfRuntimeRecord *Record,
                             ValueProfData *Dst);

namespace IndexedInstrProf {
struct Summary;
}

///// Profile summary computation ////
// The 'show' command displays richer summary of the profile data. The profile
// summary is one or more (Cutoff, MinBlockCount, NumBlocks) triplets. Given a
// target execution count percentile, we compute the minimum number of blocks
// needed to reach this target and the minimum execution count of these blocks.
struct ProfileSummaryEntry {
  uint32_t Cutoff;        ///< The required percentile of total execution count.
  uint64_t MinBlockCount; ///< The minimum execution count for this percentile.
  uint64_t NumBlocks;     ///< Number of blocks >= the minumum execution count.
  ProfileSummaryEntry(uint32_t TheCutoff, uint64_t TheMinBlockCount,
                      uint64_t TheNumBlocks)
      : Cutoff(TheCutoff), MinBlockCount(TheMinBlockCount),
        NumBlocks(TheNumBlocks) {}
};

class ProfileSummary {
  // We keep track of the number of times a count appears in the profile and
  // keep the map sorted in the descending order of counts.
  std::map<uint64_t, uint32_t, std::greater<uint64_t>> CountFrequencies;
  std::vector<ProfileSummaryEntry> DetailedSummary;
  std::vector<uint32_t> DetailedSummaryCutoffs;
  // Sum of all counts.
  uint64_t TotalCount;
  uint64_t MaxBlockCount, MaxInternalBlockCount, MaxFunctionCount;
  uint32_t NumBlocks, NumFunctions;
  inline void addCount(uint64_t Count, bool IsEntry);

public:
  static const int Scale = 1000000;
  ProfileSummary(std::vector<uint32_t> Cutoffs)
      : DetailedSummaryCutoffs(Cutoffs), TotalCount(0), MaxBlockCount(0),
        MaxInternalBlockCount(0), MaxFunctionCount(0), NumBlocks(0),
        NumFunctions(0) {}
  ProfileSummary(const IndexedInstrProf::Summary &S);
  inline void addRecord(const InstrProfRecord &);
  inline std::vector<ProfileSummaryEntry> &getDetailedSummary();
  void computeDetailedSummary();
  uint32_t getNumBlocks() { return NumBlocks; }
  uint64_t getTotalCount() { return TotalCount; }
  uint32_t getNumFunctions() { return NumFunctions; }
  uint64_t getMaxFunctionCount() { return MaxFunctionCount; }
  uint64_t getMaxBlockCount() { return MaxBlockCount; }
  uint64_t getMaxInternalBlockCount() { return MaxInternalBlockCount; }
};

// This is called when a count is seen in the profile.
void ProfileSummary::addCount(uint64_t Count, bool IsEntry) {
  TotalCount += Count;
  if (Count > MaxBlockCount)
    MaxBlockCount = Count;
  if (!IsEntry && Count > MaxInternalBlockCount)
    MaxInternalBlockCount = Count;
  NumBlocks++;
  CountFrequencies[Count]++;
}

void ProfileSummary::addRecord(const InstrProfRecord &R) {
  NumFunctions++;
  if (R.Counts[0] > MaxFunctionCount)
    MaxFunctionCount = R.Counts[0];

  for (size_t I = 0, E = R.Counts.size(); I < E; ++I)
    addCount(R.Counts[I], (I == 0));
}

std::vector<ProfileSummaryEntry> &ProfileSummary::getDetailedSummary() {
  if (!DetailedSummaryCutoffs.empty() && DetailedSummary.empty())
    computeDetailedSummary();
  return DetailedSummary;
}

namespace IndexedInstrProf {

enum class HashT : uint32_t {
  MD5,

  Last = MD5
};

inline uint64_t ComputeHash(HashT Type, StringRef K) {
  switch (Type) {
  case HashT::MD5:
    return MD5Hash(K);
  }
  llvm_unreachable("Unhandled hash type");
}

const uint64_t Magic = 0x8169666f72706cff; // "\xfflprofi\x81"

enum ProfVersion {
  // Version 1 is the first version. In this version, the value of
  // a key/value pair can only include profile data of a single function.
  // Due to this restriction, the number of block counters for a given
  // function is not recorded but derived from the length of the value.
  Version1 = 1,
  // The version 2 format supports recording profile data of multiple
  // functions which share the same key in one value field. To support this,
  // the number block counters is recorded as an uint64_t field right after the
  // function structural hash.
  Version2 = 2,
  // Version 3 supports value profile data. The value profile data is expected
  // to follow the block counter profile data.
  Version3 = 3,
  // In this version, profile summary data \c IndexedInstrProf::Summary is
  // stored after the profile header.
  Version4 = 4,
  // The current version is 4.
  CurrentVersion = INSTR_PROF_INDEX_VERSION
};
const uint64_t Version = ProfVersion::CurrentVersion;

const HashT HashType = HashT::MD5;

inline uint64_t ComputeHash(StringRef K) { return ComputeHash(HashType, K); }

// This structure defines the file header of the LLVM profile
// data file in indexed-format.
struct Header {
  uint64_t Magic;
  uint64_t Version;
  uint64_t Unused; // Becomes unused since version 4
  uint64_t HashType;
  uint64_t HashOffset;
};

static const uint32_t SummaryCutoffs[] = {
    10000,  /*  1% */
    100000, /* 10% */
    200000, 300000, 400000, 500000, 600000, 500000, 600000, 700000,
    800000, 900000, 950000, 990000, 999000, 999900, 999990, 999999};
static const uint32_t NumSummaryCutoffs =
    sizeof(SummaryCutoffs) / sizeof(*SummaryCutoffs);

// Profile summary data recorded in the profile data file in indexed
// format. It is introduced in version 4. The summary data follows
// right after the profile file header.
struct Summary {

  struct Entry {
    uint64_t Cutoff; ///< The required percentile of total execution count.
    uint64_t
        MinBlockCount;  ///< The minimum execution count for this percentile.
    uint64_t NumBlocks; ///< Number of blocks >= the minumum execution count.
  };
  // New field kind to existing enum value mapping should remain unchanged
  // when new kind is added in the future.
  enum SummaryFieldKind {
    /// The total number of functions instrumented.
    TotalNumFunctions = 0,
    /// Total number of instrumented blocks/edges.
    TotalNumBlocks = 1,
    /// The maximal execution count among all functions.
    /// This field does not exist for profile data from IR based
    /// instrumentation.
    MaxFunctionCount = 2,
    /// Max block count of the program.
    MaxBlockCount = 3,
    /// Max internal block count of the program (excluding entry blocks).
    MaxInternalBlockCount = 4,
    /// The sum of all instrumented block counts.
    TotalBlockCount = 5,
    NumKinds = TotalBlockCount + 1
  };

  // The number of summmary fields following the summary header.
  uint64_t NumSummaryFields;
  // The number of Cutoff Entries (Summary::Entry) following summary fields.
  uint64_t NumCutoffEntries;

  static uint32_t getSize(uint32_t NumSumFields, uint32_t NumCutoffEntries) {
    return sizeof(Summary) + NumCutoffEntries * sizeof(Entry) +
           NumSumFields * sizeof(uint64_t);
  }

  const uint64_t *getSummaryDataBase() const {
    return reinterpret_cast<const uint64_t *>(this + 1);
  }
  uint64_t *getSummaryDataBase() {
    return reinterpret_cast<uint64_t *>(this + 1);
  }
  const Entry *getCutoffEntryBase() const {
    return reinterpret_cast<const Entry *>(
        &getSummaryDataBase()[NumSummaryFields]);
  }
  Entry *getCutoffEntryBase() {
    return reinterpret_cast<Entry *>(&getSummaryDataBase()[NumSummaryFields]);
  }

  uint64_t get(SummaryFieldKind K) const {
    return getSummaryDataBase()[K];
  }

  void set(SummaryFieldKind K, uint64_t V) {
    getSummaryDataBase()[K] = V;
  }

  const Entry &getEntry(uint32_t I) const { return getCutoffEntryBase()[I]; }
  void setEntry(uint32_t I, const ProfileSummaryEntry &E) {
    Entry &ER = getCutoffEntryBase()[I];
    ER.Cutoff = E.Cutoff;
    ER.MinBlockCount = E.MinBlockCount;
    ER.NumBlocks = E.NumBlocks;
  }

  Summary(uint32_t Size) { memset(this, 0, Size); }
  void operator delete(void *ptr) { ::operator delete(ptr); }

  Summary() = delete;
};

inline std::unique_ptr<Summary> allocSummary(uint32_t TotalSize) {
  return std::unique_ptr<Summary>(new (::operator new(TotalSize))
                                      Summary(TotalSize));
}
} // end namespace IndexedInstrProf

namespace RawInstrProf {

// Version 1: First version
// Version 2: Added value profile data section. Per-function control data
// struct has more fields to describe value profile information.
// Version 3: Compressed name section support. Function PGO name reference
// from control data struct is changed from raw pointer to Name's MD5 value.
const uint64_t Version = INSTR_PROF_RAW_VERSION;

template <class IntPtrT> inline uint64_t getMagic();
template <> inline uint64_t getMagic<uint64_t>() {
  return INSTR_PROF_RAW_MAGIC_64;
}

template <> inline uint64_t getMagic<uint32_t>() {
  return INSTR_PROF_RAW_MAGIC_32;
}

// Per-function profile data header/control structure.
// The definition should match the structure defined in
// compiler-rt/lib/profile/InstrProfiling.h.
// It should also match the synthesized type in
// Transforms/Instrumentation/InstrProfiling.cpp:getOrCreateRegionCounters.
template <class IntPtrT> struct LLVM_ALIGNAS(8) ProfileData {
  #define INSTR_PROF_DATA(Type, LLVMType, Name, Init) Type Name;
  #include "llvm/ProfileData/InstrProfData.inc"
};

// File header structure of the LLVM profile data in raw format.
// The definition should match the header referenced in
// compiler-rt/lib/profile/InstrProfilingFile.c  and
// InstrProfilingBuffer.c.
struct Header {
#define INSTR_PROF_RAW_HEADER(Type, Name, Init) const Type Name;
#include "llvm/ProfileData/InstrProfData.inc"
};

}  // end namespace RawInstrProf

} // end namespace llvm

namespace std {
template <>
struct is_error_code_enum<llvm::instrprof_error> : std::true_type {};
}

#endif // LLVM_PROFILEDATA_INSTRPROF_H_
