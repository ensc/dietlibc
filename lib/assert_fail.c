#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int __ltostr(char *s, int size, unsigned long i, int base, char UpCase);

void __assert_fail (const char *assertion, const char *file, unsigned int line, const char *function)
{
  int alen=strlen(assertion);
  int flen=strlen(file);
  int fulen=strlen(function);
  char *buf=(char*)alloca(alen+flen+fulen+50);
  if (buf) {
    char *tmp;
    *buf=0;
    if (file) strcat(strcat(buf,file),":");
    tmp=buf+strlen(buf);
    __ltostr(tmp,10,line,10,0);
    strcat(buf,": ");
    if (function) strcat(strcat(buf,function),": ");
    strcat(buf,"Assertion `");
    strcat(buf,assertion);
    strcat(buf,"' failed.\n");
    write(2,buf,strlen(buf));
  }
  abort();
}
