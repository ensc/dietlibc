/*
 * dynamic unified-syscalls
 * Olaf Dreesen
 */

#include "../syscalls.s/environ.S"
#include "../syscalls.s/errno.S"

#include <dietfeatures.h>
#include <syscalls.h>

/* ok now include all syscalls.s/*.S and sysdep *.S */

#include "mmap.S"

/* here on i386 I had split them some pre- some post-unified-syscall,
 * because the jumps pre-u.s.c. have only a 1 byte displacement. the first
 * post-u.s.c. have also 1 byte dsplacement the rest a 4 byte.
 * this pre/post split saves about 250 byte of code ....
 *
 * Please put additional syscalls AFTER the post-u.s.c. 
 * or try to not break the savings :) */

#include "select.S"

#include "../syscalls.s/fork.S"
#include "../syscalls.s/read.S"
#include "../syscalls.s/write.S"
#include "../syscalls.s/open.S"
#include "../syscalls.s/close.S"
#include "../syscalls.s/waitpid.S"

#include "../syscalls.s/_llseek.S"
#include "../syscalls.s/getgid.S"
#include "../syscalls.s/getpgid.S"
#include "../syscalls.s/getpid.S"
#include "../syscalls.s/getppid.S"
#include "../syscalls.s/getuid.S"
#include "../syscalls.s/kill.S"
#include "../syscalls.s/llseek.S"
#include "../syscalls.s/lseek.S"
#include "../syscalls.s/lstat.S"
#include "../syscalls.s/lstat64.S"
#include "../syscalls.s/mkdir.S"
#include "../syscalls.s/mprotect.S"
#include "../syscalls.s/nanosleep.S"
#include "../syscalls.s/pipe.S"
#include "../syscalls.s/poll.S"
#include "../syscalls.s/readv.S"
#include "../syscalls.s/signal.S"
#include "../syscalls.s/socketcall.S"
#include "../syscalls.s/stat.S"
#include "../syscalls.s/stat64.S"
#include "../syscalls.s/truncate.S"
#include "../syscalls.s/umask.S"
#include "../syscalls.s/wait4.S"
#include "../syscalls.s/writev.S"

/* OK this is basicaly unified.S */
.text
.weak exit
exit:
.global _exit
_exit:
__exit:
	mov $__NR_exit, %al
__unified_syscall:
	movzbl	%al, %eax
	push	%edi
	push	%esi
	push	%ebx
	movl	%esp,%edi
	movl	0x10(%edi),%ebx
	movl	0x14(%edi),%ecx
	movl	0x18(%edi),%edx
	movl	0x1c(%edi),%esi
	movl	0x20(%edi),%edi
	int	$0x80
	cmp	$-124,%eax
	jbe	.Lnoerror
	neg	%eax

	/* PIC-code Ohhhhh My. */
	push	%eax
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b], %ebx
	call	__errno_location@PLT
	popl	%ebx
	movl	%ebx,(%eax)

	xor	%eax,%eax
	dec	%eax
.Lnoerror:
	pop	%ebx
	pop	%esi
	pop	%edi
	ret

