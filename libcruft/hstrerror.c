#include <netdb.h>

const char* hstrerror(int h_errno) {
  switch (h_errno) {
  case 0: return "OK";
  case HOST_NOT_FOUND: return "Unknown host";
  case TRY_AGAIN: return "Temporary failure.";
  default: return "DNS error (hehe)";
  }
}
