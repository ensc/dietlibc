#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/cdefs.h>
#include <sys/types.h>

#if defined(__i386__)
struct stat {
	unsigned short st_dev;
	unsigned short __pad1;
	unsigned long st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned short __pad2;
	unsigned long  st_size;
	unsigned long  st_blksize;
	unsigned long  st_blocks;
	  signed long  st_atime;
	unsigned long  __unused1;
	  signed long  st_mtime;
	unsigned long  __unused2;
	  signed long  st_ctime;
	unsigned long  __unused3;
	unsigned long  __unused4;
	unsigned long  __unused5;
};

struct stat64 {
	unsigned short	st_dev;
	unsigned char	__pad0[10];

#define STAT64_HAS_BROKEN_ST_INO	1
	unsigned long	__st_ino;

	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	unsigned short	st_rdev;
	unsigned char	__pad3[10];

__extension__	long long	st_size;
	unsigned long	st_blksize;

	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */
	unsigned long	__pad4;		/* future possible st_blocks high bits */

	unsigned long	st_atime;
	unsigned long	__pad5;

	  signed long	st_mtime;
	unsigned long	__pad6;

	unsigned long	st_ctime;
	unsigned long	__pad7;		/* will be high 32 bits of ctime someday */

__extension__	unsigned long long	st_ino;
};
#elif defined(__sparc__)
struct stat {
	unsigned short	st_dev;
	unsigned long	st_ino;
	unsigned short	st_mode;
	short		st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	unsigned short	st_rdev;
	long		st_size;
	long		st_atime;
	unsigned long	__unused1;
	long		st_mtime;
	unsigned long	__unused2;
	long		st_ctime;
	unsigned long	__unused3;
	long		st_blksize;
	long		st_blocks;
	unsigned long	__unused4[2];
};

struct stat64 {
	unsigned char	__pad0[6];
	unsigned short	st_dev;

__extension__	unsigned long long	st_ino;

	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned int	st_uid;
	unsigned int	st_gid;

	unsigned char	__pad2[6];
	unsigned short	st_rdev;

	unsigned char	__pad3[8];

__extension__	long long	st_size;
	unsigned int	st_blksize;

	unsigned char	__pad4[8];
	unsigned int	st_blocks;

	  signed int	st_atime;
	unsigned int	__unused1;

	  signed int	st_mtime;
	unsigned int	__unused2;

	  signed int	st_ctime;
	unsigned int	__unused3;

	unsigned int	__unused4;
	unsigned int	__unused5;
};
#elif defined(__alpha__)
struct stat {
	unsigned int	st_dev;
	unsigned int	st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	st_rdev;
	long		st_size;
	  signed long	st_atime;
	  signed long	st_mtime;
	  signed long	st_ctime;
	unsigned int	st_blksize;
	int		st_blocks;
	unsigned int	st_flags;
	unsigned int	st_gen;
};
#elif defined(__mips__)
struct stat {
	dev_t		st_dev;
	long		st_pad1[3];		/* Reserved for network id */
	ino_t		st_ino;
	mode_t		st_mode;
	nlink_t		st_nlink;
	uid_t		st_uid;
	gid_t		st_gid;
	dev_t		st_rdev;
	long		st_pad2[2];
	off_t		st_size;
	long		st_pad3;
	/*
	 * Actually this should be timestruc_t st_atime, st_mtime and st_ctime
	 * but we don't have it under Linux.
	 */
	time_t		st_atime;
	long		reserved0;
	time_t		st_mtime;
	long		reserved1;
	time_t		st_ctime;
	long		reserved2;
	long		st_blksize;
	long		st_blocks;
	char		st_fstype[16];	/* Filesystem type name */
	long		st_pad4[8];
	/* Linux specific fields */
	unsigned int	st_flags;
	unsigned int	st_gen;
};

