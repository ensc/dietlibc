#include "arm-features.h"

FUNC_START __aeabi_unwind_cpp_pr0
FUNC_START __aeabi_unwind_cpp_pr1
FUNC_START __aeabi_unwind_cpp_pr2
.hidden __aeabi_unwind_cpp_pr0
.hidden __aeabi_unwind_cpp_pr1
.hidden __aeabi_unwind_cpp_pr2

	mov	pc, lr	@ return from subroutine

FUNC_END  __aeabi_unwind_cpp_pr2
FUNC_END  __aeabi_unwind_cpp_pr1
FUNC_END  __aeabi_unwind_cpp_pr0
