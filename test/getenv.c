#include <stdlib.h>
#include <assert.h>

main() {
  assert(getenv("PATH"));
  putenv("foo=bar");
  assert(!strcmp(getenv("foo"),"bar"));
}
