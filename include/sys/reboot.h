#ifndef _SYS_REBOOT_H
#define _SYS_REBOOT_H

#include <linux/reboot.h>

/* Reboot or halt the system.  */
int reboot (int flag, void *arg);

#endif	/* _SYS_REBOOT_H */
