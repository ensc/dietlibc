#include <unistd.h>

extern int __diet_brk(void *end_data_segment);

void* __curbrk=0;

int __brk(void *end_data_segment);

int __brk(void *end_data_segment) {
  int res;
  if ((res=__diet_brk(end_data_segment))==0)
    __curbrk=end_data_segment;
  return res;
}

int brk (void *end_data_segment) __attribute__((weak,alias("__brk")));
