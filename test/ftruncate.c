#define _GNU_SOURCE
#define _XOPEN_SOURCE 600
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

#define CHECK(_sz)				\
  assert(ftruncate(fd, (_sz)) == 0);		\
  assert(fstat(fd, &st) == 0);			\
  assert(st.st_size == (_sz));			\

#define CHECK64(_sz)				\
  assert(ftruncate64(fd, (_sz)) == 0);		\
  assert(fstat64(fd, &st64) == 0);		\
  assert(st64.st_size == (_sz));		\

int main(void)
{
  char file[] = "/tmp/dietlibc-fadvise-test.XXXXXX";
  int fd;
  struct stat st;

  fd = mkstemp(file);
  unlink(file);

  assert(ftruncate(fd, 500000) == 0);
  assert(fstat(fd, &st) == 0);

  if (st.st_blocks > 1) {
    /* spare files not supported by filesystem :( */
    return EXIT_SUCCESS;
  }

  CHECK(1);
  CHECK(0x7fffffff);

#if __WORDSIZE == 32
  {
    struct stat64 st64;
    CHECK64(1);
    CHECK64(0x7fffffff);
    CHECK64(0x80000001ul);
    CHECK64(0x17fffffffull);
  }
#else
  CHECK(0x17fffffffull);
#endif

  return EXIT_SUCCESS;
}
