#include <stdlib.h>
#include <string.h>

#include <dietwarning.h>

link_warning("setenv","setenv is a Q&D hack and uses malloc,getenv,putenv,... in other words: BLOAT");

/*
 * Q & D from O.D.
 *
 * if you really want to change the enviroment and stay small:
 *   use "static" strings and putenv
 */

int setenv(const char *name, const char *value, int overwrite)
{
  int len;
  char *tmp = getenv(name);
  if (tmp && (overwrite==0)) return 0;
  len = strlen(name) + strlen(value) + 2;
  tmp = (char*) malloc(len);

  strcpy(tmp,name);
  strcat(tmp,"=");
  strcat(tmp,value);

  return putenv(tmp);
}
