#include <sys/types.h>
#include "syscalls.h"

#define __SYSCALL_STRING                                                \
        "ta     0x10;"                                                  \
        "bcs    2f;"                                                    \
        " nop;"                                                         \
        "1:"                                                            \
        ".subsection 2;"                                                \
        "2:"                                                            \
        "save   %%sp, -192, %%sp;"                                      \
        "call   __errno_location;"                                      \
        " nop;"                                                         \
        "st     %%i0,[%%o0];"                                           \
        "ba     1b;"                                                    \
        " restore %%g0, -1, %%o0;"                                      \
        ".previous;"

#define __SYSCALL_CLOBBERS "g2", "g3", "g4", "g5", "g7",                \
        "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",                 \
        "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",           \
        "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",         \
        "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",         \
        "cc", "memory"

#define inline_syscall6(name,arg1,arg2,arg3,arg4,arg5,arg6)             \
({                                                                      \
        register long __o0 __asm__ ("o0") = (long)(arg1);               \
        register long __o1 __asm__ ("o1") = (long)(arg2);               \
        register long __o2 __asm__ ("o2") = (long)(arg3);               \
        register long __o3 __asm__ ("o3") = (long)(arg4);               \
        register long __o4 __asm__ ("o4") = (long)(arg5);               \
        register long __o5 __asm__ ("o5") = (long)(arg6);               \
        register long __g1 __asm__ ("g1") = __NR_##name;                \
        __asm__ (__SYSCALL_STRING : "=r" (__g1), "=r" (__o0) :          \
                 "0" (__g1), "1" (__o0), "r" (__o1), "r" (__o2),        \
                 "r" (__o3), "r" (__o4), "r" (__o5) :                   \
                 __SYSCALL_CLOBBERS);                                   \
        __o0;                                                           \
})
int mmap(void*start,size_t length,int prot,int flags,int fd,off_t offset) {
  return inline_syscall6(mmap,start,length,prot,flags,fd,offset);
}
