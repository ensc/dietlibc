#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>

extern int h_errno;

static const hostentsize=((sizeof(struct hostent)+15)&(-16));

struct hostent* gethostbyname (const char *host) {
  struct hostent *hostbuf;
  struct hostent *hp;
  size_t hstbuflen;
  char *tmphstbuf;
  int res;
  int herr;

  hstbuflen = 1024;
  if (!(tmphstbuf = malloc (hstbuflen))) return NULL;
  hostbuf=(struct hostent*)tmphstbuf;

  while ((res = gethostbyname_r (host, hostbuf, tmphstbuf+hostentsize, hstbuflen-hostentsize,
				  &hp, &herr)) == ERANGE)
    {
      /* Enlarge the buffer.  */
      hstbuflen *= 2;
      tmphstbuf = realloc (tmphstbuf, hstbuflen);
    }
  /*  Check for errors.  */
  if (res || hp == NULL) {
    free(tmphstbuf);
    return NULL;
  }
  return hp;
}

