; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -instcombine -S | FileCheck %s

target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:128:128-n8:16:32"

;pairwise reverse
;template <typename T>
;T reverse(T v) {
;  T s = sizeof(v) * 8;
;  T mask = ~(T)0;
;  while ((s >>= 1) > 0) {
;    mask ^= (mask << s);
;    v = ((v >> s) & mask) | ((v << s) & ~mask);
;  }
;  return v;
;}
define i8 @rev8(i8 %v) {
; CHECK-LABEL: @rev8(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[OR:%.*]] = call i8 @llvm.fshl.i8(i8 [[V:%.*]], i8 [[V]], i8 4)
; CHECK-NEXT:    [[SHR4_1:%.*]] = lshr i8 [[OR]], 2
; CHECK-NEXT:    [[AND_1:%.*]] = and i8 [[SHR4_1]], 51
; CHECK-NEXT:    [[SHL7_1:%.*]] = shl i8 [[OR]], 2
; CHECK-NEXT:    [[AND9_1:%.*]] = and i8 [[SHL7_1]], -52
; CHECK-NEXT:    [[OR_1:%.*]] = or i8 [[AND_1]], [[AND9_1]]
; CHECK-NEXT:    [[SHR4_2:%.*]] = lshr i8 [[OR_1]], 1
; CHECK-NEXT:    [[AND_2:%.*]] = and i8 [[SHR4_2]], 85
; CHECK-NEXT:    [[SHL7_2:%.*]] = shl i8 [[OR_1]], 1
; CHECK-NEXT:    [[AND9_2:%.*]] = and i8 [[SHL7_2]], -86
; CHECK-NEXT:    [[OR_2:%.*]] = or i8 [[AND_2]], [[AND9_2]]
; CHECK-NEXT:    ret i8 [[OR_2]]
;
entry:
  %shr4 = lshr i8 %v, 4
  %shl7 = shl i8 %v, 4
  %or = or i8 %shr4, %shl7
  %shr4.1 = lshr i8 %or, 2
  %and.1 = and i8 %shr4.1, 51
  %shl7.1 = shl i8 %or, 2
  %and9.1 = and i8 %shl7.1, -52
  %or.1 = or i8 %and.1, %and9.1
  %shr4.2 = lshr i8 %or.1, 1
  %and.2 = and i8 %shr4.2, 85
  %shl7.2 = shl i8 %or.1, 1
  %and9.2 = and i8 %shl7.2, -86
  %or.2 = or i8 %and.2, %and9.2
  ret i8 %or.2
}

define i16 @rev16(i16 %v) {
; CHECK-LABEL: @rev16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[OR:%.*]] = call i16 @llvm.bswap.i16(i16 [[V:%.*]])
; CHECK-NEXT:    [[SHR4_1:%.*]] = lshr i16 [[OR]], 4
; CHECK-NEXT:    [[AND_1:%.*]] = and i16 [[SHR4_1]], 3855
; CHECK-NEXT:    [[SHL7_1:%.*]] = shl i16 [[OR]], 4
; CHECK-NEXT:    [[AND9_1:%.*]] = and i16 [[SHL7_1]], -3856
; CHECK-NEXT:    [[OR_1:%.*]] = or i16 [[AND_1]], [[AND9_1]]
; CHECK-NEXT:    [[SHR4_2:%.*]] = lshr i16 [[OR_1]], 2
; CHECK-NEXT:    [[AND_2:%.*]] = and i16 [[SHR4_2]], 13107
; CHECK-NEXT:    [[SHL7_2:%.*]] = shl i16 [[OR_1]], 2
; CHECK-NEXT:    [[AND9_2:%.*]] = and i16 [[SHL7_2]], -13108
; CHECK-NEXT:    [[OR_2:%.*]] = or i16 [[AND_2]], [[AND9_2]]
; CHECK-NEXT:    [[SHR4_3:%.*]] = lshr i16 [[OR_2]], 1
; CHECK-NEXT:    [[AND_3:%.*]] = and i16 [[SHR4_3]], 21845
; CHECK-NEXT:    [[SHL7_3:%.*]] = shl i16 [[OR_2]], 1
; CHECK-NEXT:    [[AND9_3:%.*]] = and i16 [[SHL7_3]], -21846
; CHECK-NEXT:    [[OR_3:%.*]] = or i16 [[AND_3]], [[AND9_3]]
; CHECK-NEXT:    ret i16 [[OR_3]]
;
entry:
  %shr4 = lshr i16 %v, 8
  %shl7 = shl i16 %v, 8
  %or = or i16 %shr4, %shl7
  %shr4.1 = lshr i16 %or, 4
  %and.1 = and i16 %shr4.1, 3855
  %shl7.1 = shl i16 %or, 4
  %and9.1 = and i16 %shl7.1, -3856
  %or.1 = or i16 %and.1, %and9.1
  %shr4.2 = lshr i16 %or.1, 2
  %and.2 = and i16 %shr4.2, 13107
  %shl7.2 = shl i16 %or.1, 2
  %and9.2 = and i16 %shl7.2, -13108
  %or.2 = or i16 %and.2, %and9.2
  %shr4.3 = lshr i16 %or.2, 1
  %and.3 = and i16 %shr4.3, 21845
  %shl7.3 = shl i16 %or.2, 1
  %and9.3 = and i16 %shl7.3, -21846
  %or.3 = or i16 %and.3, %and9.3
  ret i16 %or.3
}

