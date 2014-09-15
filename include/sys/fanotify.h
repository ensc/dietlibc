#ifndef SYS_FANOITFY_H
#define SYS_FANOITFY_H

#include <sys/cdefs.h>
#include <stdint.h>
#include <sys/types.h>

struct fanotify_event_metadata {
  uint32_t event_len;
  uint8_t vers, reserved;
  uint16_t metadata_len;
  uint64_t mask;
  int32_t fd, pid;
}

__BEGIN_DECLS

int fanotify_init(unsigned int flags, unsigned int event_f_flags);
int fanotify_mark(int fanotify_fd, unsigned int flags,
		  uint64_t mask, int dirfd, const char *pathname);

__END_DECLS

#endif
