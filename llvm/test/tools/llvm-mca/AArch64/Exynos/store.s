# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m3 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M3
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m4 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M4
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m5 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M5

stur	x0, [sp, #8]
strb	w0, [sp], #1
strh	w0, [sp, #2]!
str	x0, [sp, #8]
strb	w0, [sp, xzr]
strh	w0, [sp, xzr, lsl #1]
str	w0, [sp, wzr, sxtw]
str	x0, [sp, wzr, uxtw #3]
stnp	w0, w1, [sp, #8]
stp	x0, x1, [sp], #16
stp	w0, w1, [sp, #8]!

# ALL:      Iterations:        100
# ALL-NEXT: Instructions:      1100
# ALL-NEXT: Total Cycles:      1303

# M3-NEXT:  Total uOps:        1300
# M4-NEXT:  Total uOps:        1100
# M5-NEXT:  Total uOps:        1100

# ALL:      Dispatch Width:    6

# M3-NEXT:  uOps Per Cycle:    1.00
# M4-NEXT:  uOps Per Cycle:    0.84
# M5-NEXT:  uOps Per Cycle:    0.84

# ALL-NEXT: IPC:               0.84

# M3-NEXT:  Block RThroughput: 11.0
# M4-NEXT:  Block RThroughput: 5.5
# M5-NEXT:  Block RThroughput: 5.5

# ALL:      Instruction Info:
# ALL-NEXT: [1]: #uOps
# ALL-NEXT: [2]: Latency
# ALL-NEXT: [3]: RThroughput
# ALL-NEXT: [4]: MayLoad
# ALL-NEXT: [5]: MayStore
# ALL-NEXT: [6]: HasSideEffects (U)

# ALL:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:

# M3-NEXT:   1      1     1.00           *            stur	x0, [sp, #8]
# M3-NEXT:   1      1     1.00           *            strb	w0, [sp], #1
# M3-NEXT:   1      1     1.00           *            strh	w0, [sp, #2]!
# M3-NEXT:   1      1     1.00           *            str	x0, [sp, #8]
# M3-NEXT:   1      1     1.00           *            strb	w0, [sp, xzr]
# M3-NEXT:   1      1     1.00           *            strh	w0, [sp, xzr, lsl #1]
# M3-NEXT:   2      2     1.00           *            str	w0, [sp, wzr, sxtw]
# M3-NEXT:   2      2     1.00           *            str	x0, [sp, wzr, uxtw #3]
# M3-NEXT:   1      1     1.00           *            stnp	w0, w1, [sp, #8]
# M3-NEXT:   1      1     1.00           *            stp	x0, x1, [sp], #16
# M3-NEXT:   1      1     1.00           *            stp	w0, w1, [sp, #8]!

# M4-NEXT:   1      1     0.50           *            stur	x0, [sp, #8]
# M4-NEXT:   1      1     0.50           *            strb	w0, [sp], #1
# M4-NEXT:   1      1     0.50           *            strh	w0, [sp, #2]!
# M4-NEXT:   1      1     0.50           *            str	x0, [sp, #8]
# M4-NEXT:   1      1     0.50           *            strb	w0, [sp, xzr]
# M4-NEXT:   1      1     0.50           *            strh	w0, [sp, xzr, lsl #1]
# M4-NEXT:   1      2     0.50           *            str	w0, [sp, wzr, sxtw]
# M4-NEXT:   1      2     0.50           *            str	x0, [sp, wzr, uxtw #3]
# M4-NEXT:   1      1     0.50           *            stnp	w0, w1, [sp, #8]
# M4-NEXT:   1      1     0.50           *            stp	x0, x1, [sp], #16
# M4-NEXT:   1      1     0.50           *            stp	w0, w1, [sp, #8]!

# M5-NEXT:   1      1     0.50           *            stur	x0, [sp, #8]
# M5-NEXT:   1      1     0.50           *            strb	w0, [sp], #1
# M5-NEXT:   1      1     0.50           *            strh	w0, [sp, #2]!
# M5-NEXT:   1      1     0.50           *            str	x0, [sp, #8]
# M5-NEXT:   1      1     0.50           *            strb	w0, [sp, xzr]
# M5-NEXT:   1      1     0.50           *            strh	w0, [sp, xzr, lsl #1]
# M5-NEXT:   1      2     0.50           *            str	w0, [sp, wzr, sxtw]
# M5-NEXT:   1      2     0.50           *            str	x0, [sp, wzr, uxtw #3]
# M5-NEXT:   1      1     0.50           *            stnp	w0, w1, [sp, #8]
# M5-NEXT:   1      1     0.50           *            stp	x0, x1, [sp], #16
# M5-NEXT:   1      1     0.50           *            stp	w0, w1, [sp, #8]!
