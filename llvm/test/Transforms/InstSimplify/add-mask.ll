; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -instsimplify < %s | FileCheck %s

define i1 @test1(i32 %a) {
; CHECK-LABEL: @test1(
; CHECK-NEXT:    ret i1 false
;
  %rhs = add i32 %a, -1
  %and = and i32 %a, %rhs
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

define i1 @test1v(<2 x i32> %a) {
; CHECK-LABEL: @test1v(
; CHECK-NEXT:    [[RHS:%.*]] = add <2 x i32> [[A:%.*]], <i32 -1, i32 0>
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i32> [[A]], [[RHS]]
; CHECK-NEXT:    [[EXT:%.*]] = extractelement <2 x i32> [[AND]], i32 0
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[EXT]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %rhs = add <2 x i32> %a, <i32 -1, i32 0>
  %and = and <2 x i32> %a, %rhs
  %ext = extractelement <2 x i32> %and, i32 0
  %res = icmp eq i32 %ext, 1
  ret i1 %res
}

define i1 @test2(i32 %a) {
; CHECK-LABEL: @test2(
; CHECK-NEXT:    ret i1 false
;
  %rhs = add i32 %a, 1
  %and = and i32 %a, %rhs
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

define i1 @test2v(<2 x i32> %a) {
; CHECK-LABEL: @test2v(
; CHECK-NEXT:    [[RHS:%.*]] = add <2 x i32> [[A:%.*]], <i32 0, i32 1>
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i32> [[A]], [[RHS]]
; CHECK-NEXT:    [[EXT:%.*]] = extractelement <2 x i32> [[AND]], i32 1
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[EXT]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %rhs = add <2 x i32> %a, <i32 0, i32 1>
  %and = and <2 x i32> %a, %rhs
  %ext = extractelement <2 x i32> %and, i32 1
  %res = icmp eq i32 %ext, 1
  ret i1 %res
}

define i1 @test3(i32 %a) {
; CHECK-LABEL: @test3(
; CHECK-NEXT:    ret i1 false
;
  %rhs = add i32 %a, 7
  %and = and i32 %a, %rhs
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

define i1 @test3v(<2 x i32> %a) {
; CHECK-LABEL: @test3v(
; CHECK-NEXT:    [[RHS:%.*]] = add <2 x i32> [[A:%.*]], <i32 7, i32 0>
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i32> [[A]], [[RHS]]
; CHECK-NEXT:    [[EXT:%.*]] = extractelement <2 x i32> [[AND]], i32 0
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[EXT]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %rhs = add <2 x i32> %a, <i32 7, i32 0>
  %and = and <2 x i32> %a, %rhs
  %ext = extractelement <2 x i32> %and, i32 0
  %res = icmp eq i32 %ext, 1
  ret i1 %res
}

@B = external global i32
declare void @llvm.assume(i1)

; Known bits without a constant
define i1 @test4(i32 %a) {
; CHECK-LABEL: @test4(
; CHECK-NEXT:    [[B:%.*]] = load i32, i32* @B
; CHECK-NEXT:    [[B_AND:%.*]] = and i32 [[B]], 1
; CHECK-NEXT:    [[B_CND:%.*]] = icmp eq i32 [[B_AND]], 1
; CHECK-NEXT:    call void @llvm.assume(i1 [[B_CND]])
; CHECK-NEXT:    ret i1 false
;
  %b = load i32, i32* @B
  %b.and = and i32 %b, 1
  %b.cnd = icmp eq i32 %b.and, 1
  call void @llvm.assume(i1 %b.cnd)

  %rhs = add i32 %a, %b
  %and = and i32 %a, %rhs
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

; Negative test - even number
define i1 @test5(i32 %a) {
; CHECK-LABEL: @test5(
; CHECK-NEXT:    [[RHS:%.*]] = add i32 [[A:%.*]], 2
; CHECK-NEXT:    [[AND:%.*]] = and i32 [[A]], [[RHS]]
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[AND]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %rhs = add i32 %a, 2
  %and = and i32 %a, %rhs
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

define i1 @test5v(<2 x i32> %a) {
; CHECK-LABEL: @test5v(
; CHECK-NEXT:    [[RHS:%.*]] = add <2 x i32> [[A:%.*]], <i32 2, i32 0>
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i32> [[A]], [[RHS]]
; CHECK-NEXT:    [[EXT:%.*]] = extractelement <2 x i32> [[AND]], i32 1
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[EXT]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %rhs = add <2 x i32> %a, <i32 2, i32 0>
  %and = and <2 x i32> %a, %rhs
  %ext = extractelement <2 x i32> %and, i32 1
  %res = icmp eq i32 %ext, 1
  ret i1 %res
}

define i1 @test6(i32 %a) {
; CHECK-LABEL: @test6(
; CHECK-NEXT:    ret i1 false
;
  %lhs = add i32 %a, -1
  %and = and i32 %lhs, %a
  %res = icmp eq i32 %and, 1
  ret i1 %res
}

define i1 @test6v(<2 x i32> %a) {
; CHECK-LABEL: @test6v(
; CHECK-NEXT:    [[LHS:%.*]] = add <2 x i32> [[A:%.*]], <i32 0, i32 -1>
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i32> [[LHS]], [[A]]
; CHECK-NEXT:    [[EXT:%.*]] = extractelement <2 x i32> [[AND]], i32 1
; CHECK-NEXT:    [[RES:%.*]] = icmp eq i32 [[EXT]], 1
; CHECK-NEXT:    ret i1 [[RES]]
;
  %lhs = add <2 x i32> %a, <i32 0, i32 -1>
  %and = and <2 x i32> %lhs, %a
  %ext = extractelement <2 x i32> %and, i32 1
  %res = icmp eq i32 %ext, 1
  ret i1 %res
}
