#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ftw.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int ftw(const char *dir,int (*fn)(const char *file, const struct stat *sb, int flag), int depth) {
  char *cwd;
  int cwdlen;
  DIR *d;
  struct dirent *de;
  struct stat sb;
  if (chdir(dir)) return -1;
  cwd=alloca(PATH_MAX+1);
  if (!getcwd(cwd,PATH_MAX)) return -1;
  cwd[PATH_MAX]=0;
  cwdlen=strlen(cwd);
/*  write(1,"ftw in ",7); puts(cwd); */
  if (!(d=opendir("."))) return -1;
  while ((de=readdir(d))) {
    int res;
    int flag;
    int nlen;
    char *filename;
    if (de->d_name[0]=='.' &&
	(de->d_name[1]==0 ||
	(de->d_name[1]=='.' && de->d_name[2]==0))) continue;
    nlen=strlen(de->d_name);
    filename=alloca(nlen+cwdlen+3);
    memmove(filename,cwd,cwdlen);
    filename[cwdlen]='/';
    memmove(filename+cwdlen+1,de->d_name,nlen+1);
    if (!lstat(de->d_name,&sb)) {
      if (S_ISLNK(sb.st_mode)) flag=FTW_SL; else
      if (S_ISDIR(sb.st_mode)) flag=FTW_D; else
      flag=FTW_F;
    } else
      flag=FTW_NS;
    res=fn(filename,&sb,flag);
    if (res) return res;
    if (flag==FTW_D && depth>0) {
      res=ftw(filename,fn,depth-1);
      chdir(dir);
      if (res) return res;
    }
/*    puts(de->d_name); */
  }
  closedir(d);
  return 0;
}
