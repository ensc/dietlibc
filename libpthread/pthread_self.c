#include <unistd.h>
#include <pthread.h>

pthread_t pthread_self() { return getpid(); }
