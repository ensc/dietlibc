#include <signal.h>
#include <string.h>

main() {
  puts(strsignal(SIGPIPE));
}
