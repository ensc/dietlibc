#include <dlfcn.h>
#include "_dl_int.h"

#define MAX_QUEUE 64

static int _dl_start=0;
static int _dl_stop=0;

static struct {
  const char* name;
  int flags;
} _dl_queue[MAX_QUEUE];

int _dl_queue_lib(const char* name, int flags) {
  if (_dl_find_lib(name)==0) {
    register int tmp;
    if ((tmp=_dl_stop+1)>=MAX_QUEUE) tmp=0;
    if (tmp==_dl_start) return -1;
    _dl_queue[_dl_stop].name=name;
    _dl_queue[_dl_stop].flags=flags;
    _dl_stop=tmp;
  }
  return 0;
}

int _dl_open_dep() {
  while (_dl_start!=_dl_stop) {
    register int tmp=_dl_start;
    (++_dl_start>=MAX_QUEUE)?_dl_start=0:0;
    if (!dlopen(_dl_queue[tmp].name,_dl_queue[tmp].flags)) return 1;
  }
  return 0;
}
