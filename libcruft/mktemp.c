#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <dietwarning.h>

link_warning("mktemp","\n\n[1;33;41m>>> mktemp stinks DON'T USE IT ! <<<[0m\n");

char* mktemp(char* template) {
  int fd;
  if ((fd=mkstemp(template))<0) return 0;
  close(fd);
  return template;
}
