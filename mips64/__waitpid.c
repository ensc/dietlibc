/* mips64 doesn't support waitpid, only wait4; same as s390x, so reuse that code */
#include "../s390x/__waitpid.c"
