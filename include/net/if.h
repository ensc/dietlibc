#ifndef _NET_IF_H
#define _NET_IF_H

#include <sys/cdefs.h>

unsigned int if_nametoindex (const char *ifname) __THROW;
char *if_indextoname (unsigned int ifindex, char *ifname) __THROW;

#endif
