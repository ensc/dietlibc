all: libpthread.a

ARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')

CFLAGS=-pipe
CROSS=

CC=gcc

VPATH=../$(ARCH)/

PTHREAD_OBJS = \
	__testandset.o \
 \
	pthread_internal.o \
	pthread_key.o \
 \
	pthread_once.o pthread_spinlock.o \
 \
	pthread_create.o pthread_detach.o \
	pthread_join.o pthread_self.o \
	pthread_exit.o pthread_equal.o \
 \
	pthread_cleanup_push.o \
	pthread_cleanup_pop.o \
 \
	pthread_attr_getdetachstate.o \
	pthread_attr_getinheritsched.o \
	pthread_attr_getschedparam.o \
	pthread_attr_getschedpolicy.o \
	pthread_attr_getscope.o \
	pthread_attr_getstackaddr.o \
	pthread_attr_getstacksize.o \
	pthread_attr_init.o \
	pthread_attr_setdetachstate.o \
	pthread_attr_setinheritsched.o \
	pthread_attr_setschedparam.o \
	pthread_attr_setschedpolicy.o \
	pthread_attr_setscope.o \
	pthread_attr_setstackaddr.o \
	pthread_attr_setstacksize.o \
 \
	pthread_cancel.o pthread_setcancelstate.o \
	pthread_setcanceltype.o pthread_testcancel.o \
 \
	pthread_mutex_init.o \
	pthread_mutex_destroy.o \
	pthread_mutex_lock.o \
	pthread_mutex_trylock.o \
	pthread_mutex_unlock.o \
 \
	pthread_mutexattr_getkind_np.o \
	pthread_mutexattr_init.o \
	pthread_mutexattr_setkind_np.o \
 \
	pthread_condattr.o \
	pthread_cond_broadcast.o \
	pthread_cond_destroy.o \
	pthread_cond_init.o \
	pthread_cond_signal.o \
	pthread_cond_timedwait.o \
	pthread_cond_wait.o \
 \
	pthread_key_create.o \
	pthread_key_delete.o \
	pthread_getspecific.o \
	pthread_setspecific.o \
 \
	pthread_atfork.o \
	pthread_sys_alloc.o \
	pthread_sys_close.o \
	pthread_sys_create.o \
	pthread_sys_fcntl.o \
	pthread_sys_fsync.o \
	pthread_sys_nanosleep.o \
	pthread_sys_logging.o \
	pthread_sys_open.o \
	pthread_sys_pause.o \
	pthread_sys_read.o \
	pthread_sys_sigsuspend.o \
	pthread_sys_tcdrain.o \
	pthread_sys_waitpid.o \
	pthread_sys_write.o

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $?

include ../$(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

#CFLAGS = -g
CFLAGS += -Wall

CFLAGS += -I.. -I../include -Wall

PWD=$(shell pwd)

.SUFFIXES:
.SUFFIXES: .S .c

% :: %,v

%.o: %.S
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $<

%.o: %.c
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $<
#	$(CROSS)strip -x -R .comment -R .note $@

libpthread.a: $(PTHREAD_OBJS)
	ar cr $@ $^

libpthread.so: libpthread.a
	$(CROSS)ld -whole-archive -shared -o $@ $^


clean:
	$(RM) *.o *.a *.so *.out *~

exports: libpthread.a
	nm -g libpthread.a | grep -w T | awk '{ print $$3 }' | sort -u > exports

.PHONY: test.out

test.out: test.o libpthread.a $(LIBS)
	$(CROSS)$(CC) -g $(CFLAGS) -nostdlib ../bin-$(ARCH)/start.o -o $@ $^ ../bin-$(ARCH)/dietlibc.a -lgcc


.PHONY: sparc ppc mips arm alpha i386

sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t libdietc.so

mips arm:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t libdietc.so

