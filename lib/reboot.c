#include <sys/reboot.h>

int __reboot(int magic1, int magic2, int cmd, void * arg);

int reboot(int cmd, void *arg)
{
  return __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, cmd, arg);
}
