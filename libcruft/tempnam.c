#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <dietwarning.h>

link_warning("tempnam","\e[1;33;41m>>> tempnam stinks! NEVER ! NEVER USE IT ! <<<\e[0m");

static char buf[1024];
char* tempnam(char* dir,char* template) {
  int len=sizeof(buf)-1,fd;
  buf[len]=0;
  strncpy(buf,(dir)?dir:"/tmp",len);
  len=(sizeof(buf)-1)-strlen(buf);
  strncat(buf,"/",1);
  strncat(buf,template, --len);
  len=(sizeof(buf)-1)-strlen(buf);
  strncat(buf,"XXXXXX",len);
  if ((fd=mkstemp(template))<0) return 0;
  close(fd);
  return template;
}
