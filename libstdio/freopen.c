#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>

FILE *freopen (const char *path, const char *mode, FILE *stream) {
  fclose(stream);
  stream=fopen(path,mode);
  return stream;
}