define i32 @rev32(i32 %v) {
; CHECK-LABEL: @rev32(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[OR_1:%.*]] = call i32 @llvm.bswap.i32(i32 [[V:%.*]])
; CHECK-NEXT:    [[SHR1_2:%.*]] = lshr i32 [[OR_1]], 4
; CHECK-NEXT:    [[AND_2:%.*]] = and i32 [[SHR1_2]], 252645135
; CHECK-NEXT:    [[SHL2_2:%.*]] = shl i32 [[OR_1]], 4
; CHECK-NEXT:    [[AND3_2:%.*]] = and i32 [[SHL2_2]], -252645136
; CHECK-NEXT:    [[OR_2:%.*]] = or i32 [[AND_2]], [[AND3_2]]
; CHECK-NEXT:    [[SHR1_3:%.*]] = lshr i32 [[OR_2]], 2
; CHECK-NEXT:    [[AND_3:%.*]] = and i32 [[SHR1_3]], 858993459
; CHECK-NEXT:    [[SHL2_3:%.*]] = shl i32 [[OR_2]], 2
; CHECK-NEXT:    [[AND3_3:%.*]] = and i32 [[SHL2_3]], -858993460
; CHECK-NEXT:    [[OR_3:%.*]] = or i32 [[AND_3]], [[AND3_3]]
; CHECK-NEXT:    [[SHR1_4:%.*]] = lshr i32 [[OR_3]], 1
; CHECK-NEXT:    [[AND_4:%.*]] = and i32 [[SHR1_4]], 1431655765
; CHECK-NEXT:    [[SHL2_4:%.*]] = shl i32 [[OR_3]], 1
; CHECK-NEXT:    [[AND3_4:%.*]] = and i32 [[SHL2_4]], -1431655766
; CHECK-NEXT:    [[OR_4:%.*]] = or i32 [[AND_4]], [[AND3_4]]
; CHECK-NEXT:    ret i32 [[OR_4]]
;
entry:
  %shr1 = lshr i32 %v, 16
  %shl2 = shl i32 %v, 16
  %or = or i32 %shr1, %shl2
  %shr1.1 = lshr i32 %or, 8
  %and.1 = and i32 %shr1.1, 16711935
  %shl2.1 = shl i32 %or, 8
  %and3.1 = and i32 %shl2.1, -16711936
  %or.1 = or i32 %and.1, %and3.1
  %shr1.2 = lshr i32 %or.1, 4
  %and.2 = and i32 %shr1.2, 252645135
  %shl2.2 = shl i32 %or.1, 4
  %and3.2 = and i32 %shl2.2, -252645136
  %or.2 = or i32 %and.2, %and3.2
  %shr1.3 = lshr i32 %or.2, 2
  %and.3 = and i32 %shr1.3, 858993459
  %shl2.3 = shl i32 %or.2, 2
  %and3.3 = and i32 %shl2.3, -858993460
  %or.3 = or i32 %and.3, %and3.3
  %shr1.4 = lshr i32 %or.3, 1
  %and.4 = and i32 %shr1.4, 1431655765
  %shl2.4 = shl i32 %or.3, 1
  %and3.4 = and i32 %shl2.4, -1431655766
  %or.4 = or i32 %and.4, %and3.4
  ret i32 %or.4
}

