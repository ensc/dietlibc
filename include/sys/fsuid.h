#ifndef __FSUID_H
#define __FSUID_H 1

#include <sys/types.h>

int setfsuid(uid_t uid);
int setfsgid(gid_t gid);

#endif
