#include <stdlib.h>
#include <assert.h>
#include <write12.h>

int compint(const void *a, const void *b) {
  register const int* A=a;
  register const int* B=b;
  return *B-*A;
}

void die(const char* message) {
  __write2(message);
  exit(1);
}

int main() {
#define SIZE 1000
  int array[SIZE],array2[SIZE];
  int i,j;
  long a,b,c;
  int *k;
  for (i=0; i<SIZE; ++i) array[i]=rand();
  memcpy(array2,array,sizeof(array));
  qsort(array,SIZE,sizeof(int),compint);
  for (i=0; i<SIZE-1; ++i)
    if (array[i]<array[i+1])
      die("not sorted after qsort!\n");
  for (i=0; i<SIZE; ++i) {
//    printf("element %d: ",i);
    k=bsearch(array+i,array,SIZE,sizeof(int),compint);
    if (!k) die("bsearch returned NULL\n");
    if (k != array+i) die("bsearch found wrong element\n");
//    printf("%d\n",k-array);
  }
  return 0;
}
