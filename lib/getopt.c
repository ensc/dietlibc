#include <getopt.h>
#include <unistd.h>
#include <string.h>

int opterr;
int optopt;

int optind=1;
char *optarg;

/* fsck, no GNU style permutation of argv this time. */

static void getopterror(void) {
  static char error[]="Unknown option `-x'.\n";
  if (opterr) {
    error[17]=optopt;
    write(2,error,22);
  }
}

int getopt(int argc,char*const argv[], const char* optstring) {
  static int lastidx=0,lastofs=0;
  char *tmp;
again:
  if (optind>argc || !argv[optind] || *argv[optind]!='-' || argv[optind][1]==0)
    return -1;
  if (argv[optind][1]=='-' && argv[optind][2]==0) {
    ++optind;
    return -1;
  }
  if (lastidx!=optind) {
    lastidx=optind; lastofs=0;
  }
  optopt=argv[optind][lastofs+1];
  if ((tmp=strchr(optstring,optopt))) {
    if (*tmp==0) {	/* apparently, we looked for \0, i.e. end of argument */
      ++optind;
      goto again;
    }
    if (tmp[1]==':') {	/* argument expected */
      if (argv[optind][lastofs+2]) {	/* "-foo", return "oo" as optarg */
	optarg=argv[optind]+lastofs+2;
	++optind;
	return optopt;
      }
      optarg=argv[optind+1];
      if (!optarg) {	/* missing argument */
	if (*optstring==':') return ':';
	getopterror();
	return '?';
      }
      optind+=2;
    } else ++lastofs;
    return optopt;
  } else {	/* not found */
    getopterror();
    return '?';
  }
}

