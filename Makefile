ARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')

OBJDIR=bin-$(ARCH)

all: $(OBJDIR) $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a $(OBJDIR)/liblatin1.a $(OBJDIR)/elftrunc $(OBJDIR)/diet

CFLAGS=-pipe
CROSS=

CC=gcc

VPATH=lib:libstdio:libugly:libcruft:libcrypt:libshell:liblatin1:syscalls.c

SYSCALLOBJ=$(patsubst syscalls.s/%.S,$(OBJDIR)/%.o,$(wildcard syscalls.s/*.S))

LIBOBJ=$(patsubst lib/%.c,$(OBJDIR)/%.o,$(wildcard lib/*.c))
LIBUGLYOBJ=$(patsubst libugly/%.c,$(OBJDIR)/%.o,$(wildcard libugly/*.c))
LIBSTDIOOBJ=$(patsubst libstdio/%.c,$(OBJDIR)/%.o,$(wildcard libstdio/*.c))
LIBCRUFTOBJ=$(patsubst libcruft/%.c,$(OBJDIR)/%.o,$(wildcard libcruft/*.c))
LIBCRYPTOBJ=$(patsubst libcrypt/%.c,$(OBJDIR)/%.o,$(wildcard libcrypt/*.c))
LIBSHELLOBJ=$(patsubst libshell/%.c,$(OBJDIR)/%.o,$(wildcard libshell/*.c))

include $(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

#CFLAGS = -g
CFLAGS += -Wall

PWD=$(shell pwd)

.SUFFIXES:
.SUFFIXES: .S .c

$(OBJDIR):
	mkdir $@

% :: %,v

$(OBJDIR)/%.o: %.S
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	$(CROSS)$(CC) -I. -Iinclude $(CFLAGS) -c $< -o $@
	$(CROSS)strip -x -R .comment -R .note $@

DIETLIBC_OBJ = $(SYSCALLOBJ) $(LIBOBJ) $(LIBSTDIOOBJ) $(LIBUGLYOBJ) \
$(LIBCRUFTOBJ) $(LIBCRYPTOBJ) $(LIBSHELLOBJ) \
$(OBJDIR)/__longjmp.o $(OBJDIR)/setjmp.o $(OBJDIR)/unified.o \
$(OBJDIR)/mmap.o $(OBJDIR)/clone.o

$(OBJDIR)/dietlibc.a: $(DIETLIBC_OBJ) $(OBJDIR)/start.o
	$(CROSS)ar cru $@ $(DIETLIBC_OBJ)

LIBLATIN1_OBJS=$(patsubst liblatin1/%.c,$(OBJDIR)/%.o,$(wildcard liblatin1/*.c))
$(OBJDIR)/liblatin1.a: $(LIBLATIN1_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR)/libdietc.so: $(OBJDIR)/dietlibc.a
	$(CROSS)ld -whole-archive -shared -o $@ $^

$(SYSCALLOBJ): syscalls.h

$(OBJDIR)/elftrunc: contrib/elftrunc.c $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a
	$(CROSS)$(CC) -Iinclude $(CFLAGS) -nostdlib -o $@ $^

$(OBJDIR)/diet: diet.c $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a
	$(CROSS)$(CC) -Iinclude $(CFLAGS) -nostdlib -o $@ $^ -DDIETHOME=\"$(PWD)\"
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

install:
	cp start.o $(INSTALLPREFIX)$(prefix)/lib/dietstart.o
	cp dietlibc.a $(INSTALLPREFIX)$(prefix)/lib/libdietc.a

.PHONY: sparc ppc mips arm alpha i386

arm sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t bin-$@/libdietc.so

mips:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t bin-$@/libdietc.so

cross:
	$(MAKE) arm sparc ppc alpha i386 mips


# these depend on dietfeatures.h for large file backward compatibility
__fstat64.o __lstat64.o __stat64.o: dietfeatures.h

# these depend on dietfeatures.h for thread support
alloc.o perror.o logging.o unified.o: dietfeatures.h

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
