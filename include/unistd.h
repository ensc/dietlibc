#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fsuid.h>

extern int optind,opterr;
extern char *optarg;
int getopt(int argc, char *const argv[], const char *options);

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK 4 /* Test for read permission.  */
#define W_OK 2 /* Test for write permission.  */
#define X_OK 1 /* Test for execute permission.  */
#define F_OK 0 /* Test for existence.  */

/* Test for access to NAME using the real UID and real GID.  */
extern int access (const char *__name, int __type) __THROW;

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#define STDIN_FILENO   0
#define STDOUT_FILENO  1
#define STDERR_FILENO  2

off_t lseek(int fildes, off_t offset, int whence) __THROW;
#ifndef __NO_STAT64
loff_t lseek64(int fildes, loff_t offset, int whence) __THROW;
#endif
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define lseek(fildes,offset,whence) lseek64(fildes,offset,whence)
#endif

int chdir(const char *path) __THROW;
int fchdir(int fd) __THROW;
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
int execl(const char *path, ...) __THROW;
int execle(const char *path, ...) __THROW;

pid_t getpid(void) __THROW __attribute__((__const__));
pid_t getppid(void) __THROW;

int setpgid (pid_t pid,pid_t pgid) __THROW;
pid_t getpgid (pid_t pid) __THROW;
int setpgrp (void) __THROW;
pid_t getpgrp (void) __THROW;
pid_t getsid(pid_t pid) __THROW;
pid_t setsid (void) __THROW;
int dup (int oldfd) __THROW;
int dup2 (int oldfd,int newfd) __THROW;

struct dirent;
struct dirent64;
int getdents(int fd, struct dirent *dirp, unsigned int count) __THROW;
int getdents64(int fd, struct dirent64 *dirp, unsigned int count) __THROW;

pid_t fork(void) __THROW;
pid_t vfork(void) __THROW;

int readlink(const char *path, char *buf, size_t bufsiz) __THROW;
int symlink(const char *oldpath, const char *newpath) __THROW;
int link(const char *oldpath, const char *newpath) __THROW;

int chown(const char *path, uid_t owner, gid_t group) __THROW;
int fchown(int fd, uid_t owner, gid_t group) __THROW;
int lchown(const char *path, uid_t owner, gid_t group) __THROW;

int fsync(int fd) __THROW;
#define _POSIX_SYNCHRONIZED_IO
int fdatasync(int fd) __THROW;

int pipe(int filedes[2]) __THROW;

char *ttyname (int desc) __THROW;

int brk(void *end_data_segment) __THROW;
void *sbrk(ptrdiff_t increment) __THROW;

int gethostname(char *name, size_t len) __THROW;
int sethostname(const char *name, size_t len) __THROW;

int usleep(unsigned long useconds) __THROW;
unsigned int sleep(unsigned int seconds) __THROW;

unsigned int alarm(unsigned int seconds) __THROW;
int sync(void) __THROW;

int isatty(int desc) __THROW;

void _exit(int status) __THROW __attribute__((noreturn));

extern int daemon(int nochdir,int noclose) __THROW;

int pause(void) __THROW;

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
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

int select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) __THROW;

int nice(int inc) __THROW;

extern char **__environ;

char *crypt(const char *key, const char *salt) __THROW;
void encrypt(char block[64], int edflag) __THROW;
void setkey(const char *key) __THROW;

size_t getpagesize(void) __THROW __attribute__((__const__));

int getdomainname(char *name, size_t len) __THROW;
int setdomainname(const char *name, size_t len) __THROW;

int getgroups(int size, gid_t list[]) __THROW;
int getdtablesize(void) __THROW;
char *getpass(const char * prompt) __THROW;

/* warning: linux specific: */
int llseek(int fildes, unsigned long hi, unsigned long lo, loff_t* result,int whence) __THROW;

/* include <linux/sysctl.h> to get all the definitions! */
struct __sysctl_args;
int _sysctl(struct __sysctl_args *args) __THROW;

#define _SC_CLK_TCK 1
#define _SC_ARG_MAX 2
#define _SC_OPEN_MAX 4
#define _SC_PAGESIZE 5
#define _SC_NPROCESSORS_ONLN 6
#define _SC_NPROCESSORS_CONF _SC_NPROCESSORS_ONLN
long sysconf(int name) __THROW;
#define _PC_PATH_MAX 1
#define _PC_VDISABLE 2

pid_t tcgetpgrp(int fd) __THROW;
int tcsetpgrp(int fd, pid_t pgrpid) __THROW;

int profil(unsigned short *buf, size_t bufsiz, size_t offset, unsigned int scale);

/* Linux only: */
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid) __THROW;
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid) __THROW;
int setresuid(uid_t ruid, uid_t euid, uid_t suid) __THROW;
int setresgid(gid_t rgid, gid_t egid, gid_t sgid) __THROW;

/* 32-bit uid support */
int chown32(const char *path, uid32_t owner, gid32_t group) __THROW;
int fchown32(int fd, uid32_t owner, gid32_t group) __THROW;
int lchown32(const char *path, uid32_t owner, gid32_t group) __THROW;
uid32_t getuid32(void) __THROW;
uid32_t geteuid32(void) __THROW;
gid32_t getgid32(void) __THROW;
gid32_t getegid32(void) __THROW;
int setuid32(uid32_t uid) __THROW;
int setgid32(gid32_t gid) __THROW;
int setreuid32(uid32_t ruid, uid32_t euid) __THROW;
int setregid32(gid32_t rgid, gid32_t egid) __THROW;
#define seteuid32(euid) setreuid32(-1,euid)
#define setegid32(egid) setregid32(-1,egid)
int getgroups32(int size, gid32_t list[]) __THROW;
int getresuid32(uid32_t *ruid, uid32_t *euid, uid32_t *suid);
int getresgid32(gid32_t *rgid, gid32_t *egid, gid32_t *sgid);
int setresuid32(uid32_t ruid, uid32_t euid, uid32_t suid) __THROW;
int setresgid32(gid32_t rgid, gid32_t egid, gid32_t sgid) __THROW;

#ifdef _BSD_SOURCE
char *getusershell(void);
void setusershell(void);
void endusershell(void);
#endif

#define   _POSIX_VERSION  199506L

#endif
