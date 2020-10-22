; RUN: llc -march=nvptx -verify-machineinstrs < %s | FileCheck %s

; Tests the following pattern:
; (X & 8) != 0 --> (X & 8) >> 3

; CHECK-LABEL: @pow2_mask_cmp
; CHECK: bfe.u32 {{%r[0-9]+}}, {{%r[0-9]+}}, 3, 1
define i32 @pow2_mask_cmp(i32 %x) {
  %a = and i32 %x, 8
  %cmp = icmp ne i32 %a, 0
  %r = zext i1 %cmp to i32
  ret i32 %r
}
