#include <unistd.h>
#include <string.h>
#include <write12.h>

int __attribute__ (( regparm(1) )) __write1 (const char* s) {
  return write(1, s, strlen(s));
}

int __attribute__ (( regparm(1) )) __write2 (const char* s) {
  return write(2, s, strlen(s));
}
