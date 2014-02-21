#include <libintl.h>
#include <string.h>
#include <stdlib.h>

static char* dir;

char* bindtextdomain(const char* domainname,const char* dirname) {
  (void)domainname;
  if (dir) free(dir);
  if (!(dir=strdup(dirname))) return 0;
  return dir;
}
