#include "dietwarning.h"

double difftime(time_t time1, time_t time0) {
  return (double)(time1>time0?time1-time0:time0-time1);
}

link_warning("difftime","introduces an unnecessary floating point dependency. Don't use it!")
