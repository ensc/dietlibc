#include <string.h>

int timingsafe_memcmp(const void* a,const void* b,size_t n) {
  /* slightly harder than bcmp because we want to know which one was
   * smaller, not just if they did not match */
  register const unsigned char *x=a;
  register const unsigned char *y=b;
  int r=0,done=0;
  /* r is what we return in the end,
   * done is there to be 0 or -1 so we can assign to r branchlessly */
  size_t i;
  for (i=0; i<n; ++i) {
    int t=x[i]-y[i];
    /* if t is nonzero, t|-t will have the sign bit set */
    int minusone = (t|-t)>>(sizeof(int)-1);
    /* if there is no mismatch, t is 0. Then r |= t is a no-op. */
    /* if this is the first mismatch, we want to do r |= t. */
    /* if this is not the first mismatch, we want to leave r alone. */
    r |= t & ~done;
    done |= minusone;
  }
  return r;
}
