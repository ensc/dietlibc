#include <wchar.h>
#include <assert.h>

int main() {
  wchar_t buf[100];
  int i;

  /* does wmemset work? */
  assert(wmemset(buf,0,100)==buf);
  for (i=0; i<100; ++i) assert(buf[i]==0);

  /* do wcscpy and wcscat work? */
  assert(wcscpy(buf,L"fnord")==buf);
  assert(wcscat(buf,L"blah")==buf);
  assert(wcslen(buf)==9);
  assert(wcscmp(buf,L"fnordblah")==0);

  /* does wmemcmp work? */
  assert(wmemcmp(buf,L"fnordxxx",5)==0);
  assert(wmemcmp(buf,L"fnordxxx",6)<0);

  /* check wmemcpy */
  assert(wmemcpy(buf+5,buf,5)==buf+5);
  assert(wmemcmp(buf,L"fnordfnord",10)==0);

  /* does wmemmove handle overlapping properly */
  assert(wmemmove(buf+1,buf,3)==buf+1);
  assert(wmemcmp(buf,L"ffnod",5)==0);
  assert(wmemmove(buf,buf+1,3)==buf);
  assert(wmemcmp(buf,L"fnood",5)==0);
}
