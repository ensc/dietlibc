#include <pthread.h>

char *strtok_r(char *s, const char *delim, char **ptrptr);

static char *strtok_pos;
static pthread_mutex_t mutex_strtok;

char *strtok(char *s, const char *delim)
{
  char* ret;
  pthread_mutex_lock(&mutex_strtok);
  ret=strtok_r(s,delim,&strtok_pos);
  pthread_mutex_unlock(&mutex_strtok);
  return ret;
}
