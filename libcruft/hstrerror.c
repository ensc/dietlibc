#include <netdb.h>

const char* hstrerror(int e) {
  switch (e) {
  case 0: return "OK";
  case NO_DATA: return "No data of requested type.";
  case TRY_AGAIN: return "Temporary failure.";
  case HOST_NOT_FOUND:
  default: return "Unknown host.";
  }
}
