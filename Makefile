all: start.o dietlibc.a liblatin1.a elftrunc

ARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')

CFLAGS=-pipe
CROSS=

CC=gcc

VPATH=lib:libstdio:libugly:libcruft:liblatin1:syscalls.c

SYSCALLOBJ=$(patsubst syscalls.s/%.S,%.o,$(wildcard syscalls.s/*.S))

LIBOBJ=$(patsubst lib/%.c,%.o,$(wildcard lib/*.c))
LIBUGLYOBJ=$(patsubst libugly/%.c,%.o,$(wildcard libugly/*.c))
LIBSTDIOOBJ=$(patsubst libstdio/%.c,%.o,$(wildcard libstdio/*.c))
LIBCRUFTOBJ=$(patsubst libcruft/%.c,%.o,$(wildcard libcruft/*.c))

include $(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

#CFLAGS = -g
CFLAGS += -Wall

PWD=$(shell pwd)

.SUFFIXES:
.SUFFIXES: .S .c

% :: %,v

%.o: %.S
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $<

%.o: %.c
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $<
#	$(CROSS)strip -x -R .comment -R .note $@

DIETLIBC_OBJ = $(SYSCALLOBJ) $(LIBOBJ) $(LIBSTDIOOBJ) $(LIBUGLYOBJ) \
$(LIBCRUFTOBJ) __longjmp.o setjmp.o unified.o mmap.o clone.o

dietlibc.a: $(DIETLIBC_OBJ) start.o
	$(CROSS)ar cru dietlibc.a $(DIETLIBC_OBJ)

LIBLATIN1_OBJS=$(patsubst liblatin1/%.c,%.o,$(wildcard liblatin1/*.c))
liblatin1.a: $(LIBLATIN1_OBJS)
	$(CROSS)ar cru $@ $^

libdietc.so: dietlibc.a
	$(CROSS)ld -whole-archive -shared -o $@ $^

$(SYSCALLOBJ): syscalls.h

elftrunc: contrib/elftrunc.c start.o dietlibc.a
	$(CROSS)$(CC) -Iinclude $(CFLAGS) -nostdlib -o $@ $^

djb: compile load

compile:
	echo 'exec gcc $(CFLAGS) -I$(PWD)/include -c $${1+"$$@"}' > $@
	chmod 755 $@

load:
	echo 'main="$$1"; shift; exec gcc -nostdlib -o "$$main" $(PWD)/start.o "$$main".o $${1+"$$@"} $(PWD)/dietlibc.a -lgcc'  > $@
	chmod 755 $@

clean:
	rm -f *.o *.a t t1 compile load elftrunc exports mapfile libdietc.so
	$(MAKE) -C examples clean

tar: clean
	rm -f armv4l
	ln -sf arm armv4l
	cd ..; tar cvvf dietlibc.tar.bz2 dietlibc --use=bzip2 --exclude CVS

exports: dietlibc.a
	nm -g dietlibc.a | grep -w T | awk '{ print $$3 }' | sort -u > exports

.PHONY: t t1
t:
	$(CROSS)$(CC) -g $(CFLAGS) -fno-builtin -nostdlib -Iinclude -o t t.c start.o dietlibc.a -lgcc -Wl,-Map,mapfile

t1:
	$(CROSS)$(CC) -g -o t1 t.c

install:
	cp start.o $(INSTALLPREFIX)$(prefix)/lib/dietstart.o
	cp dietlibc.a $(INSTALLPREFIX)$(prefix)/lib/libdietc.a

.PHONY: sparc ppc mips arm alpha i386

arm sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t libdietc.so

mips:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t libdietc.so



# these depend on dietfeatures.h for large file backward compatibility
__fstat64.o __lstat64.o __stat64.o: dietfeatures.h

# these depend on dietfeatures.h for thread support
alloc.o perror.o: dietfeatures.h

# these depend on dietfeatures.h for linker warnings
assert_fail.o sprintf.o vsnprintf.o ___div.o fflush.o setvbuf.o system.o sendfile.o: dietfeatures.h

# these depend on dietfeatures.h for buffered stdio
fclose.o fdglue.o fflush.o fgetc.o fputc.o fread.o fseek.o: dietfeatures.h
printf.o setvbuf.o stderr.o stdin.o stdout.o fwrite.o: dietfeatures.h

# these depend on dietfeatures.h for fast string routines
strcasecmp.o strcat.o strchr.o strcmp.o strcpy.o strlen.o: dietfeatures.h
strncasecmp.o strncat.o strrchr.o: dietfeatures.h

# these depend on dietfeatures.h for /proc
tty.o: dietfeatures.h

# these depend on dietfeatures.h for ungetc support ;-)
ungetc.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_TZFILE_PARSER
localtime_r.o: dietfeatures.h
