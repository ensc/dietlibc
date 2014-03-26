#include <libintl.h>

char* dcgettext(const char *domainname, const char *msgid, int category) {
  (void)domainname;
  (void)category;
  return (char*)msgid;
}
