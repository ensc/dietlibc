#include <time.h>

char *ctime(const time_t *timep) {
  return asctime(gmtime(timep));
}
