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

static void error(const char *message) {
  write(2,message,strlen(message));
  exit(1);
}

static const char* Os[] = {
  "i386","-Os","-mpreferred-stack-boundary=2",
	 "-malign-functions=0","-malign-jumps=0",
	 "-malign-loops=0","-fomit-frame-pointer",0,
  "sparc","-Os","-mcpu=supersparc",0,
  "alpha","-Os","-fomit-frame-pointer",0,
  "arm","-Os","-fomit-frame-pointer",0,
  "mips","-Os","-fomit-frame-pointer","-fno-pic",0,
  "ppc","-Os","-fomit-frame-pointer","-mpowerpc-gpopt","-mpowerpc-gfxopt",0,
  0};

int main(int argc,char *argv[]) {
  int _link=0;
  int compile=0;
  char diethome[]=DIETHOME;
  char platform[1000]=DIETHOME "/bin-";
  char* shortplatform=0;
#ifdef WANT_SAFEGUARD
  char safeguard1[]="-include";
  char safeguard2[]=DIETHOME "/include/dietref.h";
#endif
  const char *nostdlib="-nostdlib";
  const char *libgcc="-lgcc";
  char dashL[1000]="-L";
  int i;
  int mangleopts=0;

  if (argc<2) {
    error("usage: diet [gcc command line]\n"
	  "e.g.   diet gcc -c t.c\n"
	  "or     diet sparc-linux-gcc -o foo foo.c bar.o\n");
  }
  if (!strcmp(argv[1],"-Os")) {
    ++argv; --argc;
    mangleopts=1;
  }
  {
    char *tmp=strchr(argv[1],0)-2;
    char *tmp2;
    char *cc=argv[1];
    if (tmp<cc) goto donttouch;
    if ((tmp2=strstr(cc,"linux-"))) {	/* cross compiling? */
      int len=strlen(platform);
      --tmp2;
      if (tmp2-cc>90) error("platform name too long!\n");
      shortplatform=platform+len;
      memmove(platform+len,argv[1],(size_t)(tmp2-cc));
      platform[tmp2-cc+len]=0;
      if (shortplatform[0]=='i' && shortplatform[2]=='8' && shortplatform[3]=='6') shortplatform[1]='3';
    } else {
#ifdef __sparc__
      strcat(platform,"sparc");
#endif
#ifdef __powerpc__
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
      shortplatform=platform;
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
      newargv=alloca(sizeof(char*)*(argc+20));
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
#ifdef WANT_SAFEGUARD
      if (compile) {
	*dest++=safeguard1;
	*dest++=safeguard2;
      }
#endif
      if (_link) { *dest++=b; }
#ifdef WANT_DYNAMIC
      if (_link) { *dest++=d; }
#endif
      for (i=2; i<argc; ++i) {
	if (mangleopts)
	  if (argv[i][0]=='-' && (argv[i][1]=='O' || argv[i][1]=='f' || argv[i][1]=='m'))
	    continue;
	*dest++=argv[i];
      }
      if (mangleopts) {
	const char **o=Os;
	for (o=Os;*o;) {
	  if (!strcmp(*o,shortplatform)) {
	    ++o;
	    while (*o) {
	      *dest++=(char*)*o;
	      ++o;
	    }
	  } else
	    while (*o) ++o;
	}
      }
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
