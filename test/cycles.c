#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char* argv[]) {
  long a,b;
  asm("rdtsc":"=a"(a)::"edx");
  if (!fork()) { execve(argv[1],argv+1,environ); exit(1); }
  wait(0);
  asm("rdtsc":"=a"(b)::"edx");
  printf("%lu cycles\n",b-a);
}
