#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>

#define __WANT_POSIX1B_SIGNALS__

#include <sys/types.h>
#include <sys/time.h>

#define NSIG		32

#ifdef __mips__
#define _NSIG		128
#else
#define _NSIG		64
#endif

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCLD		SIGCHLD
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
#define SIGUNUSED	31

/* These should not be considered constants from userland.  */
#define SIGRTMIN	32
#define SIGRTMAX	(_NSIG-1)

/* SA_FLAGS values: */
#define SA_NOCLDSTOP	0x00000001
#define SA_NOCLDWAIT	0x00000002 /* not supported yet */
#define SA_SIGINFO	0x00000004
#define SA_RESTORER	0x04000000
#define SA_ONSTACK	0x08000000
#define SA_RESTART	0x10000000
#define SA_INTERRUPT	0x20000000 /* dummy -- ignored */
#define SA_NODEFER	0x40000000
#define SA_RESETHAND	0x80000000

/* ugh, historic Linux legacy, for gpm :-( */
#define SA_NOMASK	SA_NODEFER
#define SA_ONESHOT	SA_RESETHAND

/* sigaltstack controls */
#define SS_ONSTACK	1
#define SS_DISABLE	2

#define MINSIGSTKSZ	2048
#define SIGSTKSZ	8192

#define SIG_BLOCK	0	/* for blocking signals */
#define SIG_UNBLOCK	1	/* for unblocking signals */
#define SIG_SETMASK	2	/* for setting the signal mask */

typedef int sig_atomic_t;

typedef void (*sighandler_t)(int);

#ifdef _BSD_SOURCE
typedef sighandler_t sig_t;
#endif

#define SIG_DFL ((sighandler_t)0)	/* default signal handling */
#define SIG_IGN ((sighandler_t)1)	/* ignore signal */
#define SIG_ERR ((sighandler_t)-1)	/* error return from signal */

typedef union sigval {
  int sival_int;
  void *sival_ptr;
} sigval_t;

#define SI_MAX_SIZE	128
#define SI_PAD_SIZE	((SI_MAX_SIZE/sizeof(int)) - 3)

typedef struct siginfo {
  int si_signo;
  int si_errno;
  int si_code;
  union {
    int _pad[SI_PAD_SIZE];
    /* kill() */
    struct {
      pid_t _pid;		/* sender's pid */
      uid_t _uid;		/* sender's uid */
    } _kill;
    /* POSIX.1b timers */
    struct {
      unsigned int _timer1;
      unsigned int _timer2;
    } _timer;
    /* POSIX.1b signals */
    struct {
      pid_t _pid;		/* sender's pid */
      uid_t _uid;		/* sender's uid */
      sigval_t _sigval;
    } _rt;
    /* SIGCHLD */
    struct {
      pid_t _pid;		/* which child */
      uid_t _uid;		/* sender's uid */
      int _status;		/* exit code */
      clock_t _utime;
      clock_t _stime;
    } _sigchld;
    /* SIGILL, SIGFPE, SIGSEGV, SIGBUS */
    struct {
      void *_addr; /* faulting insn/memory ref. */
    } _sigfault;
    /* SIGPOLL */
    struct {
      int _band;	/* POLL_IN, POLL_OUT, POLL_MSG */
      int _fd;
    } _sigpoll;
  } _sifields;
} siginfo_t;

/*
 * How these fields are to be accessed.
 */
#define si_pid		_sifields._kill._pid
#define si_uid		_sifields._kill._uid
#define si_status	_sifields._sigchld._status
#define si_utime	_sifields._sigchld._utime
#define si_stime	_sifields._sigchld._stime
#define si_value	_sifields._rt._sigval
#define si_int		_sifields._rt._sigval.sival_int
#define si_ptr		_sifields._rt._sigval.sival_ptr
#define si_addr		_sifields._sigfault._addr
#define si_band		_sifields._sigpoll._band
#define si_fd		_sifields._sigpoll._fd

#define _NSIG_WORDS	((_NSIG/sizeof(long))>>3)

typedef struct {
  unsigned long sig[_NSIG_WORDS];
} sigset_t;

struct sigaction {
  union {
    sighandler_t _sa_handler;
    void (*_sa_sigaction)(int, siginfo_t*, void*);
  } _u;
  unsigned long sa_flags;
  void (*sa_restorer)(void);
  sigset_t sa_mask;
};

#define sa_handler	_u._sa_handler
#define sa_sigaction	_u._sa_sigaction

typedef struct sigaltstack {
  void *ss_sp;
  int ss_flags;
  size_t ss_size;
} stack_t;

int sigaltstack(const struct sigaltstack *newstack, struct sigaltstack *oldstack) __THROW;

int sigemptyset(sigset_t *set) __THROW;
int sigfillset(sigset_t *set) __THROW;
int sigaddset(sigset_t *set, int signum) __THROW;
int sigdelset(sigset_t *set, int signum) __THROW;
int sigismember(const sigset_t *set, int signo) __THROW;
int sigsuspend(const sigset_t *mask) __THROW;
int sigpending(sigset_t *set) __THROW;
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) __THROW;


sighandler_t signal(int signum, sighandler_t action);

int raise (int sig) __THROW;
int kill(pid_t pid, int sig) __THROW;

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) __THROW;

int sigtimedwait(const sigset_t *mask, siginfo_t *info, const struct timespec *ts) __THROW;
int sigqueueinfo(int pid, int sig, siginfo_t *info) __THROW;
int siginterrupt(int sig, int flag) __THROW;

/* 0 is OK ! kernel puts in MAX_THREAD_TIMEOUT :) */
#define sigwaitinfo(m, i) sigtimedwait((m),(i),0)

extern const char *const sys_siglist[];

#define killpg(pgrp,sig) kill(-pgrp,sig)

#endif
