#include <stdlib.h>

char* getlogin() {
  return getenv("LOGNAME");
}
