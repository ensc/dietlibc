#include <string.h>
#include <netdb.h>

extern int getservbyport_r(int port,const char* proto,
			   struct servent *res, char *buf, size_t buflen,
			   struct servent **res_sig) {
  int _res=-1;
  while (!getservent_r(res,buf,buflen,res_sig))
    if (port==res->s_port && !strcmp(res->s_proto,proto)) { _res=0; break; }
  endservent();
  return _res;
}
