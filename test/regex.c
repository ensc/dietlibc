#include <stdio.h>
#include <regex.h>
#include <assert.h>

main() {
  regex_t r;
  char buf[16*1024];
  int i;
  regmatch_t matches[10];
  memset(buf,'a',sizeof buf);
  strcpy(buf+sizeof(buf)-100," foo . .. bar\n");
#if 0
  printf("regcomp %d\n",regcomp(&r,"\\.( ? ? ?\\.)*\\.",REG_EXTENDED|REG_NOSUB));
  printf("regexec %d\n",regexec(&r,buf,1,0,0));
  regfree(&r);
#endif
#if 1
  printf("regcomp %d\n",regcomp(&r,"^(ksambakdeplugin|mnemisis|kylixxmlrpclib|ripunix|featurekong)@freshmeat.net",REG_EXTENDED|REG_NEWLINE|REG_ICASE));
  {
    int canary[100];
    for (i=0; i<100; ++i) canary[i]=i;
    printf("regexec %d\n",regexec(&r,"mnemisis@freshmeat.net",2,matches,0));
    for (i=0; i<100; ++i) assert(canary[i]==i);
  }
  regfree(&r);
  for (i=0; i<10; ++i) {
    printf("%s(%d %d)",i?", ":" -> ",matches[i].rm_so,matches[i].rm_eo);
  }
  printf("\n");
#endif
  printf("regcomp %d\n",regcomp(&r,"^(a|b|c|d|e)@freshmeat.net",REG_EXTENDED|REG_NEWLINE|REG_NOSUB|REG_ICASE));
  printf("regexec %d\n",regexec(&r,"a@freshmeat.net",1,0,0));
  regfree(&r);
}
