#include <stdlib.h>
#define scandir _duh
#include <dirent.h>
#undef scandir
#include <string.h>

int scandir(const char *dir, struct dirent ***namelist,
	    int (*select)(const struct dirent *),
	    int (*compar)(const struct dirent **, const struct dirent **)) {
  DIR* d;
  struct dirent *D;
  int num=0;
  if (!(d=opendir(dir)))
    return -1;
  *namelist=0;
  while ((D=readdir(d))) {
    if (select==0 ||  select(D)) {
      struct dirent **tmp;
      ++num;
/*      printf("realloc %p,%d -> ",*namelist,num*sizeof(struct dirent**)); */
      if (!(tmp=realloc(*namelist,num*sizeof(struct dirent**))) ||
	  !(tmp[num-1]=malloc(sizeof(struct dirent)))) {
	int i;
	for (i=0; i<num-1; ++i) free(tmp[i]);
	free(*namelist);
	closedir(d);
	return -1;
      }
      strncpy(tmp[num-1]->d_name,D->d_name,NAME_MAX);
      *namelist=tmp;
/*      printf("%p; tmp[num-1(%d)]=%p\n",*namelist,num-1,tmp[num-1]); */
    }
  }
  closedir(d);
#if 0
  {
    int i;
    puts("pre-qsort:\n");
    for (i=0; i<num-1; ++i) {
      puts((*namelist)[i]->d_name);
    }
    puts("post-qsort:\n");
  }
#endif
//  qsort(&(*namelist)[0],num,sizeof(struct dirent*),(int (*)(const void*,const void*))(compar));
  qsort(*namelist,num,sizeof(struct dirent*),(int (*)(const void*,const void*))(compar));
  return num;
}
