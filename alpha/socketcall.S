#include "syscalls.h"

/*
 * Doesn't exist on alpha ( accept, bind, ... are SYSCALLS !?! )
 */

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
