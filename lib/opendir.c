#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>

DIR *opendir (const char *name) {
  DIR *t=(DIR*)malloc(sizeof(DIR));
  if (t) {
    if ((t->fd=open(name,O_RDONLY|O_DIRECTORY))>=0) {
      t->num=t->cur=0;
    } else {
      free(t);
      t=0;
    }
  }
  return t;
}
