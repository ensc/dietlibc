#include <sys/types.h>

#define __THROW
#define __pure__ __attribute__ ((__pure__))

void* memset(void *s, int c, size_t n) __THROW;
int memcmp(const void *s1, const void *s2, size_t n) __THROW __pure__;
void* memcpy(void *dest, const void *src, size_t n) __THROW;
char *strncpy(char *dest, const char *src, size_t n) __THROW;
int strncmp(const char *s1, const char *s2, size_t n) __THROW __pure__;
char *strncat(char *dest, const char *src, size_t n) __THROW;

main() {
  /* this is just a test to see whether gcc still has broken internal
   * prototypes */
}
