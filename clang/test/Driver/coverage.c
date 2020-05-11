// RUN: %clang -### -S -fprofile-arcs %s 2>&1 | FileCheck --check-prefix=PROFILE-ARCS %s
// RUN: %clang -### -S -fprofile-arcs -fno-profile-arcs %s 2>&1 | FileCheck --check-prefix=NO-PROFILE-ARCS %s

// PROFILE-ARCS: "-femit-coverage-data"
// PROFILE-ARCS: "-coverage-notes-file" "{{.*}}{{/|\\\\}}coverage.c"
// NO-PROFILE-ARCS-NOT: "-femit-coverage-data"

// RUN: %clang -### -S -fprofile-arcs %s -o /foo/bar.o 2>&1 | FileCheck --check-prefix=GCNO-LOCATION %s
// RUN: %clang_cl -### /c --coverage /Fo/foo/bar.obj -- %s 2>&1 | FileCheck --check-prefix=GCNO-LOCATION %s
// RUN: %clang -### -c -fprofile-arcs %s -o foo/bar.o 2>&1 | FileCheck --check-prefix=GCNO-LOCATION-REL %s

// GCNO-LOCATION: "-coverage-notes-file" "{{.*}}/foo/bar.gcno"
// GCNO-LOCATION-REL: "-coverage-notes-file" "{{.*}}{{/|\\\\}}foo/bar.gcno"

/// Test -fprofile-dir=
// RUN: not %clang -S -Werror -fprofile-dir=abc %s
// RUN: not %clang -S -Werror -ftest-coverage -fprofile-dir=abc %s
// RUN: %clang -### -S -fprofile-arcs -fprofile-dir=abc %s 2>&1 | FileCheck --check-prefix=PROFILE-DIR %s
// RUN: %clang -### -S --coverage -fprofile-dir=abc %s 2>&1 | FileCheck --check-prefix=PROFILE-DIR %s

// PROFILE-DIR: "-coverage-data-file" "abc

/// These should only get passed if any of --coverage, -ftest-coverage, or
/// -fprofile-arcs is passed.
// RUN: %clang -### -c %s 2>&1 | FileCheck --check-prefix=NO-COV %s
// NO-COV-NOT: "-coverage-notes-file"
// NO-COV-NOT: "-coverage-data-file"
