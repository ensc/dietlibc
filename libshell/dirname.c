#include <string.h>
#include <libgen.h>

/*
       path           dirname        basename
       "/usr/lib"     "/usr"         "lib"
       "/usr/"        "/"            "usr"
       "usr"          "."            "usr"
       "/"            "/"            "/"
       "."            "."            "."
       ".."           "."            ".."
*/

char *dirname(char *path) {
  char *c;
again:
  if (!(c=strrchr(path,'/'))) return ".";
  while (c[1]==0) {	/* remove trailing slashes */
    if (c==path) return c;	/* unless path=='/' */
    *c=0;
    if (*--c!='/') break;
  }
  if (*c=='/') { if (c!=path) *c=0; return path; }
  goto again;
}
