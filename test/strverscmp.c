#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <assert.h>

int main() {
  int r;
  assert(strverscmp("foo","foo")==0);
  assert(strverscmp("foo","bar")>0);
  assert(strverscmp("bar","foo")<0);
  assert(strverscmp("test23","test123")<0);
}
