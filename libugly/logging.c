#include "dietfeatures.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <asm/socket.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>

#define _PATH_CONSOLE	"/dev/console"
#define DEF_SIZE 128	/* default buffer size */
#define FAIL_SIZE (3 * sizeof(pid_t) + sizeof("no openlog with ident []") + 1)
			/* see error-cases in vsyslog */

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

static void sigpipe_handler(int signo)
{
  closelog_intern();
}

void vsyslog(int priority, const char *format, void *arg_ptr)
{
  char *buffer, *message;
  char failbuf[FAIL_SIZE + 130];
  char header[130];
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
  strftime(failbuf, FAIL_SIZE, "%h %e %T", localtime_r (&now, &now_tm));

  if (LogStat & LOG_PID)
    headerlen = snprintf(header, 130, "<%d>%s %s[%d]: ", priority, failbuf, LogTag, pid);
  else
    headerlen = snprintf(header, 130, "<%d>%s %s: ", priority, failbuf, LogTag);

  if (LogTag == NULL) {
    if ((LogStat & LOG_PID) != LOG_PID)
      headerlen = snprintf(header, 130, "<%d>%s %s[%d]: ", priority, failbuf, LogTag, pid);
    buflen = snprintf(failbuf, FAIL_SIZE, "%sno openlog with ident [%d]", header, pid);
    buffer = failbuf;
  }
  else if ((buffer = alloca(DEF_SIZE)) == NULL) {
    buflen = snprintf(failbuf, FAIL_SIZE, "%sout of memory [%d]", header, pid);
    buffer = failbuf;
  }
  else {
    (*(__errno_location()))=saved_errno;
    buflen = vsnprintf(buffer, DEF_SIZE, format, arg_ptr);
    free(buffer);
    if (buflen > DEF_SIZE) {
      buffer = realloc(buffer, buflen + 1);
      buflen = vsnprintf(buffer, buflen + 1, format, arg_ptr);
    }
    if ((message = alloca(buflen + headerlen + 1)) == NULL) {
      buflen = snprintf(failbuf, FAIL_SIZE, "%sout of memory [%d]", header, pid);
      buffer = failbuf;
    }
    else {
      strcpy(message, header);
      strcat(message, buffer);
      buflen = buflen + headerlen;
      buffer = message;
    }
  }
  if (LogStat & LOG_PERROR) {
    write(1, buffer, buflen);
    if (buffer[buflen] != '\n') write(1,"\n", 1);
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

  if (!connected || (send(LogFile, buffer, buflen, 0) != buflen)) {
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
      write(fd, buffer, buflen);
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
