#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

char *getpass(const char* prompt) {
  struct termios old,tmp;
  int out,in=open("/dev/tty",O_RDWR);
  int doclose=(in>=0);
  static char buf[PASS_MAX];
  if (!doclose) { in=0; out=2; } else out=in;
  if (!tcgetattr(in,&old)) {
    tmp=old;
    tmp.c_lflag &= ~(ECHO|ISIG);
    tcsetattr(in,TCSAFLUSH,&tmp);
  }
  write(out,prompt,strlen(prompt));
  {
    int nread,ofs=0;
    for (;;) {
      nread=read(in,buf+ofs,PASS_MAX-ofs);
      if (nread<0) {
	buf[ofs]=0;
	break;
      } else if (ofs+nread>=PASS_MAX) {
	buf[PASS_MAX-1]=0;
	break;
      } else if (buf[ofs+nread-1]=='\n') {
	buf[ofs+nread-1]=0;
	break;
      }
      ofs+=nread;
    }
    write(out,"\n",1);
  }
  tcsetattr(in,TCSAFLUSH,&old);
  if (doclose) close(in);
  return buf;
}
