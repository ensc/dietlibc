#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include <fcntl.h>

#include "_dl_int.h"

#define WANT_LD_SO_CONF_SEARCH

static const char *_dl_search_rpath=0;

void _dl_set_rpath(const char *path) { _dl_search_rpath=path; }
const char* _dl_get_rpath() { return _dl_search_rpath; }

/* search a colon (semicolon) seperated path for the libraray "filename" */
static int _dl_search_path(char *buf, int len, const char*path, const int pathlen, const char *filename)
{
  int fd,i=1,ml=len-strlen(filename);
  const char *c,*pe=path+pathlen;

  if (path) {
    for (c=path; c<pe; c+=i) {
      int l=len-1;
      if ((*c==':')||(*c==';')) ++c;
      i=strcspn(c,":;");
      if (i) {
	if (i>ml) continue;	/* if len(path-entry)+len(filename)+2 is greater than the buffer ? SKIP */
	memcpy(buf, c, i); buf[i]=0;
	l-=i;
	strncat(buf, "/", l);
      }
      else
	buf[0]=0;
      strncat(buf, filename, --l);
//      DEBUG(printf("_dl_search_path: %s\n",buf);)
      if ((fd=open(buf,O_RDONLY))!=-1) return fd;
    }
  }
  return -1;
}

/* parse the SMALL file "conf" for lib directories (aem... hang me if you can :) ) */
static int _dl_search_conf(char *buf, int len, const char *conf, const char *filename)
{
  char ld_so_conf[1024];
  int i,l,fd=open(conf,O_RDONLY);
  if (fd!=-1) {
    l=read(fd,ld_so_conf,sizeof(ld_so_conf)-1);
    ld_so_conf[sizeof(ld_so_conf)-1]=0;
    close(fd);
    if (ld_so_conf[l-1]=='\n') ld_so_conf[--l]=0;
    for (i=0;i<l;i++) if (ld_so_conf[i]=='\n') ld_so_conf[i]=':';
    if ((fd=_dl_search_path(buf,len,ld_so_conf,l,filename))!=-1) return fd;
  }
  return -1;
}

int _dl_search(char *buf, int len, const char *filename)
{
  int fd;

  /* 1. search the LD_RUN_PATH (from the executable) */
  if (_dl_search_rpath) {
    if ((fd=_dl_search_path(buf,len,_dl_search_rpath,strlen(_dl_search_rpath),filename))!=-1) return fd;
  }

  /* 2. IF we have a "secure" enviroment THEN search LD_LIBRARY_PATH */
  if (getuid()==geteuid()) {
    char *p=getenv("LD_LIBRARY_PATH");
    if (p)
      if ((fd=_dl_search_path(buf,len,p,strlen(p),filename))!=-1) return fd;
  }

  /* 3. search all pathes in the the /etc/diet.ld.conf, a dietlibc extension :) */
  if ((fd=_dl_search_conf(buf,len,"/etc/diet.ld.conf",filename))!=-1) return fd;

#ifdef WANT_LD_SO_CONF_SEARCH
  /* 4. search all pathes in the the /etc/ld.so.conf / can't handle this ...=lib?? crap */
  if ((fd=_dl_search_conf(buf,len,"/etc/ld.so.conf",filename))!=-1) return fd;
#endif

  /* default path search */
  {
    const char def_path[]="/usr/lib:/lib";
    return _dl_search_path(buf,len,def_path,strlen(def_path),filename);
  }
}
