#include <signal.h>
/* F....ng Kernel haeder is damn broken... */
#ifndef _NSIG
#define _NSIG 64
#endif
#include <asm/signal.h>

char * strsignal(int sig) {
  if (sig==SIGHUP)
    return "Hangup";
  else if (sig==SIGINT)
    return "Interrupt";
  else if (sig==SIGQUIT)
    return "Quit";
  else if (sig==SIGILL)
    return "Illegal instruction";
  else if (sig==SIGTRAP)
    return "Trace/breakpoint trap";
  else if (sig==SIGABRT)
    return "Aborted";
  else if (sig==SIGFPE)
    return "Floating point exception";
  else if (sig==SIGKILL)
    return "Killed";
  else if (sig==SIGBUS)
    return "Bus error";
  else if (sig==SIGSEGV)
    return "Segmentation fault";
  else if (sig==SIGPIPE)
    return "Broken pipe";
  else if (sig==SIGALRM)
    return "Alarm clock";
  else if (sig==SIGTERM)
    return "Terminated";
  else if (sig==SIGURG)
    return "Urgent I/O condition";
  else if (sig==SIGSTOP)
    return "Stopped (signal)";
  else if (sig==SIGTSTP)
    return "Stopped";
  else if (sig==SIGCONT)
    return "Continue";
  else if (sig==SIGCHLD)
    return "Child exited";
  else if (sig==SIGTTIN)
    return "Stopped (tty input)";
  else if (sig==SIGTTOU)
    return "Stopped (tty output)";
  else if (sig==SIGIO)
    return "I/O possible";
  else if (sig==SIGXCPU)
    return "CPU time limit exceeded";
  else if (sig==SIGXFSZ)
    return "File size limit exceeded";
  else if (sig==SIGVTALRM)
    return "Virtual timer expired";
  else if (sig==SIGPROF)
    return "Profiling timer expired";
  else if (sig==SIGWINCH)
    return "Window changed";
  else if (sig==SIGUSR1)
    return "User defined signal 1";
  else if (sig==SIGUSR2)
    return "User defined signal 1";
#ifdef SIGEMT
  else if (sig==SIGEMT)
    return "EMT trap";
#endif
#ifdef SIGSYS
  else if (sig==SIGSYS)
    return "Bad system call";
#endif
#ifdef SIGSTKFLT
  else if (sig==SIGSTKFLT)
    return "Stack fault";
#endif
#ifdef SIGINFO
  else if (sig==SIGINFO)
    return "Information request";
#elif defined(SIGPWR) && (!defined(SIGLOST) || (SIGPWR!=SIGLOST))
  else if (sig==SIGPWR)
    return "Power falure";
#endif
#ifdef SIGLOST
  else if (sig==SIGLOST)
    return "Resource lost";
#endif
  else if ((sig>=SIGRTMIN)&&(sig<=SIGRTMAX))
    return "Real time signal";
  else
    return "(unknown signal)";
}
