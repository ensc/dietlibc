#include <stdlib.h>
#include <assert.h>

main() {
  char buf[1024];
  assert(__ltostr(buf,10,1723,10,0)==4); assert(!strcmp(buf,"1723"));
  assert(__ltostr(buf,3,1723,10,0)==2); assert(!strcmp(buf,"23"));
  assert(__ltostr(buf,2,0x1234,16,0)==1); assert(!strcmp(buf,"4"));
  assert(__ltostr(buf,3,0xFEFE,16,1)==2); assert(!strcmp(buf,"FE"));
}
