#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dietwarning.h>

link_warning("tempnam","\e[1;33;41m>>> tempnam stinks! NEVER ! NEVER USE IT ! <<<\e[0m");

char* tempnam(char* dir,char* template) {
  char buf[1024];
  int len=sizeof(buf)-2,fd;
  buf[len]=0;
  if ((dir)&&(*dir)) {
    char* c=memccpy(buf,dir,0,len);
    if (!c) return 0;
    memcpy(c-1,"/",2);
  }
  else
    strncpy(buf,"/tmp/",len);
  len=(sizeof(buf)-1)-strlen(buf);
  if (len<1) return 0;
  if (template)
    strncat(buf,template, --len);
  else
    strncat(buf,"temp_", --len);
  len=(sizeof(buf)-1)-strlen(buf);
  strncat(buf,"XXXXXX",len);
  if ((fd=mkstemp(buf))<0) return 0;
  close(fd);
  unlink(buf);
  return strdup(buf);
}
