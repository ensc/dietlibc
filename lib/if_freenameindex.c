#include <net/if.h>
#include <stdlib.h>

void if_freenameindex(struct if_nameindex* ptr) {
  free(ptr);
}
