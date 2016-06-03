#ifndef _SYS_XATTR_H
#define _SYS_XATTR_H

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/* The following constants should be used for the fifth parameter of
   `*setxattr'.  */
enum
{
  XATTR_CREATE = 1,	/* set value, fail if attr already exists.  */
#define XATTR_CREATE	XATTR_CREATE
  XATTR_REPLACE = 2	/* set value, fail if attr does not exist.  */
#define XATTR_REPLACE	XATTR_REPLACE
};

/* Set the attribute NAME of the file pointed to by PATH to VALUE (which
   is SIZE bytes long).  Return 0 on success, -1 for errors.  */
int setxattr(const char *path, const char *name, const void *value, size_t size, int flags) __THROW;

/* Set the attribute NAME of the file pointed to by PATH to VALUE (which is
   SIZE bytes long), not following symlinks for the last pathname component.
   Return 0 on success, -1 for errors.  */
int lsetxattr (const char *path, const char *name, const void *value, size_t size, int flags) __THROW;

/* Set the attribute NAME of the file descriptor FD to VALUE (which is SIZE
   bytes long).  Return 0 on success, -1 for errors.  */
int fsetxattr (int fd, const char *name, const void *value, size_t size, int flags) __THROW;

/* Get the attribute NAME of the file pointed to by PATH to VALUE (which is
   SIZE bytes long).  Return 0 on success, -1 for errors.  */
ssize_t getxattr (const char *path, const char *name, void *value, size_t size) __THROW;

/* Get the attribute NAME of the file pointed to by PATH to VALUE (which is
   SIZE bytes long), not following symlinks for the last pathname component.
   Return 0 on success, -1 for errors.  */
ssize_t lgetxattr (const char *path, const char *name, void *value, size_t size) __THROW;

/* Get the attribute NAME of the file descriptor FD to VALUE (which is SIZE
   bytes long).  Return 0 on success, -1 for errors.  */
ssize_t fgetxattr (int fd, const char *name, void *value, size_t size) __THROW;

/* List attributes of the file pointed to by PATH into the user-supplied
   buffer LIST (which is SIZE bytes big).  Return 0 on success, -1 for
   errors.  */
ssize_t listxattr (const char *path, char *list, size_t size) __THROW;

/* List attributes of the file pointed to by PATH into the user-supplied
   buffer LIST (which is SIZE bytes big), not following symlinks for the
   last pathname component.  Return 0 on success, -1 for errors.  */
ssize_t llistxattr (const char *path, char *list, size_t size) __THROW;

/* List attributes of the file descriptor FD into the user-supplied buffer
   LIST (which is SIZE bytes big).  Return 0 on success, -1 for errors.  */
ssize_t flistxattr (int fd, char *list, size_t size) __THROW;

/* Remove the attribute NAME from the file pointed to by PATH.  Return 0
   on success, -1 for errors.  */
int removexattr (const char *path, const char *name) __THROW;

/* Remove the attribute NAME from the file pointed to by PATH, not
   following symlinks for the last pathname component.  Return 0 on
   success, -1 for errors.  */
int lremovexattr (const char *path, const char *name) __THROW;

/* Remove the attribute NAME from the file descriptor FD.  Return 0 on
   success, -1 for errors.  */
int fremovexattr (int fd, const char *name) __THROW;

__END_DECLS

#endif
