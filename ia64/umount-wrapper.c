#include <sys/mount.h>

int umount(const char *target) {
  return umount2(target, 0);
}
