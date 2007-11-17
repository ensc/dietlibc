#include <assert.h>
#include <string.h>

int main() {
  const char* x="foo bar baz";
  size_t i;
  assert(strlen("")==0);
  for (i=0; i<11; ++i)
    assert(strlen(x+i)==11-i);
  return 0;
}
