#include <string.h>

int timingsafe_bcmp(const void* a,const void* b,size_t n) {
  size_t i;
  const unsigned char* x = a;
  const unsigned char* y = b;
  int r;
  for (i=r=0; i<n; ++i)
    r |= x[i] ^ y[i];	/* x[i]^y[i] is 0 iff x[i]==y[i] */
  return !!r;	/* turn nonzero into 1 */
}
