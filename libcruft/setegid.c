#include <sys/types.h>
#include <unistd.h>

#undef setegid
int setegid(gid_t gid) {
  return setregid((gid_t)-1,gid);
}

