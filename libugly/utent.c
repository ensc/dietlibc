#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <utmp.h>

static const char *utmp_file_name = _PATH_UTMP;
static int fd = -1;

void utmpname(const char *file) {
  if (file)
    utmp_file_name = file;
  else
    utmp_file_name = _PATH_UTMP;
}

void setutent() {
  if (fd<0) fd = open(utmp_file_name,O_RDWR);
  lseek(fd,0,SEEK_SET);
}

void endutent() {
  if (fd<0) return;
  close(fd); fd=-1;
}

struct utmp *getutent(void) {
  static struct utmp getutent_tmp;

  if (fd<0) {
    setutent();
    if (fd<0) return 0;
  }
  if (read(fd, &getutent_tmp, sizeof(struct utmp))<1)
    return 0;
  return &getutent_tmp;
}

struct utmp *getutid(struct utmp *ut) {
  struct utmp *tmp;

  while (1) {
    tmp = getutent();
    if (!tmp) break;
    if (ut->ut_type && (ut->ut_type <= OLD_TIME)) {
      if (ut->ut_type == tmp->ut_type) break;
    }

    if ((ut->ut_type >= INIT_PROCESS) && (ut->ut_type <= DEAD_PROCESS)) {
      if (!strncmp(ut->ut_id,tmp->ut_id,4)) break;
    }
  }
  return tmp;
}

struct utmp *getutline(struct utmp *ut) {
  struct utmp *tmp;

  while(1) {
    tmp = getutent();
    if (!tmp) break;
    if ((tmp->ut_type == USER_PROCESS) || (tmp->ut_type == LOGIN_PROCESS)) {
      if (!strncmp(ut->ut_line,tmp->ut_line,UT_LINESIZE)) break;
    }
  }
  return tmp;
}

void pututline(struct utmp *ut) {
  struct utmp *tmp;

  if ((tmp = getutid(ut))) {
    lseek(fd, - sizeof(struct utmp), SEEK_CUR);
    write(fd, ut, sizeof(struct utmp));
  }
  else {
    lseek(fd, 0, SEEK_END);
    write(fd, ut, sizeof(struct utmp));
  }
}

