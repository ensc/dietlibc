#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[],char* envp[]) {
  int i;
  char** x;
  for (i=0; i<=argc; ++i)
    printf("[%d]: \"%s\"\n",i,argv[i]);
  puts("\nEnvironment:");
  for (x=environ; *x; ++x)
    puts(*x);
  return 0;
}
