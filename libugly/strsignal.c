#include <signal.h>
/* F....ng Kernel haeder is damn broken... */
#ifndef _NSIG
#define _NSIG 64
#endif
#include <asm/signal.h>

const char* strsignal(int sig) {
  if (sig<=SIGUSR2) return sys_siglist[sig];
  else if ((sig>=SIGRTMIN)&&(sig<=SIGRTMAX))
    return "Real time signal";
  else
    return "(unknown signal)";
}
