#ifndef _UNISTD_H
#define _UNISTD_H

#include <dirent.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <getopt.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

off_t lseek(int fildes, off_t offset, int whence) __THROW;

int chdir(const char *path) __THROW;
int fchdir(int fd) __THROW;
int mkdir(const char *pathname, mode_t mode) __THROW;
int rmdir(const char *pathname) __THROW;
char *getcwd(char *buf, size_t size) __THROW;

int open(const char* pathname,int flags, ...) __THROW;
int creat(const char* pathname,mode_t mode) __THROW;
int write(int fd,const void* buf,size_t len) __THROW;
int read(int fd,const void* buf,size_t len) __THROW;
int close(int fd) __THROW;

int pread(int fd, void *buf, size_t count, off_t offset);
int pwrite(int fd, const void *buf, size_t count, off_t offset);

int execve (const char *filename, char *const argv [], char *const envp[]) __THROW;

pid_t getpid(void) __attribute__((const)) __THROW;
pid_t getppid(void) __THROW;
int kill(pid_t pid, int sig) __THROW;

int mknod(const char *pathname, mode_t mode, dev_t dev) __THROW;

int getdents(unsigned int fd, struct dirent *dirp, unsigned int count) __THROW;
int getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count) __THROW;

pid_t fork() __THROW;
pid_t vfork() __THROW;

int readlink(const char *path, char *buf, size_t bufsiz) __THROW;
int symlink(const char *oldpath, const char *newpath) __THROW;
int link(const char *oldpath, const char *newpath) __THROW;

int chown(const char *path, uid_t owner, gid_t group) __THROW;
int fchown(int fd, uid_t owner, gid_t group) __THROW;
int lchown(const char *path, uid_t owner, gid_t group) __THROW;

char *ttyname (int desc) __THROW;

int brk(void *end_data_segment) __THROW;
void *sbrk(ptrdiff_t increment) __THROW;

int gethostname(char *name, size_t len) __THROW;

#endif
