#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>

#define TMPDIR	"/tmp"

#define TEST(_op)	do {						\
		int rc = (_op);						\
		if (rc < 0) {						\
			printf("%s:%u: %s: %u/%s\n", __FILE__, __LINE__,	\
			       # _op, errno, strerror(errno));		\
			abort();					\
		}							\
	} while (0)

#define memeq(_a, _b) \
	((memcmp(_a, _b, sizeof *(_a)) == 0 ? 0 : -1))

#define memne(_a, _b) \
	((memcmp(_a, _b, sizeof *(_a)) == 0 ? -1 : 0))

int main(void)
{
	char dname_buf[] = "/tmp/test-mmap.XXXXXX";
	char *dname;
	int dir_fd;
	int file_fd;
	struct stat st[5];

	/* check whether *at(2) syscalls are available */
	dir_fd = openat(AT_FDCWD, "/", O_DIRECTORY|O_RDONLY);
	if (dir_fd == -1 && errno == ENOSYS)
		return EXIT_SUCCESS;	/* kernel too old */

	assert(dir_fd != -1);
	close(dir_fd);

	/* create a tree like
	 * /
	 * +- tmp/
	 *    +- <tmpname>/
	 *       |- test/
	 *       |- some-file
	 *       +- some-link -> some-file
	 */
	dname = mkdtemp(dname_buf);
	assert(dname != NULL);

	dir_fd = open(dname, O_DIRECTORY|O_RDONLY);
	assert(dir_fd != -1);

	TEST(mkdirat(dir_fd, "test", 0700));
	TEST(chdir(dname));
	TEST(chdir("test"));

	file_fd = openat(dir_fd, "some-file", O_WRONLY|O_CREAT, 0400);
	assert(file_fd != -1);

	write(file_fd, "some text\n", 10);
	TEST(close(file_fd));

	TEST(symlinkat("some-file", dir_fd, "some-link"));
	TEST(symlinkat("dangling", dir_fd, "dangling-link"));

	/* now check, whether attributes of 'some-file' and 'some-link'
	 * returned by stat(2), lstat(2) and fstatat(2) are consistent */
	TEST(stat("../some-file", &st[0]));
	TEST(lstat("../some-link", &st[1]));

	TEST(fstatat(dir_fd, "some-file", &st[2], 0));
	TEST(fstatat(dir_fd, "some-link", &st[3], AT_SYMLINK_NOFOLLOW));
	TEST(fstatat(dir_fd, "some-link", &st[4], 0));

	TEST(faccessat(dir_fd, "some-file", R_OK, 0));
	TEST((faccessat(dir_fd, "some-file", W_OK, 0) == -1 &&
	      errno == EACCES) ? 0 : -1);;

	if (1)
		fputs("skipping faccessat(..., AT_SYMLINK_NOFOLLOW) checks for now...\n",
		      stderr);
	else {
		/* this is broken for dietlibc; the 'flags' parameter is not checked
		 * by the kernel but must be handled by the libc itself */
		TEST(faccessat(dir_fd, "some-link", W_OK, AT_SYMLINK_NOFOLLOW));
		TEST(faccessat(dir_fd, "dangling-link", R_OK, AT_SYMLINK_NOFOLLOW));
	}

	assert(st[0].st_mode == (0400 | S_IFREG));
	assert(S_ISLNK(st[1].st_mode));

	TEST(memne(&st[0], &st[1]));
	TEST(memeq(&st[0], &st[2]));
	TEST(memeq(&st[0], &st[4]));
	TEST(memeq(&st[1], &st[3]));

	/* and cleanup the mess... */
	TEST(unlinkat(dir_fd, "some-link", 0));
	TEST(unlinkat(dir_fd, "some-file", 0));
	TEST(unlinkat(dir_fd, "dangling-link", 0));
	TEST(unlinkat(dir_fd, "test", AT_REMOVEDIR));
	TEST(rmdir(dname));

	return EXIT_SUCCESS;
}
