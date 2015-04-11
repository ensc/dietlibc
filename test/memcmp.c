#include <string.h>
#include <assert.h>

int main() {
  const char* test="blubber";
  assert(memcmp(test,"blubber",8)==0);
  assert(memcmp(test,"fnord",5)<0);
  assert(memcmp(test,"armenien",8)>0);
  /* now check if the endianness conversion works right */
  assert(memcmp("bbbacbbb","bbbbbbbb",8)<0);
  assert(memcmp(test,0,0)==0);
  return 0;
}