define i64 @rev64(i64 %v) {
; CHECK-LABEL: @rev64(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[OR_2:%.*]] = call i64 @llvm.bswap.i64(i64 [[V:%.*]])
; CHECK-NEXT:    [[SHR2_3:%.*]] = lshr i64 [[OR_2]], 4
; CHECK-NEXT:    [[AND_3:%.*]] = and i64 [[SHR2_3]], 1085102592571150095
; CHECK-NEXT:    [[SHL4_3:%.*]] = shl i64 [[OR_2]], 4
; CHECK-NEXT:    [[AND5_3:%.*]] = and i64 [[SHL4_3]], -1085102592571150096
; CHECK-NEXT:    [[OR_3:%.*]] = or i64 [[AND_3]], [[AND5_3]]
; CHECK-NEXT:    [[SHR2_4:%.*]] = lshr i64 [[OR_3]], 2
; CHECK-NEXT:    [[AND_4:%.*]] = and i64 [[SHR2_4]], 3689348814741910323
; CHECK-NEXT:    [[SHL4_4:%.*]] = shl i64 [[OR_3]], 2
; CHECK-NEXT:    [[AND5_4:%.*]] = and i64 [[SHL4_4]], -3689348814741910324
; CHECK-NEXT:    [[OR_4:%.*]] = or i64 [[AND_4]], [[AND5_4]]
; CHECK-NEXT:    [[SHR2_5:%.*]] = lshr i64 [[OR_4]], 1
; CHECK-NEXT:    [[AND_5:%.*]] = and i64 [[SHR2_5]], 6148914691236517205
; CHECK-NEXT:    [[SHL4_5:%.*]] = shl i64 [[OR_4]], 1
; CHECK-NEXT:    [[AND5_5:%.*]] = and i64 [[SHL4_5]], -6148914691236517206
; CHECK-NEXT:    [[OR_5:%.*]] = or i64 [[AND_5]], [[AND5_5]]
; CHECK-NEXT:    ret i64 [[OR_5]]
;
entry:
  %shr2 = lshr i64 %v, 32
  %shl4 = shl i64 %v, 32
  %or = or i64 %shr2, %shl4
  %shr2.1 = lshr i64 %or, 16
  %and.1 = and i64 %shr2.1, 281470681808895
  %shl4.1 = shl i64 %or, 16
  %and5.1 = and i64 %shl4.1, -281470681808896
  %or.1 = or i64 %and.1, %and5.1
  %shr2.2 = lshr i64 %or.1, 8
  %and.2 = and i64 %shr2.2, 71777214294589695
  %shl4.2 = shl i64 %or.1, 8
  %and5.2 = and i64 %shl4.2, -71777214294589696
  %or.2 = or i64 %and.2, %and5.2
  %shr2.3 = lshr i64 %or.2, 4
  %and.3 = and i64 %shr2.3, 1085102592571150095
  %shl4.3 = shl i64 %or.2, 4
  %and5.3 = and i64 %shl4.3, -1085102592571150096
  %or.3 = or i64 %and.3, %and5.3
  %shr2.4 = lshr i64 %or.3, 2
  %and.4 = and i64 %shr2.4, 3689348814741910323
  %shl4.4 = shl i64 %or.3, 2
  %and5.4 = and i64 %shl4.4, -3689348814741910324
  %or.4 = or i64 %and.4, %and5.4
  %shr2.5 = lshr i64 %or.4, 1
  %and.5 = and i64 %shr2.5, 6148914691236517205
  %shl4.5 = shl i64 %or.4, 1
  %and5.5 = and i64 %shl4.5, -6148914691236517206
  %or.5 = or i64 %and.5, %and5.5
  ret i64 %or.5
}

;unsigned char rev8_xor(unsigned char x) {
;  unsigned char y;
;  y = x&0x55; x ^= y; x |= (y<<2)|(y>>6);
;  y = x&0x66; x ^= y; x |= (y<<4)|(y>>4);
;  return (x<<1)|(x>>7);
;}

