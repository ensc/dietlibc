#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int __sc_nr_cpus() {
  int fd;
  fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd==-1) return 1;
  else {
    char buf[2048]; /* holds ~6 cpuinfos */
    register char pch=0;
    int n,nr=0;
    while((n=read(fd,buf,sizeof(buf)))>0) {
      register int i;
      for (i=0;i<n;i++) {
	if ((buf[i]=='\n')&&(pch=='\n')) ++nr;
	pch=buf[i];
      }
    }
    close(fd);
    return nr;
  }
}
