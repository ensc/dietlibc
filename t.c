#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <shadow.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mount.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <math.h>
#include <termios.h>
#include <netdb.h>
#include <sys/mman.h>
#include <ctype.h>
#include <mntent.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int foo;

int main(int argc,char *argv[]) {
  char type[64];
  int len;
  printf("%d\n",sscanf("\n","%63s%n",type,&len));
  printf("%s %d\n",type,len);
#if 0
  char buf[100];
  char ip[16];
  memset(ip,0,16);
  printf("%p %p\n",inet_ntop(AF_INET6,ip,buf,100),buf);
  puts(buf);
#endif
#if 0
  struct addrinfo *ai;
  struct addrinfo hints;
  char buf[16];
  printf("%d\n",inet_pton(AF_INET6,"hellhound",buf));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE|AI_CANONNAME;
  hints.ai_socktype = SOCK_STREAM;
  printf("%d\n",getaddrinfo(0,"80",&hints,&ai));
  while (ai) {
    printf("found host %s, port %d, family %s, socktype %s\n",ai->ai_canonname,
	   ntohs(ai->ai_family==AF_INET6?((struct sockaddr_in6*)ai->ai_addr)->sin6_port:
				   ((struct sockaddr_in*)ai->ai_addr)->sin_port),
	   ai->ai_family==AF_INET6?"PF_INET6":"PF_INET",
	   ai->ai_socktype==SOCK_STREAM?"SOCK_STREAM":"SOCK_DGRAM");
    {
      char buf[100];
      inet_ntop(ai->ai_family,ai->ai_family==AF_INET6?
		(char*)&(((struct sockaddr_in6*)ai->ai_addr)->sin6_addr):
		(char*)&(((struct sockaddr_in*)ai->ai_addr)->sin_addr),buf,100);
      printf("  %s\n",buf);
    }
    ai=ai->ai_next;
  }
#endif
#if 0
  int i=foo;
  printf("load average is %3.2f\n",0.0);
#endif
#if 0
  struct dirent **namelist;
  int n;

  n = scandir(".", &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    while(n--) {
      printf("%s\n", namelist[n]->d_name);
      free(namelist[n]);
    }
    free(namelist);
  }
#endif
#if 0
  char foo[10]="none,zlib";
  char *tmp,*tmp2=foo;
  while (tmp=strsep(&tmp2,",")) {
    puts(tmp);
  }
#endif
#if 0
  char foo[10];
  printf("%d %d\n",abs(-3),abs(23));
  strcpy(foo,"foo");
  strncat(foo,"barbaz",3);
  foo[6]=0;
  puts(foo);
#endif
#if 0
  struct hostent * host;
  struct in_addr i;

  host = gethostbyname("ftp.ciril.fr");

  if (!host)
    printf("host null\n");

  if (host && host->h_name) {
    printf("name %s\n", host->h_name);
  }
  if (host && (host->h_addr_list)[0]) {
    struct in_addr address;
    address = *((struct in_addr *) (host->h_addr_list)[0]);
    printf("addr %s\n", inet_ntoa(address));
  }
#endif
#if 0
  struct msgbuf bla;
  bla.mtype=0;
  bla.mtext[0]='x';
  msgsnd(327680,&bla,5,IPC_NOWAIT);
#endif
#if 0
  char buf[PATH_MAX];
  printf("%s\n",realpath("../../incoming/..///.zshrc",buf));
#endif
#if 0
  regex_t t;
  regcomp(&t,"^OpenSSH_2\\.5\\.[012]",5);
  printf("%d\n",regexec(&t,"OpenSSH_2.5.2p2",0,0,0));
#endif
#if 0
  float my_float = 9.2334;
  char buffer[100];

  sprintf(buffer, "%.2f", my_float);
  fprintf(stdout, "%s", buffer);
#endif
#if 0
  printf("%d\n",setenv("foo","bar",0));
  printf("%d\n",setenv("foo","bar",1));
  execlp("printenv","printenv","foo",0);
#endif
#if 0
  printf("%d\n",fnmatch("*c*","bin",0));
  if (!fnmatch("s*", "sub", 0))
    printf("s* sub\n");
  if (!fnmatch("s*", "glob", 0))
    printf("s* glob\n");
  if (!fnmatch("s*b", "sub", 0))
    printf("s*b sub\n");
  if (!fnmatch("s*h", "sub", 0))
    printf("s*h sub\n");
#endif
#if 0
  char*tmp;
  int n=asprintf(&tmp,"foo %s %d\n","bar",23);
  write(1,tmp,n);
  free(tmp);
#endif
#if 0
  struct passwd *p=getpwnam("leitner");
  struct spwd *s=getspnam("leitner");
  printf("%g\n",30.0123);
#endif
#if 0
  initgroups("root",100);
#endif
#if 0
  time_t t=time(0);
  puts(asctime(localtime(&t)));
#endif
#if 0
  struct servent *foo=getservbyname("ident","tcp");
  if (foo)
    printf("found service %s on port %d\n",foo->s_name,foo->s_port);
#endif
#if 0
  char buf[128];
  strcpy(buf,"/tmp/foo.XXXXXXX");
  printf("%d\n",mkstemp(buf));
  printf("%s\n",buf);
  unlink(buf);
#endif
#if 0
  char buf[512]="foo";
  strncat(buf,"barbaz",3);
  puts(buf);
#endif
#if 0
  time_t oink=time(0);
  struct tm *duh=localtime(&oink);
  strftime(buf,512,"%A %B %Y\n",duh);
  puts(buf);
#endif
#if 0
  struct in_addr bar;
  struct hostent *foo;
/*  inet_aton("160.45.10.8",&bar); */
  foo=gethostbyname("zeit.fu-berlin.de");
/*  foo=gethostbyaddr(&bar,4,AF_INET); */
  if (foo)
    printf("%s -> %s\n",foo->h_name,inet_ntoa(*(struct in_addr*)foo->h_addr));
/*  printf("%g %g\n",1e-10,1e10); */
#endif
#if 0
  double d=0.0;
  long long t=0x12345678ABCDEF01;
  d/=0.0;
  printf("%d %llx\n",__isnan(d),t,*(long long*)&d);
#endif
#if 0
  int i,j;
  long a,b,c;
  int *res;
  printf("%p\n",malloc(0));
  qsort(array,2,sizeof(int),compint);
  for (i=0; i<SIZE; ++i)
    array[i]=rand();
  rdtscl(a);
  qsort(array,SIZE,sizeof(int),compint);
  rdtscl(b);
  j=array[LOOKFOR];
  res=bsearch(&j,array,SIZE,sizeof(int),compint);
  rdtscl(c);
  printf("%lu cycles sort, %lu cycles bsearch\n",b-a,c-b);
  for (i=0; i<SIZE-1; ++i)
    if (array[i]>array[i+1]) {
      printf("qsort does not work, index %d: %d > %d\n",i,array[i],array[i+1]);
      return 1;
    }
  if (*res!=j)
    printf("besearch does not work, returned %p (%d) instead of %p (%d)\n",res,res?*res:-1,array+LOOKFOR,j);
/*  printf("array={%d,%d,%d,%d,%d}\n",array[0],array[1],array[2],array[3],array[4]); */
#endif
#if 0
  struct in_addr duh;
  printf("%d\n",inet_aton(argv[1]?argv[1]:"10.0.0.1",&duh));
  printf("%x\n",duh.s_addr);
#endif
/*  printf("%-19s %10lu %9lu %9lu %3d%% %s\n","/dev/ide/host0/bus0/target0/lun0/part2",8393688,705683,1337084,85,"/"); */
#if 0
  char buf[100];
  fgets(buf,100,stdin); printf("got %d bytes\n",strlen(buf));
  fgets(buf,100,stdin); printf("got %d bytes\n",strlen(buf));
#endif
#if 0
  struct tm duh;
  time_t t;
  time(&t);
  gmtime_r(&t,&duh);
  printf("%s\n",asctime(&duh));
#endif
#if 0
  char buf[30];
  duh.tm_sec=42;
  duh.tm_min=23;
  duh.tm_hour=17;
  duh.tm_mday=2;
  duh.tm_mon=7;
  duh.tm_year=100;
  t=mktime(&duh);
  printf("%s\n",asctime_r(&duh,buf));
#endif
#if 0
  int i;
  for (i=0; i<5; i++) {
    fprintf(stdout,"first message\n");
    fprintf(stdout,"second message\n");
    fprintf(stdout,"third message\n");
    printf("foo %d\n",i);
  }
#endif
#if 0
  char buf[1024];
  sscanf("foo bar","%s",buf);
  printf("%s\n",buf);
#endif
#if 0
  mount("/dev/scsi/host0/bus0/target2/lun0/cd", "/cd", "iso9660", MS_MGC_VAL|MS_RDONLY, NULL);
  perror("mount");
#endif
#if 0
  char *t="<4>Linux version 2.4.0-test10 (leitner@hellhound) (gcc version 2.95.2 19991024 (release))";
  int i=strtol(t+1,&t,10);
  printf("%d %s\n",i,t);
#endif
#if 0
  char **tmp;
  putenv("FOO");
  assert(1==2);
  for (tmp=environ; *tmp; tmp++)
    puts(*tmp);
#endif
#if 0
  char buf[1024];
  printf("%d\n",fprintf(stderr,"duh\n"));
#endif
#if 0
  struct passwd *p=getpwuid(100);
  puts(p->pw_name);
#endif
#if 0
  int pid;
  char name[32];
  sscanf("1 (init","%d (%15c",&pid,name);
  printf("pid %d name %s\n",pid,name);
#endif
#if 0
  DIR *d=opendir("/proc");
  if (d) {
    struct dirent *D;
    while (D=readdir(d))
      puts(D->d_name);
    closedir(d);
  }
#endif
#if 0
  char buf[1024];
  int fd=open("/etc/passwd",0);
  pread(fd,buf,30,32);
  close(fd);
  write(1,buf,32);
#endif
#if 0
  char *argv[] = {"echo","foo",0};
  char buf[100];
  buf[5]='x';
  sprintf(buf,"foo\n");
  if (buf[5] == 'x')
    exit(0);
  else
    exit(1);
  execvp(argv[0],argv);
#endif
#if 0
  struct stat64 f;
  char buf[128];
  fstat64(0,&f);
  fprintf(stderr,"%d %d\n",f.st_size,sizeof(f));
  return 0;
#endif
#if 0
  FILE *f=fopen("foo","w");
  fputc('a',f);
  fputc('b',f);
  fputc('c',f);
#endif
/*  fprintf(stdout,"foo\n"); */
}
