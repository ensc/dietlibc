#include <string.h>
#include <getopt.h>

static void getopterror(void) {
  static char error[]="Unknown option `-x'.\n";
  if (opterr) {
    error[17]=optopt;
    write(2,error,22);
  }
}

int getopt_long(int argc, char * const argv[], const char *optstring,
		const struct option *longopts, int *longindex) {
  static int lastidx=0,lastofs=0;
  char *tmp;
again:
  if (optind>argc || !argv[optind] || *argv[optind]!='-' || argv[optind][1]==0)
    return -1;
  if (argv[optind][1]=='-' && argv[optind][2]==0) {
    ++optind;
    return -1;
  }
  if (argv[optind][1]=='-') {	/* long option */
    char* arg=argv[optind]+2;
    char* max=strchr(arg,'=');
    const struct option* o;
    if (!max) max=arg+strlen(arg);
    for (o=longopts; o->name; ++o) {
      if (!strncmp(o->name,arg,max-arg)) {	/* match */
	if (longindex) *longindex=o-longopts;
	if (o->has_arg>0) {
	  if (*max=='=')
	    optarg=max+1;
	  else {
	    optarg=argv[optind+1];
	    if (!optarg && o->has_arg==1) {	/* no argument there */
	      if (*optstring==':') return ':';
	      write(2,"argument required: `",20);
	      write(2,arg,max-arg);
	      write(2,"'.\n",3);
	      ++optind;
	      return '?';
	    }
	    ++optind;
	  }
	  ++optind;
	}
	if (!o->flag) return o->val;
	return 0;
      }
    }
    if (*optstring==':') return ':';
    write(2,"invalid option `",16);
    write(2,arg,max-arg);
    write(2,"'.\n",3);
    return '?';
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
