#include <signal.h>

const char* strsignal(int sig) {
  if (sig<=SIGUSR2) return sys_siglist[sig];
  else if ((sig>=SIGRTMIN)&&(sig<=SIGRTMAX))
    return "Real time signal";
  else
    return "(unknown signal)";
}
