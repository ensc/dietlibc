#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int fd=open("/tmp/foo",O_CREAT|O_WRONLY|O_EXCL,0666);
  struct timeval tv[2];
  struct stat s;
  if (fd==-1) {
    perror("open");
    return -1;
  }
  lseek(fd,0x100000000,SEEK_SET);
  write(fd,"x",1);
  close(fd);
  tv[0].tv_sec=123456789;
  tv[0].tv_usec=0;
  tv[1].tv_sec=123456789;
  tv[1].tv_usec=0;
  if (utimes("/tmp/foo",tv)) {
    perror("utimes");
    unlink("/tmp/foo");
    return -1;
  }
  if (stat("/tmp/foo",&s)) {
    perror("stat");
    unlink("/tmp/foo");
    return -1;
  }
  printf("size %lld, atime %ld, mtime %ld, inode %ld\n",(long long)s.st_size,(long)s.st_atime,(long)s.st_mtime,(long)s.st_ino);
  unlink("/tmp/foo");
}
