; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -verify-machineinstrs -mtriple=powerpc64le-unknown-linux-gnu \
; RUN:   -mcpu=pwr10 -ppc-asm-full-reg-names < %s | FileCheck %s

; The purpose of this test is to check the call protocols for the situation
; where the caller has PC Relative disabled, the callee has PC Relative
; enabled and both functions are in the same file.
; Note that the callee does not know if it clobbers the TOC because it
; contains an external call to @externalFunc.

@global = external local_unnamed_addr global i32, align 4

define dso_local signext i32 @callee(i32 signext %a) local_unnamed_addr #0 {
; CHECK-LABEL: callee:
; CHECK:         .localentry callee, 1
; CHECK-NEXT:  # %bb.0: # %entry
; CHECK-NEXT:    mflr r0
; CHECK-NEXT:    std r30, -16(r1) # 8-byte Folded Spill
; CHECK-NEXT:    std r0, 16(r1)
; CHECK-NEXT:    stdu r1, -48(r1)
; CHECK-NEXT:    mr r30, r3
; CHECK-NEXT:    bl externalFunc@notoc
; CHECK-NEXT:    add r3, r3, r30
; CHECK-NEXT:    extsw r3, r3
; CHECK-NEXT:    addi r1, r1, 48
; CHECK-NEXT:    ld r0, 16(r1)
; CHECK-NEXT:    ld r30, -16(r1) # 8-byte Folded Reload
; CHECK-NEXT:    mtlr r0
; CHECK-NEXT:    blr
entry:
  %call = tail call signext i32 @externalFunc(i32 signext %a) #3
  %add = add nsw i32 %call, %a
  ret i32 %add
}

declare signext i32 @externalFunc(i32 signext) local_unnamed_addr #1

define dso_local void @caller(i32 signext %a) local_unnamed_addr #2 {
; CHECK-LABEL: caller:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    mflr r0
; CHECK-NEXT:    std r30, -16(r1) # 8-byte Folded Spill
; CHECK-NEXT:    std r0, 16(r1)
; CHECK-NEXT:    stdu r1, -48(r1)
; CHECK-NEXT:    addis r4, r2, .LC0@toc@ha
; CHECK-NEXT:    ld r30, .LC0@toc@l(r4)
; CHECK-NEXT:    lwz r4, 0(r30)
; CHECK-NEXT:    add r3, r4, r3
; CHECK-NEXT:    extsw r3, r3
; CHECK-NEXT:    bl callee
; CHECK-NEXT:    nop
; CHECK-NEXT:    mullw r3, r3, r3
; CHECK-NEXT:    stw r3, 0(r30)
; CHECK-NEXT:    addi r1, r1, 48
; CHECK-NEXT:    ld r0, 16(r1)
; CHECK-NEXT:    ld r30, -16(r1) # 8-byte Folded Reload
; CHECK-NEXT:    mtlr r0
; CHECK-NEXT:    blr
entry:
  %0 = load i32, i32* @global, align 4
  %add = add nsw i32 %0, %a
  %call = tail call signext i32 @callee(i32 signext %add)
  %mul = mul nsw i32 %call, %call
  store i32 %mul, i32* @global, align 4
  ret void
}

define dso_local signext i32 @tail_caller(i32 signext %a) local_unnamed_addr #2 {
; CHECK-LABEL: tail_caller:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    mflr r0
; CHECK-NEXT:    std r0, 16(r1)
; CHECK-NEXT:    stdu r1, -32(r1)
; CHECK-NEXT:    addis r4, r2, .LC0@toc@ha
; CHECK-NEXT:    ld r4, .LC0@toc@l(r4)
; CHECK-NEXT:    lwz r4, 0(r4)
; CHECK-NEXT:    add r3, r4, r3
; CHECK-NEXT:    extsw r3, r3
; CHECK-NEXT:    bl callee
; CHECK-NEXT:    nop
; CHECK-NEXT:    addi r1, r1, 32
; CHECK-NEXT:    ld r0, 16(r1)
; CHECK-NEXT:    mtlr r0
; CHECK-NEXT:    blr
entry:
  %0 = load i32, i32* @global, align 4
  %add = add nsw i32 %0, %a
  %call = tail call signext i32 @callee(i32 signext %add)
  ret i32 %call
}


; Left the target features in this test because it is important that caller has
; -pcrelative-memops while callee has +pcrelative-memops
attributes #0 = { nounwind "target-features"="+altivec,+bpermd,+crypto,+direct-move,+extdiv,+pcrelative-memops,+power8-vector,+power9-vector,+vsx,-htm,-spe" }
attributes #1 = { "target-features"="+altivec,+bpermd,+crypto,+direct-move,+extdiv,+pcrelative-memops,+power8-vector,+power9-vector,+vsx,-htm,-spe" }
attributes #2 = { nounwind "target-features"="+altivec,+bpermd,+crypto,+direct-move,+extdiv,+power8-vector,+power9-vector,+vsx,-htm,-pcrelative-memops,-spe" }
attributes #3 = { nounwind }
