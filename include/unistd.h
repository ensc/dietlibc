#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/cdefs.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/time.h>
#include <getopt.h>
#include <linux/sysctl.h>

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK 4 /* Test for read permission.  */
#define W_OK 2 /* Test for write permission.  */
#define X_OK 1 /* Test for execute permission.  */
#define F_OK 0 /* Test for existence.  */

/* Test for access to NAME using the real UID and real GID.  */
extern int access (__const char *__name, int __type) __THROW;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN_FILENO   0
#define STDOUT_FILENO  1
#define STDERR_FILENO  2

off_t lseek(int fildes, off_t offset, int whence) __THROW;

int chdir(const char *path) __THROW;
int fchdir(int fd) __THROW;
int mkdir(const char *pathname, mode_t mode) __THROW;
int rmdir(const char *pathname) __THROW;
char *getcwd(char *buf, size_t size) __THROW;

int open(const char* pathname,int flags, ...) __THROW;
int open64(const char* pathname,int flags, ...) __THROW;
int creat(const char* pathname,mode_t mode) __THROW;
int creat64(const char* pathname,mode_t mode) __THROW;
int write(int fd,const void* buf,size_t len) __THROW;
int read(int fd,void* buf,size_t len) __THROW;
int close(int fd) __THROW;

int unlink(const char *pathname) __THROW;

int pread(int fd, void *buf, size_t count, off_t offset);
int pwrite(int fd, const void *buf, size_t count, off_t offset);

int execve(const char *filename, char *const argv [], char *const envp[]) __THROW;
int execlp(const char *file, const char *arg, ...) __THROW;
int execv(const char *path, char *const argv[]) __THROW;
int execvp(const char *file, char *const argv[]) __THROW;
int execl(const char *path, const char *arg, ...) __THROW;

pid_t getpid(void) __attribute__((__const__)) __THROW;
pid_t getppid(void) __THROW;
int kill(pid_t pid, int sig) __THROW;

int setpgid (pid_t pid,pid_t pgid) __THROW;
pid_t getpgid (pid_t pid) __THROW;
int setpgrp (void) __THROW;
pid_t getpgrp (void) __THROW;
pid_t setsid (void) __THROW;
int dup (int oldfd) __THROW;
int dup2 (int oldfd,int newfd) __THROW;

int mknod(const char *pathname, mode_t mode, dev_t dev) __THROW;

int getdents(unsigned int fd, struct dirent *dirp, unsigned int count) __THROW;
int getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count) __THROW;

pid_t fork(void) __THROW;
pid_t vfork(void) __THROW;

int readlink(const char *path, char *buf, size_t bufsiz) __THROW;
int symlink(const char *oldpath, const char *newpath) __THROW;
int link(const char *oldpath, const char *newpath) __THROW;

int chown(const char *path, uid_t owner, gid_t group) __THROW;
int fchown(int fd, uid_t owner, gid_t group) __THROW;
int lchown(const char *path, uid_t owner, gid_t group) __THROW;

int fsync(int fd) __THROW;

int pipe(int filedes[2]) __THROW;

char *ttyname (int desc) __THROW;

int brk(void *end_data_segment) __THROW;
void *sbrk(ptrdiff_t increment) __THROW;

int gethostname(char *name, size_t len) __THROW;
int sethostname(const char *name, size_t len) __THROW;

void usleep(unsigned long usec) __THROW;
unsigned int sleep(unsigned int seconds) __THROW;

unsigned int alarm(unsigned int seconds) __THROW;
int sync(void) __THROW;

int isatty(int desc) __THROW;

void _exit(int status) __THROW __attribute__((noreturn));

extern int daemon (int nochdir,int noclose) __THROW;

#if _FILE_OFFSET_BITS == 64
#define open open64
#define creat creat64
#endif

extern char* getlogin(void) __THROW;
/* warning: the diet libc getlogin() simply returns getenv("LOGNAME") */

int chroot(const char *path) __THROW;

uid_t getuid(void) __THROW;
uid_t geteuid(void) __THROW;
gid_t getgid(void) __THROW;
gid_t getegid(void) __THROW;
int setuid(uid_t uid) __THROW;
/* int seteuid(uid_t uid) __THROW; */
int setgid(gid_t gid) __THROW;
/* int setegid(gid_t gid) __THROW; */
int setregid(gid_t rgid, gid_t egid) __THROW;
int setreuid(uid_t ruid, uid_t euid) __THROW;
#define seteuid(euid) setreuid(-1,euid)
#define setegid(egid) setregid(-1,egid)

int rename(const char *oldpath, const char *newpath) __THROW;
int ftruncate(int fd, off_t length) __THROW;

int mkstemp(char *Template) __THROW;

int select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) __THROW;

int nice(int inc) __THROW;

extern char *__environ[];

char *crypt(const char *key, const char *salt) __THROW;
void encrypt(char block[64], int edflag) __THROW;
void setkey(const char *key) __THROW;

size_t getpagesize(void) __attribute__((__const__)) __THROW;

int getdomainname(char *name, size_t len) __THROW;
int setdomainname(const char *name, size_t len) __THROW;

int getgroups(int size, gid_t list[]) __THROW;

/* warning: linux specific: */
int _sysctl(struct __sysctl_args *args) __THROW;
int  sendfile(int out_fd, int in_fd, off_t *offset, size_t count) __THROW;

#endif
