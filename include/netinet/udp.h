#ifndef _NETINET_UDP_H
#define _NETINET_UDP_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct udphdr {		/* size 8 */
  uint16_t source;	/* offset 0 */
  uint16_t dest;	/* offset 2 */
  uint16_t len;		/* offset 4 */
  uint16_t check;	/* offset 6 */
};

#define SOL_UDP            17      /* sockopt level for UDP */

#endif
