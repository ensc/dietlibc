#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>
#include <errno.h>

FILE *freopen (const char *path, const char *mode, FILE *stream) {
  if (stream) {
    fflush(stream);
    if (close(stream->fd)) stream=0;
    else {
      int f=__stdio_parse_mode(mode);
      if ((stream->fd=open(path,f,0666))!=-1) {
	struct stat st;
	fstat(stream->fd,&st);
	stream->flags=(S_ISFIFO(st.st_mode))?FDPIPE:0;
      }
    }
  } else errno=EINVAL;
  return stream;
}