struct stat64 {
	unsigned long	st_dev;
	unsigned long	st_pad0[3];	/* Reserved for st_dev expansion  */
	ino_t		st_ino;
	mode_t		st_mode;
	nlink_t		st_nlink;
	uid_t		st_uid;
	gid_t		st_gid;
	unsigned long	st_rdev;
	unsigned long	st_pad1[3];	/* Reserved for st_rdev expansion  */
__extension__	long long	st_size;
	/*
	 * Actually this should be timestruc_t st_atime, st_mtime and st_ctime
	 * but we don't have it under Linux.
	 */
	time_t		st_atime;
	unsigned long	reserved0;	/* Reserved for st_atime expansion  */
	time_t		st_mtime;
	unsigned long	reserved1;	/* Reserved for st_atime expansion  */
	time_t		st_ctime;
	unsigned long	reserved2;	/* Reserved for st_atime expansion  */
	unsigned long	st_blksize;
__extension__	long long	st_blocks;
};
#elif defined(__powerpc__)
struct stat {
	dev_t		st_dev;
	ino_t		st_ino;
	mode_t		st_mode;
	nlink_t		st_nlink;
	uid_t 		st_uid;
	gid_t 		st_gid;
	dev_t		st_rdev;
	off_t		st_size;
	unsigned long  	st_blksize;
	unsigned long  	st_blocks;
	  signed long  	st_atime;
	unsigned long  	__unused1;
	  signed long  	st_mtime;
	unsigned long  	__unused2;
	  signed long  	st_ctime;
	unsigned long  	__unused3;
	unsigned long  	__unused4;
	unsigned long  	__unused5;
};

/* This matches struct stat64 in glibc2.1.
 */
struct stat64 {
__extension__	unsigned long long st_dev; 	/* Device.  */
__extension__	unsigned long long st_ino;	/* File serial number.  */
	unsigned int st_mode;		/* File mode.  */
	unsigned int st_nlink;		/* Link count.  */
	unsigned int st_uid;		/* User ID of the file's owner.  */
	unsigned int st_gid;		/* Group ID of the file's group. */
__extension__	unsigned long long st_rdev; 	/* Device number, if device.  */
	unsigned short int __pad2;
__extension__	long long st_size;		/* Size of file, in bytes.  */
	long st_blksize;		/* Optimal block size for I/O.  */

__extension__	long long st_blocks;		/* Number 512-byte blocks allocated. */
	long st_atime;			/* Time of last access.  */
	unsigned long int __unused1;
	long st_mtime;			/* Time of last modification.  */
	unsigned long int __unused2;
	long st_ctime;			/* Time of last status change.  */
	unsigned long int __unused3;
	unsigned long int __unused4;
	unsigned long int __unused5;
};
#elif defined(__arm__)
struct stat {
	unsigned short st_dev;
	unsigned short __pad1;
	unsigned long st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned short __pad2;
	unsigned long  st_size;
	unsigned long  st_blksize;
	unsigned long  st_blocks;
	  signed long  st_atime;
	unsigned long  __unused1;
	  signed long  st_mtime;
	unsigned long  __unused2;
	  signed long  st_ctime;
	unsigned long  __unused3;
	unsigned long  __unused4;
	unsigned long  __unused5;
};

/* This matches struct stat64 in glibc2.1, hence the absolutely
 * insane amounts of padding around dev_t's.
 */
struct stat64 {
	unsigned short	st_dev;
	unsigned char	__pad0[10];

#define STAT64_HAS_BROKEN_ST_INO	1
	unsigned long	__st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	unsigned short	st_rdev;
	unsigned char	__pad3[10];

__extension__	long long	st_size;
	unsigned long	st_blksize;

	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */
	unsigned long	__pad4;		/* future possible st_blocks high bits */

	  signed long	st_atime;
	unsigned long	__pad5;

	  signed long	st_mtime;
	unsigned long	__pad6;