define i8 @rev8_xor(i8 %0) {
; CHECK-LABEL: @rev8_xor(
; CHECK-NEXT:    [[TMP2:%.*]] = and i8 [[TMP0:%.*]], 85
; CHECK-NEXT:    [[TMP3:%.*]] = and i8 [[TMP0]], -86
; CHECK-NEXT:    [[TMP4:%.*]] = shl i8 [[TMP2]], 2
; CHECK-NEXT:    [[TMP5:%.*]] = lshr i8 [[TMP2]], 6
; CHECK-NEXT:    [[TMP6:%.*]] = or i8 [[TMP5]], [[TMP3]]
; CHECK-NEXT:    [[TMP7:%.*]] = or i8 [[TMP6]], [[TMP4]]
; CHECK-NEXT:    [[TMP8:%.*]] = and i8 [[TMP7]], 102
; CHECK-NEXT:    [[TMP9:%.*]] = and i8 [[TMP7]], 25
; CHECK-NEXT:    [[TMP10:%.*]] = lshr i8 [[TMP8]], 4
; CHECK-NEXT:    [[TMP11:%.*]] = or i8 [[TMP10]], [[TMP9]]
; CHECK-NEXT:    [[TMP12:%.*]] = shl i8 [[TMP8]], 5
; CHECK-NEXT:    [[TMP13:%.*]] = shl nuw nsw i8 [[TMP11]], 1
; CHECK-NEXT:    [[TMP14:%.*]] = or i8 [[TMP12]], [[TMP13]]
; CHECK-NEXT:    [[TMP15:%.*]] = lshr i8 [[TMP0]], 7
; CHECK-NEXT:    [[TMP16:%.*]] = or i8 [[TMP14]], [[TMP15]]
; CHECK-NEXT:    ret i8 [[TMP16]]
;
  %2 = and i8 %0, 85
  %3 = xor i8 %0, %2
  %4 = shl i8 %2, 2
  %5 = lshr i8 %2, 6
  %6 = or i8 %5, %3
  %7 = or i8 %6, %4
  %8 = and i8 %7, 102
  %9 = xor i8 %7, %8
  %10 = lshr i8 %8, 4
  %11 = or i8 %10, %9
  %12 = shl i8 %8, 5
  %13 = shl i8 %11, 1
  %14 = or i8 %12, %13
  %15 = lshr i8 %0, 7
  %16 = or i8 %14, %15
  ret i8 %16
}

define <2 x i8> @rev8_xor_vector(<2 x i8> %0) {
; CHECK-LABEL: @rev8_xor_vector(
; CHECK-NEXT:    [[TMP2:%.*]] = and <2 x i8> [[TMP0:%.*]], <i8 85, i8 85>
; CHECK-NEXT:    [[TMP3:%.*]] = and <2 x i8> [[TMP0]], <i8 -86, i8 -86>
; CHECK-NEXT:    [[TMP4:%.*]] = shl <2 x i8> [[TMP2]], <i8 2, i8 2>
; CHECK-NEXT:    [[TMP5:%.*]] = lshr <2 x i8> [[TMP2]], <i8 6, i8 6>
; CHECK-NEXT:    [[TMP6:%.*]] = or <2 x i8> [[TMP5]], [[TMP3]]
; CHECK-NEXT:    [[TMP7:%.*]] = or <2 x i8> [[TMP6]], [[TMP4]]
; CHECK-NEXT:    [[TMP8:%.*]] = and <2 x i8> [[TMP7]], <i8 102, i8 102>
; CHECK-NEXT:    [[TMP9:%.*]] = and <2 x i8> [[TMP7]], <i8 25, i8 25>
; CHECK-NEXT:    [[TMP10:%.*]] = lshr <2 x i8> [[TMP8]], <i8 4, i8 4>
; CHECK-NEXT:    [[TMP11:%.*]] = or <2 x i8> [[TMP10]], [[TMP9]]
; CHECK-NEXT:    [[TMP12:%.*]] = shl <2 x i8> [[TMP8]], <i8 5, i8 5>
; CHECK-NEXT:    [[TMP13:%.*]] = shl nuw nsw <2 x i8> [[TMP11]], <i8 1, i8 1>
; CHECK-NEXT:    [[TMP14:%.*]] = or <2 x i8> [[TMP12]], [[TMP13]]
; CHECK-NEXT:    [[TMP15:%.*]] = lshr <2 x i8> [[TMP0]], <i8 7, i8 7>
; CHECK-NEXT:    [[TMP16:%.*]] = or <2 x i8> [[TMP14]], [[TMP15]]
; CHECK-NEXT:    ret <2 x i8> [[TMP16]]
;
  %2 = and <2 x i8> %0, <i8 85, i8 85>
  %3 = xor <2 x i8> %0, %2
  %4 = shl <2 x i8> %2, <i8 2, i8 2>
  %5 = lshr <2 x i8> %2, <i8 6, i8 6>
  %6 = or <2 x i8> %5, %3
  %7 = or <2 x i8> %6, %4
  %8 = and <2 x i8> %7, <i8 102, i8 102>
  %9 = xor <2 x i8> %7, %8
  %10 = lshr <2 x i8> %8, <i8 4, i8 4>
  %11 = or <2 x i8> %10, %9
  %12 = shl <2 x i8> %8, <i8 5, i8 5>
  %13 = shl <2 x i8> %11, <i8 1, i8 1>
  %14 = or <2 x i8> %12, %13
  %15 = lshr <2 x i8> %0, <i8 7, i8 7>
  %16 = or <2 x i8> %14, %15
  ret <2 x i8> %16
}
