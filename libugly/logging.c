#include "dietfeatures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <asm/socket.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <signal.h>
#ifdef WANT_THREAD_SAVE
#include <pthread.h>
#endif

#define _PATH_CONSOLE	"/dev/console"
#define BUF_SIZE 512	/* messagebuffer size (>= 200) */

/* those have to be global *sigh* */
static volatile int	connected = 0;  /* have done connect */
static volatile int	LogMask = 0xff; /* mask of priorities to be logged */
static char		*LogTag = NULL; /* string to tag the entry with */
static int		LogFile = -1;   /* fd for log */
static int		LogType = SOCK_DGRAM; /* type of socket connection */
static int		LogFacility = LOG_USER; /* default facility code */
static int		LogStat;	/* status bits, set by openlog() */
extern char		*__progname;	/* Program name, from crt0. */
static struct sockaddr	SyslogAddr;	/* AF_UNIX address of local logger */

#ifdef WANT_THREAD_SAVE
static pthread_mutex_t	syslog_mutex= PTHREAD_MUTEX_INITIALIZER;
#endif

static void closelog_intern(void)
{
  if (!connected) return;
  close(LogFile);
  LogFile = -1;
  connected = 0;
}

void closelog(void)
{
#ifdef WANT_THREAD_SAVE
  pthread_mutex_lock(&syslog_mutex);
#endif

  closelog_intern();

  if (LogTag != NULL) {
    free(LogTag);
    LogTag = NULL;
  }
  LogType = SOCK_DGRAM;

#ifdef WANT_THREAD_SAVE
  pthread_mutex_unlock(&syslog_mutex);
#endif
}

static void openlog_intern(int option, int facility)
{
  LogStat = option;
  if (facility && ((facility & ~LOG_FACMASK) == 0))
    LogFacility = facility;

  /* yep, there is a continue inside ;) */
  while(1) {
    if (LogFile == -1) {
      SyslogAddr.sa_family = AF_UNIX;
      strncpy(SyslogAddr.sa_data, _PATH_LOG, sizeof(SyslogAddr.sa_data));
      if (LogStat & LOG_NDELAY)
      {
	if ((LogFile = socket(AF_UNIX, LogType, 0)) == -1) return;
	fcntl(LogFile, F_SETFD, 1);
      }
    }
    if ((LogFile != -1) && !connected) {
      int old_errno = (*(__errno_location()));
      if(connect(LogFile, &SyslogAddr, sizeof(SyslogAddr)) == -1) {
	int saved_errno = (*(__errno_location()));
	close(LogFile);
	LogFile = -1;
	if((LogType == SOCK_DGRAM) && (saved_errno == EPROTOTYPE)) {
	  /* retry with SOCK_STREAM instead of SOCK_DGRAM */
	  LogType = SOCK_STREAM;
	  (*(__errno_location()))=old_errno;
	  continue;
	}
      }
      else connected = 1;
    }
    break;
  }
}

/* has to be secured against multiple, simultanious call's in threaded environment */
void openlog(char *ident, int option, int facility)
{
  char a;

#ifdef WANT_THREAD_SAVE
  pthread_mutex_lock(&syslog_mutex);
#endif

  if (LogTag != NULL) free(LogTag);
  if (ident != NULL) {
    if(strlen(ident) > 80) {
      a = ident[80];
      ident[80] = '\0';
      LogTag = strdup(ident);
      ident[80] = a;
    }
    else LogTag = strdup(ident);
  }
  openlog_intern(option, facility);

#ifdef WANT_THREAD_SAVE
  pthread_mutex_unlock(&syslog_mutex);
#endif
}

int setlogmask(int mask)
{
  int old = LogMask;
  if (mask) LogMask = mask;
  return old;
}

#ifdef WANT_THREAD_SAVE
static void cancel_handler(void *ptr)
{
  pthread_mutex_unlock(&syslog_mutex);
}
#endif