#include "../syscalls.s/__getpagesize.S"
#include "../syscalls.s/__pread.S"
#include "../syscalls.s/__reboot.S"
#include "../syscalls.s/access.S"
#include "../syscalls.s/brk.S"
#include "../syscalls.s/chdir.S"
#include "../syscalls.s/chmod.S"
#include "../syscalls.s/chown.S"
#include "../syscalls.s/chroot.S"
#include "../syscalls.s/dup.S"
#include "../syscalls.s/dup2.S"
#include "../syscalls.s/execve.S"
#include "../syscalls.s/fchdir.S"
#include "../syscalls.s/fchmod.S"
#include "../syscalls.s/fchown.S"
#include "../syscalls.s/fcntl.S"
#include "../syscalls.s/flock.S"
#include "../syscalls.s/fstat.S"
#include "../syscalls.s/fstat64.S"
#include "../syscalls.s/fstatfs.S"
#include "../syscalls.s/fsync.S"
#include "../syscalls.s/ftruncate.S"
#include "../syscalls.s/getcwd.S"
#include "../syscalls.s/getdents.S"
#include "../syscalls.s/getdents64.S"
#include "../syscalls.s/getegid.S"
#include "../syscalls.s/geteuid.S"
#include "../syscalls.s/getgroups.S"
#include "../syscalls.s/getitimer.S"
#include "../syscalls.s/getpriority.S"
#include "../syscalls.s/getresgid.S"
#include "../syscalls.s/getresuid.S"
#include "../syscalls.s/getrlimit.S"
#include "../syscalls.s/getrusage.S"
#include "../syscalls.s/getsid.S"
#include "../syscalls.s/gettimeofday.S"
#include "../syscalls.s/ioctl.S"
#include "../syscalls.s/ioperm.S"
#include "../syscalls.s/iopl.S"
#include "../syscalls.s/ipc.S"
#include "../syscalls.s/lchown.S"
#include "../syscalls.s/link.S"
#include "../syscalls.s/mknod.S"
#include "../syscalls.s/mlock.S"
#include "../syscalls.s/mlockall.S"
#include "../syscalls.s/mount.S"
#include "../syscalls.s/mremap.S"
#include "../syscalls.s/munlockall.S"
#include "../syscalls.s/munmap.S"
#include "../syscalls.s/nice.S"
#include "../syscalls.s/pause.S"
#include "../syscalls.s/personality.S"
#include "../syscalls.s/pwrite.S"
#include "../syscalls.s/query_module.S"
#include "../syscalls.s/readlink.S"
#include "../syscalls.s/rename.S"
#include "../syscalls.s/rmdir.S"
#include "../syscalls.s/sched_get_priority_max.S"
#include "../syscalls.s/sched_get_priority_min.S"
#include "../syscalls.s/sched_getparam.S"
#include "../syscalls.s/sched_getscheduler.S"
#include "../syscalls.s/sched_rr_get_interval.S"
#include "../syscalls.s/sched_setparam.S"
#include "../syscalls.s/sched_setscheduler.S"
#include "../syscalls.s/sched_yield.S"
#include "../syscalls.s/sendfile.S"
#include "../syscalls.s/setdomainname.S"
#include "../syscalls.s/setfsgid.S"
#include "../syscalls.s/setfsuid.S"
#include "../syscalls.s/setgid.S"
#include "../syscalls.s/setgroups.S"
#include "../syscalls.s/sethostname.S"
#include "../syscalls.s/setitimer.S"
#include "../syscalls.s/setpgid.S"
#include "../syscalls.s/setpriority.S"
#include "../syscalls.s/setregid.S"
#include "../syscalls.s/setresgid.S"
#include "../syscalls.s/setresuid.S"
#include "../syscalls.s/setreuid.S"
#include "../syscalls.s/setrlimit.S"
#include "../syscalls.s/setsid.S"
#include "../syscalls.s/setuid.S"
#include "../syscalls.s/sigaction.S"
#include "../syscalls.s/sigaltstack.S"
#include "../syscalls.s/sigpending.S"
#include "../syscalls.s/sigprocmask.S"
#include "../syscalls.s/sigsuspend.S"
#include "../syscalls.s/statfs.S"
#include "../syscalls.s/stime.S"
#include "../syscalls.s/swapoff.S"
#include "../syscalls.s/swapon.S"
#include "../syscalls.s/symlink.S"
#include "../syscalls.s/sync.S"
#include "../syscalls.s/sysctl.S"
#include "../syscalls.s/sysinfo.S"
#include "../syscalls.s/syslog.S"
#include "../syscalls.s/time.S"
#include "../syscalls.s/times.S"
#include "../syscalls.s/umount.S"
#include "../syscalls.s/umount2.S"
#include "../syscalls.s/uname.S"
#include "../syscalls.s/unlink.S"
#include "../syscalls.s/utime.S"
#include "../syscalls.s/vhangup.S"
/* so now this was the last u.s.c. */

/* other asm-files w.o. changes ... */
#define __errno_location __errno_location@PLT
#define _exit __exit
#include "clone.S"
#undef _exit

#define PIC

#include "__longjmp.S"
#include "setjmp.S"
