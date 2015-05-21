#include <string.h>
#include <assert.h>

char buf1[1024];
char buf2[1024];

int main() {
  unsigned int i;
  for (i=0; i<1024; ++i) {
    buf1[i]=i;
    buf2[i]=0;
  }
  for (i=0; i<100; ++i) {
    unsigned int j;
    assert(memcpy(buf2+i,buf1,10)==buf2+i);
    for (j=0; j<i; ++j)
      assert(buf2[i]==buf1[0]);
    assert(!memcmp(buf2+i,buf1,10));
  }
  return 0;
}
