#ifndef _NETINET_IN_H
#define _NETINET_IN_H

#include <sys/types.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/ipv6.h>

#define IPPORT_RESERVED 1024
#define INET6_ADDRSTRLEN 46

#define ipv6mr_interface ipv6mr_ifindex

#endif
