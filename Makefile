MYARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')
ARCH=$(MYARCH)

OBJDIR=bin-$(ARCH)

HOME=$(shell pwd)

all: $(OBJDIR) $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o $(OBJDIR)/dyn_stop.o \
	$(OBJDIR)/dietlibc.a $(OBJDIR)/liblatin1.a $(OBJDIR)/librpc.a $(OBJDIR)/libpthread.a \
	$(OBJDIR)/diet $(OBJDIR)/elftrunc

CFLAGS=-pipe
CROSS=

CC=gcc

VPATH=lib:libstdio:libugly:libcruft:libcrypt:libshell:liblatin1:librpc:libregex:syscalls.c

SYSCALLOBJ=$(patsubst syscalls.s/%.S,$(OBJDIR)/%.o,$(wildcard syscalls.s/*.S))

LIBOBJ=$(patsubst lib/%.c,$(OBJDIR)/%.o,$(wildcard lib/*.c))
LIBUGLYOBJ=$(patsubst libugly/%.c,$(OBJDIR)/%.o,$(wildcard libugly/*.c))
LIBSTDIOOBJ=$(patsubst libstdio/%.c,$(OBJDIR)/%.o,$(wildcard libstdio/*.c))
LIBCRUFTOBJ=$(patsubst libcruft/%.c,$(OBJDIR)/%.o,$(wildcard libcruft/*.c))
LIBCRYPTOBJ=$(patsubst libcrypt/%.c,$(OBJDIR)/%.o,$(wildcard libcrypt/*.c))
LIBSHELLOBJ=$(patsubst libshell/%.c,$(OBJDIR)/%.o,$(wildcard libshell/*.c))

LIBRPCOBJ=$(patsubst librpc/%.c,$(OBJDIR)/%.o,$(wildcard librpc/*.c))
LIBREGEXOBJ=$(patsubst libregex/%.c,$(OBJDIR)/%.o,$(wildcard libregex/*.c))

LIBPTHREAD_OBJS=$(patsubst libpthread/%.c,$(OBJDIR)/%.o,$(shell ./threadsafe.sh)) $(OBJDIR)/__testandset.o

include $(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

ifneq ($(DEBUG),)
CFLAGS = -g
COMMENT = :
endif
CFLAGS += -Wall -Wno-switch

PWD=$(shell pwd)

.SUFFIXES:
.SUFFIXES: .S .c

$(OBJDIR):
	mkdir $@

% :: %,v

$(OBJDIR)/%.o: %.S
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $< -o $@

$(OBJDIR)/pthread_%.o: libpthread/pthread_%.c
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $< -o $@
	$(COMMENT) $(CROSS)strip -x -R .comment -R .note $@

$(OBJDIR)/%.o: %.c
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $< -o $@
	$(COMMENT) $(CROSS)strip -x -R .comment -R .note $@

DIETLIBC_OBJ = $(SYSCALLOBJ) $(LIBOBJ) $(LIBSTDIOOBJ) $(LIBUGLYOBJ) \
$(LIBCRUFTOBJ) $(LIBCRYPTOBJ) $(LIBSHELLOBJ) $(LIBREGEXOBJ) \
$(OBJDIR)/__longjmp.o $(OBJDIR)/setjmp.o $(OBJDIR)/unified.o \
$(OBJDIR)/mmap.o $(OBJDIR)/clone.o

$(OBJDIR)/dietlibc.a: $(DIETLIBC_OBJ) $(OBJDIR)/start.o
	$(CROSS)ar cru $@ $(DIETLIBC_OBJ)

$(OBJDIR)/librpc.a: $(LIBRPCOBJ)
	$(CROSS)ar cru $@ $(LIBRPCOBJ)

LIBLATIN1_OBJS=$(patsubst liblatin1/%.c,$(OBJDIR)/%.o,$(wildcard liblatin1/*.c))
$(OBJDIR)/liblatin1.a: $(LIBLATIN1_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR)/libpthread.a: $(LIBPTHREAD_OBJS) dietfeatures.h
	$(CROSS)ar cru $@ $(LIBPTHREAD_OBJS)

$(OBJDIR)/libdietc.so: $(OBJDIR)/dietlibc.a
	$(CROSS)ld -whole-archive -shared -o $@ $^

$(SYSCALLOBJ): syscalls.h

$(OBJDIR)/elftrunc: $(OBJDIR)/diet contrib/elftrunc.c
	bin-$(MYARCH)/diet $(CROSS)$(CC) $(CFLAGS) -o $@ contrib/elftrunc.c

$(OBJDIR)/diet: $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o diet.c $(OBJDIR)/dietlibc.a $(OBJDIR)/dyn_stop.o
	$(CROSS)$(CC) -Iinclude $(CFLAGS) -nostdlib -o $@ $^ -DDIETHOME=\"$(HOME)\"
	$(CROSS)strip -R .comment -R .note $@

$(OBJDIR)/djb: $(OBJDIR)/compile $(OBJDIR)/load

$(OBJDIR)/compile:
	echo 'exec gcc $(CFLAGS) -I$(PWD)/$(OBJDIR)/include -c $${1+"$$@"}' > $@
	chmod 755 $@

$(OBJDIR)/load:
	echo 'main="$$1"; shift; exec gcc -nostdlib -o "$$main" $(PWD)/$(OBJDIR)/start.o "$$main".o $${1+"$$@"} $(PWD)/$(OBJDIR)/dietlibc.a -lgcc'  > $@
	chmod 755 $@

clean:
	rm -f *.o *.a t t1 compile load elftrunc exports mapfile libdietc.so
	rm -rf bin-*
	$(MAKE) -C examples clean

tar: clean
	rm -f armv4l
	ln -sf arm armv4l
	cd ..; tar cvvf dietlibc.tar.bz2 dietlibc --use=bzip2 --exclude CVS

$(OBJDIR)/exports: $(OBJDIR)/dietlibc.a
	nm -g $(OBJDIR)/dietlibc.a | grep -w T | awk '{ print $$3 }' | sort -u > $(OBJDIR)/exports

.PHONY: t t1
t:
	$(CROSS)$(CC) -g $(CFLAGS) -fno-builtin -nostdlib -Iinclude -o t t.c $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a -lgcc -Wl,-Map,mapfile

t1:
	$(CROSS)$(CC) -g -o t1 t.c

install: $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a $(OBJDIR)/liblatin1.a $(OBJDIR)/elftrunc $(OBJDIR)/diet
	cp $(OBJDIR)/start.o $(INSTALLPREFIX)$(prefix)/lib/dietstart.o
	cp $(OBJDIR)/dietlibc.a $(INSTALLPREFIX)$(prefix)/lib/libdietc.a
	cp $(OBJDIR)/liblatin1.a $(INSTALLPREFIX)$(prefix)/lib/libdietlatin1.a
	cp $(OBJDIR)/diet $(INSTALLPREFIX)$(prefix)/bin/diet

.PHONY: sparc ppc mips arm alpha i386

arm sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t bin-$@/libdietc.so

mips:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t bin-$@/libdietc.so

cross:
	$(MAKE) arm sparc ppc alpha i386 mips


# these depend on dietfeatures.h for large file backward compatibility
$(OBJDIR)/__fstat64.o $(OBJDIR)/__lstat64.o $(OBJDIR)/__stat64.o $(OBJDIR)/lseek64.o: dietfeatures.h

# these depend on dietfeatures.h for thread support
$(OBJDIR)/alloc.o $(OBJDIR)/perror.o $(OBJDIR)/logging.o $(OBJDIR)/unified.o $(OBJDIR)/clone.o $(OBJDIR)/set_errno.o: dietfeatures.h

# these depend on dietfeatures.h for linker warnings
$(OBJDIR)/assert_fail.o $(OBJDIR)/sprintf.o $(OBJDIR)/vsnprintf.o $(OBJDIR)/___div.o $(OBJDIR)/fflush.o $(OBJDIR)/setvbuf.o $(OBJDIR)/system.o $(OBJDIR)/sendfile.o $(OBJDIR)/setenv.o: dietfeatures.h

# these depend on dietfeatures.h for buffered stdio
fclose.o $(OBJDIR)/fdglue.o $(OBJDIR)/fflush.o $(OBJDIR)/fgetc.o $(OBJDIR)/fputc.o $(OBJDIR)/fread.o $(OBJDIR)/fseek.o $(OBJDIR)/printf.o $(OBJDIR)/setvbuf.o $(OBJDIR)/stderr.o $(OBJDIR)/stdin.o $(OBJDIR)/stdout.o $(OBJDIR)/fwrite.o $(OBJDIR)/puts.o: dietfeatures.h

# these depend on dietfeatures.h for fast string routines
strcasecmp.o $(OBJDIR)/strcat.o $(OBJDIR)/strchr.o $(OBJDIR)/strcmp.o $(OBJDIR)/strcpy.o $(OBJDIR)/strlen.o $(OBJDIR)/strncasecmp.o $(OBJDIR)/strncat.o $(OBJDIR)/strrchr.o: dietfeatures.h

# these depend on dietfeatures.h for /proc
$(OBJDIR)/tty.o: dietfeatures.h

# these depend on dietfeatures.h for ungetc support ;-)
$(OBJDIR)/ungetc.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_TZFILE_PARSER
$(OBJDIR)/localtime_r.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_BUGGY_GLIBC_STRSEP
$(OBJDIR)/strsep.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_SMALL_STDIO_BUFS
$(LIBSTDIOOBJ): dietfeatures.h

# these depend on dietfeatures.h for WANT_FULL_RESOLV_CONF
$(OBJDIR)/dnscruft.o $(OBJDIR)/dnscruft2.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_THREAD_SAFE and errno
$(LIBRPCOBJ) $(OBJDIR)/logging.o $(OBJDIR)/alloc.o $(OBJDIR)/cfsetospeed.o $(OBJDIR)/cfsetispeed.o \
$(OBJDIR)/execl.o $(OBJDIR)/execlp.o $(OBJDIR)/execv.o $(OBJDIR)/execvp.o $(OBJDIR)/isatty.o \
$(OBJDIR)/lockf.o $(OBJDIR)/perror.o $(OBJDIR)/remove.o $(OBJDIR)/set_errno.o $(OBJDIR)/sigemptyset.o \
$(OBJDIR)/tcsetattr.o $(OBJDIR)/dnscruft2.o $(OBJDIR)/dnscruft.o $(OBJDIR)/entlib.o \
$(OBJDIR)/gethostbyaddr.o $(OBJDIR)/gethostbyaddr_r.o $(OBJDIR)/gethostbyname.o \
$(OBJDIR)/gethostbyname_r.o $(OBJDIR)/gethostbyname2.o $(OBJDIR)/gethostbyname2_r.o \
$(OBJDIR)/inet_pton.o $(OBJDIR)/mkstemp.o $(OBJDIR)/mktemp.o $(OBJDIR)/tempnam.o \
$(OBJDIR)/glob.o $(OBJDIR)/realpath.o $(OBJDIR)/fdglue.o $(OBJDIR)/fdglue2.o \
$(OBJDIR)/getaddrinfo.o $(OBJDIR)/getnameinfo.o $(OBJDIR)/getprotoent.o \
$(OBJDIR)/getservent.o $(OBJDIR)/iconv.o $(OBJDIR)/iconv_open.o \
$(OBJDIR)/netent.o $(OBJDIR)/system.o: dietfeatures.h

# CFLAGS+=-W -Wshadow -Wid-clash-31 -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings
