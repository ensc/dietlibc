#ifndef _NETINET_UDP_H
#define _NETINET_UDP_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct udphdr {
  uint16_t source;
  uint16_t dest;
  uint16_t len;
  uint16_t check;
};

#define SOL_UDP            17      /* sockopt level for UDP */

#endif
