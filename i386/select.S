#include "syscalls.h"

.text
.weak select
select:
syscall(_newselect,select)
