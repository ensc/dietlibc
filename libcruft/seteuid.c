#include <sys/types.h>
#include <unistd.h>

#undef seteuid
int seteuid(uid_t uid) {
  return setreuid((uid_t)-1,uid);
}

