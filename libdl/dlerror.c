#include <string.h>

#include "_dl_int.h"

int _dl_error;
const char* _dl_error_location;
const char* _dl_error_data;

static struct _dl_err_msg {
  char *msg;
  int len;
} _dl_error_msg[] = {
#define MSG(n) { (n), sizeof((n)) }
  MSG("can't open: "),
  MSG("can't stat: "),
  MSG("shared object is not position independent: "),
  MSG("can't resolve all symbols in: "),
  MSG("can't find symbol: "),
};

const char *dlerror(void)
{
  static char buf[1024];
  register int len=sizeof(buf)-1;
  buf[0]=0;
  buf[len]=0;

  if (_dl_error>=(sizeof(_dl_error_msg)/sizeof(struct _dl_err_msg)))
    return "HAE ?!?";

  strncpy(buf,_dl_error_location,len); len -= sizeof(_dl_error_location);
  strncat(buf,": ",len); len-=2;
  strncat(buf,_dl_error_msg[_dl_error].msg,len); len -= _dl_error_msg[_dl_error].len;
  strncat(buf,_dl_error_data,len);

  _dl_error_data="";
  return buf;
}