void vsyslog(int priority, const char *format, va_list arg_ptr)
{
  char buffer[BUF_SIZE];
  char time_buf[20];
  int buflen, headerlen;
  time_t now;
  struct tm now_tm;
  pid_t pid;
  int fd;
  int sigpipe;
  struct sigaction action, oldaction;
  struct sigaction *oldaction_ptr = NULL;
  int saved_errno = (*(__errno_location()));

  /* check for invalid priority/facility bits */
  if (priority & ~(LOG_PRIMASK|LOG_FACMASK)) {
    syslog(LOG_ERR|LOG_CONS|LOG_PERROR|LOG_PID, "syslog: unknown facility/priorityority: %x", priority);
    priority &= LOG_PRIMASK|LOG_FACMASK;
  }

  /* check priority against setlogmask */
  if ((LOG_MASK(LOG_PRI(priority)) && LogMask) == 0) return;

  /* Set default facility if none specified. */
  if ((priority & LOG_FACMASK) == 0) priority |= LogFacility;

  pid = getpid();
  time(&now);
  strftime(time_buf, 20, "%h %e %T", localtime_r (&now, &now_tm));

  if (LogStat & LOG_PID)
    headerlen = snprintf(buffer, 130, "<%d>%s %s[%d]: ", priority, time_buf, LogTag, pid);
  else
    headerlen = snprintf(buffer, 130, "<%d>%s %s: ", priority, time_buf, LogTag);

  if (LogTag == NULL) {
    if ((LogStat & LOG_PID) != LOG_PID)
      headerlen = snprintf(buffer, 130, "<%d>%s (unknown)[%d]: ", priority, time_buf, pid);
    strcat(buffer+headerlen, "no openlog with ident, please check code!");
    buflen = 41;
  }
  else {
    (*(__errno_location()))=saved_errno;
    buflen = vsnprintf(buffer+headerlen, BUF_SIZE - headerlen, format, arg_ptr);
  }
  if (LogStat & LOG_PERROR) {
    write(1, buffer+headerlen, buflen);
    if (buffer[headerlen+buflen] != '\n') write(1,"\n", 1);
  }

#ifdef WANT_THREAD_SAVE
  pthread_cleanup_push(cancel_handler, &oldaction_ptr);
  pthread_mutex_lock(&syslog_mutex);
#endif

  /* prepare for broken connection */
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  sigemptyset(&action.sa_mask);

  if ((sigpipe = sigaction (SIGPIPE, &action, &oldaction))==0)
    oldaction_ptr = &oldaction;

  if (!connected) openlog_intern(LogStat | LOG_NDELAY, 0);

  /* If we have a SOCK_STREAM connection, also send ASCII NUL as a
   * record terminator. */
  if (LogType == SOCK_STREAM) buflen++;

  if (!connected || (send(LogFile, buffer, buflen+headerlen, 0) != buflen+headerlen)) {
    if (LogType == SOCK_STREAM) buflen--;
    closelog_intern();
    /*
     * Output the message to the console; don't worry about blocking,
     * if console blocks everything will.  Make sure the error reported
     * is the one from the syslogd failure.
     */
    if ((LogStat & LOG_CONS) &&
       ((fd = open(_PATH_CONSOLE, O_WRONLY|O_NOCTTY, 0)) >= 0))
    {
      write(fd, buffer, buflen+headerlen);
      write(fd, "\r\n", 2);
    }
  }

  if (sigpipe == 0)
    sigaction(SIGPIPE, &oldaction, (struct sigaction *) NULL);

#ifdef WANT_THREAD_SAVE
  pthread_cleanup_pop(0);
  pthread_mutex_unlock(&syslog_mutex);
#endif
}

void syslog(int priority, const char *format, ...)
{
  va_list arg_ptr;
  va_start(arg_ptr, format);
  vsyslog(priority, format, arg_ptr);
  va_end(arg_ptr);
}
