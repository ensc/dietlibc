#include <stdio.h>
#include <regex.h>

main() {
  regex_t r;
  char buf[16*1024];
  int i;
  memset(buf,'a',sizeof buf);
  strcpy(buf+sizeof(buf)-100," foo . .. bar\n");
  printf("regcomp %d\n",regcomp(&r,"\\.( ? ? ?\\.)*\\.",REG_EXTENDED|REG_NOSUB));
  printf("regexec %d\n",regexec(&r,buf,1,0,0));
}
