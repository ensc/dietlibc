#include <string.h>
#include <unistd.h>
#include <stdio.h>

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
  int link=0;
  int compile=0;
  char diethome[]=DIETHOME;
  char platform[1000]=DIETHOME "/bin-";
  char *nostdlib="-nostdlib";
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
    if (!strcmp(tmp,"cc")) {
      char **newargv;
      char **dest;
      char *a,*b,*c;
/* we need to add -I... if the command line contains -c, -S or -E */
      for (i=2; i<argc; ++i)
	if (!strcmp(argv[i],"-c") || !strcmp(argv[i],"-S") || !strcmp(argv[i],"-E"))
	  compile=1;
/* we need to add -nostdlib if the command line contains -o and we are not compiling*/
      for (i=2; i<argc; ++i)
	if (!strcmp(argv[i],"-o"))
	  if (!compile) link=1;
      newargv=alloca(sizeof(char*)*(argc+3));
      a=alloca(strlen(diethome)+20);
      b=alloca(strlen(platform)+20);
      c=alloca(strlen(platform)+20);

      strcpy(a,"-I"); strcat(a,diethome); strcat(a,"/include");
      strcpy(b,platform); strcat(b,"/start.o");
      strcpy(c,platform); strcat(c,"/dietlibc.a");

      dest=newargv;
      *dest++=argv[1];
      if (link) *dest++=nostdlib;
      if (compile || link) *dest++=a;
      for (i=2; i<argc; ++i)
	*dest++=argv[i];
      if (link) { *dest++=b; *dest++=c; }
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
