all: libpthread.a

ARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')

CFLAGS=-pipe
CROSS=

CC=gcc

VPATH=../$(ARCH)/

PTHREAD_OBJS = \
	__testandset.o \
 \
	thread_internal.o thread_create.o \
 \
	pthread_once.o pthread_spinlock.o \
 \
	pthread_create.o pthread_detach.o \
	pthread_join.o pthread_self.o \
 \
	pthread_attr_getdetachstate.o \
	pthread_attr_getinheritsched.o \
	pthread_attr_getschedparam.o \
	pthread_attr_getschedpolicy.o \
	pthread_attr_getscope.o \
	pthread_attr_init.o \
	pthread_attr_setdetachstate.o \
	pthread_attr_setinheritsched.o \
	pthread_attr_setschedparam.o \
	pthread_attr_setschedpolicy.o \
	pthread_attr_setscope.o \
 \
	pthread_cancel.o pthread_setcancelstate.o \
	pthread_setcanceltype.o pthread_testcancel.o \
 \
	pthread_mutex_init.o \
	pthread_mutex_lock.o \
	pthread_mutex_trylock.o \
	pthread_mutex_unlock.o \
 \
	pthread_mutexattr_getkind_np.o \
	pthread_mutexattr_init.o \
	pthread_mutexattr_setkind_np.o

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $?

include ../$(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

#CFLAGS = -g
CFLAGS += -Wall

CFLAGS += -nostdinc -I.. -I../include -I/usr/lib/gcc-lib/i386-linux/2.95.2/include -Wall

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
	$(CROSS)$(CC) -g $(CFLAGS) -nostdlib ../start.o -o $@ $^ ../dietlibc.a -lgcc


.PHONY: sparc ppc mips arm alpha i386

sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t libdietc.so

mips arm:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t libdietc.so

