#include <sys/wait.h>

int wait3(int* status,int opts,struct rusage* rusage) {
  return wait4(-1,status,opts,rusage);
}
