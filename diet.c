#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "dietfeatures.h"

/* goal:
 *   when invoked as
 * "diet gcc -c t.c"
 *   exec
 * "gcc -I/path/to/dietlibc/include -c t.c"
 *
 *   when invoked as
 * "diet sparc-linux-gcc -o t t.o"
 *   exec
 * "sparc-linux-gcc -nostdlib -static -o t t.o /path/to/dietlibc/bin-sparc/start.o /path/to/dietlibc/bin-sparc/dietlibc.a"
*/

void error(const char *message) {
  write(2,message,strlen(message));
  exit(1);
}

int main(int argc,char *argv[]) {
  int _link=0;
  int compile=0;
  char diethome[]=DIETHOME;
  char platform[1000]=DIETHOME "/bin-";
  const char *nostdlib="-nostdlib";
  const char *libgcc="-lgcc";
  char dashL[1000]="-L";
  int i;

  if (argc<2) {
    error("usage: diet [gcc command line]\n"
	  "e.g.   diet gcc -c t.c\n"
	  "or     diet sparc-linux-gcc -o foo foo.c bar.o\n");
  }
  {
    char *tmp=strchr(argv[1],0)-2;
    char *tmp2;
    char *cc=argv[1];
    if (tmp<cc) goto donttouch;
    if ((tmp2=strchr(cc,'-'))) {	/* cross compiling? */
      int len=strlen(platform);
      if (tmp2-cc>90) error("platform name too long!\n");
      memmove(platform+len,argv[1],tmp2-cc);
      platform[tmp2-cc+len]=0;
/*      printf("found platform %s\n",platform); */
    } else {
#ifdef __sparc__
      strcat(platform,"sparc");
#endif
#ifdef __ppc__
      strcat(platform,"ppc");
#endif
#ifdef __i386__
      strcat(platform,"i386");
#endif
#ifdef __alpha__
      strcat(platform,"alpha");
#endif
#ifdef __arm__
      strcat(platform,"arm");
#endif
#ifdef __mips__
      strcat(platform,"mips");
#endif
    }
    strcat(dashL,platform);
    if (!strcmp(tmp,"cc")) {
      char **newargv;
      char **dest;
      char *a,*b,*c;
#ifdef WANT_DYNAMIC
      char *d,*e;
#endif
/* we need to add -I... if the command line contains -c, -S or -E */
      for (i=2; i<argc; ++i)
	if (!strcmp(argv[i],"-c") || !strcmp(argv[i],"-S") || !strcmp(argv[i],"-E"))
	  compile=1;
/* we need to add -nostdlib if we are not compiling*/
      _link=!compile;
#if 0
      for (i=2; i<argc; ++i)
	if (!strcmp(argv[i],"-o"))
	  if (!compile) _link=1;
#endif
      newargv=alloca(sizeof(char*)*(argc+8));
      a=alloca(strlen(diethome)+20);
      b=alloca(strlen(platform)+20);
      c=alloca(strlen(platform)+20);

      strcpy(a,"-I"); strcat(a,diethome); strcat(a,"/include");
      strcpy(b,platform); strcat(b,"/start.o");
      strcpy(c,platform); strcat(c,"/dietlibc.a");

#ifdef WANT_DYNAMIC
      d=alloca(strlen(platform)+20);
      e=alloca(strlen(platform)+20);
      strcpy(d,platform); strcat(d,"/dyn_start.o");
      strcpy(e,platform); strcat(e,"/dyn_stop.o");
#endif

      dest=newargv;
      *dest++=argv[1];
      if (_link) { *dest++=(char*)nostdlib; *dest++=dashL; }
      if (compile || _link) *dest++=a;
      if (_link) { *dest++=b; }
#ifdef WANT_DYNAMIC
      if (_link) { *dest++=d; }
#endif
      for (i=2; i<argc; ++i)
	*dest++=argv[i];
      if (_link) { *dest++=c; *dest++=(char*)libgcc; }
#ifdef WANT_DYNAMIC
      if (_link) { *dest++=e; }
#endif
      *dest=0;
      execvp(newargv[0],newargv);
      goto error;
    } else if (!strcmp(tmp,"ld")) {
    }
  }
donttouch:
  execvp(argv[1],argv+1);
error:
  error("execvp failed!\n");
  return 1;
}
