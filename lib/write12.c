#include <unistd.h>
#include <string.h>
#include <write12.h>

int __write1 (const char* s) {
  return write(1, s, strlen(s));
}

int __write2 (const char* s) {
  return write(2, s, strlen(s));
}
