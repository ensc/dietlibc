#include <stdio.h>

main() {
  char buf[1000];
  printf("sftp> ");
  fgets(buf,sizeof buf,stdin);
}