	  signed long	st_ctime;
	unsigned long	__pad7;		/* will be high 32 bits of ctime someday */

__extension__	unsigned long long	st_ino;
};
#elif defined(__s390__)
struct stat {
	unsigned short st_dev;
	unsigned short __pad1;
	unsigned long  st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned short __pad2;
	unsigned long  st_size;
	unsigned long  st_blksize;
	unsigned long  st_blocks;
	unsigned long  st_atime;
	unsigned long  __unused1;
	unsigned long  st_mtime;
	unsigned long  __unused2;
	unsigned long  st_ctime;
	unsigned long  __unused3;
	unsigned long  __unused4;
	unsigned long  __unused5;
};
struct stat64 {
	unsigned char	__pad0[6];
	unsigned short	st_dev;
	unsigned int	__pad1;
#define STAT64_HAS_BROKEN_ST_INO        1
	unsigned long	__st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned long	st_uid;
	unsigned long	st_gid;
	unsigned char	__pad2[6];
	unsigned short	st_rdev;
	unsigned int	__pad3;
__extension__	long long	st_size;
	unsigned long	st_blksize;
	unsigned char	__pad4[4];
	unsigned long	__pad5; 	/* future possible st_blocks high bits */
	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */
	unsigned long	st_atime;
	unsigned long	__pad6;
	unsigned long	st_mtime;
	unsigned long	__pad7;
	unsigned long	st_ctime;
	unsigned long	__pad8; 	/* will be high 32 bits of ctime someday */
__extension__	unsigned long long	st_ino;
};

#elif defined(__hppa__)

struct stat {
	unsigned long long st_dev;
	unsigned short int _pad1;
	unsigned long st_ino;
	unsigned long st_mode;
	unsigned long st_nlink;
	unsigned long st_uid;
	unsigned long st_gid;
	unsigned long long st_rdev;
	unsigned short int _pad2;
	unsigned long st_size;
	unsigned long st_blksize;
	unsigned long st_blocks;
	unsigned long st_atime;
	unsigned long int unused1;
	unsigned long st_mtime;
	unsigned long int unused2;
	unsigned long st_ctime;
	unsigned long int unused3;
	unsigned long long unused4;
};

struct stat64 {
	unsigned long long st_dev;
	unsigned int __pad1;
	unsigned long __st_ino;
	unsigned long st_mode;
	unsigned long st_nlink;
	unsigned long st_uid;
	unsigned long st_gid;
	unsigned long long st_rdev;
	unsigned int __pad2;
	unsigned long long st_size;
	unsigned long long st_blksize;

	unsigned long long st_blocks;
 	unsigned long st_atime;
	unsigned long int __unused1;
	unsigned long st_mtime;
	unsigned long int __unused2;
	unsigned long st_ctime;
	unsigned long int __unused3;
	unsigned long long st_ino;
};

#endif

#define S_IFMT  00170000
#define S_IFSOCK 0140000
#define S_IFLNK	 0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

#ifndef STAT64_HAS_BROKEN_ST_INO
#define __NO_STAT64
#endif

extern int stat(const char *__file, struct stat *__buf) __THROW;
extern int fstat(int __fd, struct stat *__buf) __THROW;
extern int lstat(const char *__file, struct stat *__buf) __THROW;

#ifdef __NO_STAT64
#undef _FILE_OFFSET_BITS
#else
extern int stat64(const char *__file, struct stat64 *__buf) __THROW;
extern int fstat64(int __fd, struct stat64 *__buf) __THROW;
extern int lstat64(const char *__file, struct stat64 *__buf) __THROW;
#endif

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define lstat lstat64
#define fstat fstat64
#define stat stat64
#define pread pread64
#define pwrite pwrite64
#endif

#define major(dev) (((dev)>>8) & 0xff)
#define minor(dev) ((dev) & 0xff)
#define makedev(major, minor) ((((unsigned int) (major)) << 8) | ((unsigned int) (minor)))

extern int chmod (const char *__file, mode_t __mode) __THROW;
extern int fchmod (int __fd, mode_t __mode) __THROW;
extern mode_t umask (mode_t __mask) __THROW;
extern int mkdir (const char *__path, mode_t __mode) __THROW;
extern int mknod (const char *__path, mode_t __mode, dev_t __dev) __THROW;
extern int mkfifo (const char *__path, mode_t __mode) __THROW;

#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC S_IXUSR

#endif
