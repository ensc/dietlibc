#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
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

#if 0
int compint(const void *a,const void *b) {
/*  printf("comparing %d with %d\n",*(int*)a,*(int*)b); */
  return (*(int*)a-*(int*)b);
}

#define SIZE 100000
#define LOOKFOR ((SIZE/2)-2)

int array[SIZE];

#define rdtscl(low) \
     __asm__ __volatile__ ("rdtsc" : "=a" (low) : : "edx")

static unsigned int seed=1;

static int rand() {
  return ((seed = seed * 1103515245 + 12345) % ((unsigned int)RAND_MAX + 1));
}
#endif

extern double atof(const char *c);

int main(int argc,char *argv[]) {
  struct in_addr bar;
  struct hostent *foo;
/*  inet_aton("160.45.10.8",&bar); */
  foo=gethostbyname("zeit.fu-berlin.de");
/*  foo=gethostbyaddr(&bar,4,AF_INET); */
/*  if (foo)
    printf("%s -> %s\n",foo->h_name,inet_ntoa(*(struct in_addr*)foo->h_addr)); */
/*  printf("%g %g\n",1e-10,1e10); */
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
