INSTALL=install
prefix?=/opt/diet
# Set the following to install to a different root
#DESTDIR=/tmp/fefix
# Use "make DEBUG=1" to compile a debug version.

LIBDIR=${prefix}/lib
BINDIR=${prefix}/bin
MAN1DIR=${prefix}/man/man1

EXTRACFLAGS=

MYARCH:=$(shell uname -m | sed -e 's/i[4-9]86/i386/' -e 's/armv[3-7]t\?[eh]\?j\?[lb]/arm/')

# This extra-ugly cruft is here so make will not run uname and sed each
# time it looks at $(OBJDIR).  This alone sped up running make when
# nothing has to be done from 1 sec to 0.12 sec on a 900 MHz Athlon.
# We don't use ARCH:=$(MYARCH) so we can detect unknown architectures.
ifeq ($(MYARCH),i386)
ARCH=i386
else
ifeq ($(MYARCH),mips)
ARCH=mips
else
ifeq ($(MYARCH),alpha)
ARCH=alpha
else
ifeq ($(MYARCH),ppc)
ARCH=ppc
else
ifeq ($(MYARCH),ppc64)
ARCH=ppc64
else
ifeq ($(MYARCH),ppc64le)
ARCH=ppc64le
else
ifeq ($(MYARCH),arm)
ARCH=arm
else
ifeq ($(MYARCH),aarch64)
ARCH=aarch64
else
ifeq ($(MYARCH),sparc)
ARCH=sparc
else
ifeq ($(MYARCH),sparc64)
ARCH=sparc64
else
ifeq ($(MYARCH),s390)
ARCH=s390
else
ifeq ($(MYARCH),s390x)
ARCH=s390x
else
ifeq ($(MYARCH),mipsel)
ARCH=mipsel
else
ifeq ($(MYARCH),mips64)
ARCH=mips64
else
ifeq ($(MYARCH),parisc)
ARCH=parisc
else
ifeq ($(MYARCH),parisc64)
ARCH=parisc
MYARCH=parisc
else
ifeq ($(MYARCH),x86_64)
ARCH=x86_64
else
ifeq ($(MYARCH),ia64)
ARCH=ia64
else
ifeq ($(MYARCH),armeb)
ARCH=arm
else
$(error unknown architecture, please fix Makefile)
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

PIE=-fpie

# ARCH=$(MYARCH)

OBJDIR=bin-$(ARCH)
ILIBDIR=$(LIBDIR)-$(ARCH)

DIETHOME=$(shell pwd)

WHAT=	$(OBJDIR) $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o $(OBJDIR)/dyn_stop.o \
	$(OBJDIR)/dietlibc.a $(OBJDIR)/liblatin1.a \
	$(OBJDIR)/libcompat.a $(OBJDIR)/libm.a \
	$(OBJDIR)/librpc.a $(OBJDIR)/libpthread.a \
	$(OBJDIR)/libcrypt.a $(OBJDIR)/stackgap-g.o \
	$(OBJDIR)/diet $(OBJDIR)/diet-i $(OBJDIR)/elftrunc \
	$(OBJDIR)/dnsd

all: $(WHAT)

profiling: $(OBJDIR)/libgmon.a $(OBJDIR)/pstart.o

DEFAULTCFLAGS=-pipe -nostdinc -D_REENTRANT $(EXTRACFLAGS)
CFLAGS=$(DEFAULTCFLAGS)
CROSS=

CC=gcc
CCC=$(CROSS)$(CC)
STRIP=$(COMMENT) $(CROSS)strip
INC=-I. -isystem include
#INC=-I. -Iinclude

VPATH=lib:libstdio:libugly:libcruft:libcrypt:libshell:liblatin1:libcompat:libdl:librpc:libregex:libm:profiling

SYSCALLOBJ=$(patsubst syscalls.s/%.S,$(OBJDIR)/%.o,$(sort $(wildcard syscalls.s/*.S)))

LIBOBJ=$(patsubst lib/%.c,$(OBJDIR)/%.o,$(sort $(wildcard lib/*.c)))
LIBUGLYOBJ=$(patsubst libugly/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libugly/*.c)))
LIBSTDIOOBJ=$(patsubst libstdio/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libstdio/*.c)))
LIBCRUFTOBJ=$(patsubst libcruft/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libcruft/*.c)))
LIBCRYPTOBJ=$(patsubst libcrypt/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libcrypt/*.c)))
LIBSHELLOBJ=$(patsubst libshell/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libshell/*.c)))
LIBCOMPATOBJ=$(patsubst libcompat/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libcompat/*.c))) $(OBJDIR)/syscall.o
LIBMATH=$(patsubst libm/%.c,%.o,$(sort $(wildcard libm/*.c)))

LIBRPCOBJ=$(patsubst librpc/%.c,$(OBJDIR)/%.o,$(sort $(wildcard librpc/*.c)))
LIBREGEXOBJ=$(patsubst libregex/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libregex/*.c)))

LIBDLOBJ=$(patsubst libdl/%.c,$(OBJDIR)/%.o,$(sort $(wildcard libdl/*.c))) $(OBJDIR)/_dl_jump.o

LIBPTHREAD_OBJS=$(patsubst libpthread/%.c,$(OBJDIR)/%.o,$(sort $(shell ./threadsafe.sh))) $(OBJDIR)/__testandset.o

LIBGMON_OBJS=$(OBJDIR)/__mcount.o $(OBJDIR)/monitor.o $(OBJDIR)/profil.o

include $(ARCH)/Makefile.add

LIBMATHOBJ=$(patsubst %,$(OBJDIR)/%,$(LIBMATH))

ifeq ($(CFLAGS),$(DEFAULTCFLAGS))
CFLAGS+=-O -fomit-frame-pointer
endif

ifneq ($(DEBUG),)
CFLAGS = -g $(EXTRACFLAGS)
STRIP = :
endif
CFLAGS += -W -Wall -Wextra -Wchar-subscripts -Wmissing-prototypes -Wmissing-declarations -Wno-switch -Wno-unused -Wredundant-decls -Wshadow

ASM_CFLAGS = -Wa,--noexecstack
ifneq ($(subst clang,fnord,$(CC)),$(CC))
ASM_CFLAGS += -fno-integrated-as
endif

ifeq ($(ALWAYS_PIC),1)
CCFLAGS=$(CFLAGS) $(PIE)
else
CCFLAGS=$(CFLAGS)
endif

PWD=$(shell pwd)

.SUFFIXES:
.SUFFIXES: .S .c

# added real dynamic dietlibc.so
PICODIR = pic-$(ARCH)

$(OBJDIR) $(PICODIR):
	mkdir $@

% :: %,v

ifeq ($(CC),tcc)
$(OBJDIR)/%.o: %.S $(ARCH)/syscalls.h | $(OBJDIR)
	$(CROSS)cpp $(INC) $< | $(CROSS)as --noexecstack -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	tcc -I. -Iinclude -c $< -o $@
	-$(STRIP) -x -R .comment -R .note $@
else
$(OBJDIR)/pstart.o: start.S | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -DPROFILING -c $< $(ASM_CFLAGS) -o $@

$(OBJDIR)/%.o: %.S $(ARCH)/syscalls.h | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -c $< $(ASM_CFLAGS) -o $@

$(OBJDIR)/pthread_%.o: libpthread/pthread_%.c | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -c $< -o $@
	-$(STRIP) -x -R .comment -R .note $@
$(OBJDIR)/stack_smash_handler2.o:	EXTRACFLAGS:=-fno-omit-frame-pointer

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -c $< -o $@ -D__dietlibc__
	-$(STRIP) -x -R .comment -R .note $@
endif



ifeq ($(shell $(CC) -v 2>&1 | grep "gcc version"),gcc version 4.0.0)
SAFE_CFLAGS=$(shell echo $(CCFLAGS)|sed 's/-Os/-O2/')
SAFER_CFLAGS=$(shell echo $(CCFLAGS)|sed 's/-Os/-O/')
else
SAFE_CFLAGS=$(CCFLAGS)
SAFER_CFLAGS=$(CCFLAGS)
endif

CC+=-D__dietlibc__

$(OBJDIR)/start-pie.o: start.S | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -c $< $(ASM_CFLAGS) -fpie -o $@

$(OBJDIR)/start.o: start.S | $(OBJDIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -c $< $(ASM_CFLAGS) -fno-pie -o $@


$(OBJDIR)/crypt.o: libcrypt/crypt.c | $(OBJDIR)
	$(CCC) $(INC) $(SAFER_CFLAGS) $(EXTRACFLAGS) -c $< -o $@

DIETLIBC_OBJ = $(OBJDIR)/unified.o \
$(SYSCALLOBJ) $(LIBOBJ) $(LIBSTDIOOBJ) $(LIBUGLYOBJ) \
$(LIBCRUFTOBJ) $(LIBCRYPTOBJ) $(LIBSHELLOBJ) $(LIBREGEXOBJ) \
$(OBJDIR)/__longjmp.o $(OBJDIR)/setjmp.o \
$(OBJDIR)/clone.o

$(OBJDIR)/dietlibc.a: $(DIETLIBC_OBJ) $(OBJDIR)/start.o
	$(CROSS)ar cru $@ $(DIETLIBC_OBJ)

$(OBJDIR)/librpc.a: $(LIBRPCOBJ)
	$(CROSS)ar cru $@ $(LIBRPCOBJ)

$(OBJDIR)/libcrypt.a: | $(OBJDIR)
	touch dummy.c
	$(CCC) -c dummy.c
	$(CROSS)ar cru $@ dummy.o
	rm -f dummy.c dummy.o

dummy.o:

LIBLATIN1_OBJS=$(patsubst liblatin1/%.c,$(OBJDIR)/%.o,$(sort $(wildcard liblatin1/*.c)))
$(OBJDIR)/liblatin1.a: $(LIBLATIN1_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR)/libgmon.a: $(LIBGMON_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR)/libpthread.a: $(LIBPTHREAD_OBJS) dietfeatures.h
	$(CROSS)ar cru $@ $(LIBPTHREAD_OBJS)

$(OBJDIR)/libcompat.a: $(LIBCOMPATOBJ)
	$(CROSS)ar cru $@ $(LIBCOMPATOBJ)

$(OBJDIR)/libm.a: $(LIBMATHOBJ)
	$(CROSS)ar cru $@ $(LIBMATHOBJ)

$(OBJDIR)/seekdir.o $(OBJDIR)/clnt_raw.o $(OBJDIR)/clnt_udp.o \
$(PICODIR)/seekdir.o $(PICODIR)/clnt_raw.o $(PICODIR)/clnt_udp.o \
$(OBJDIR)/sha256crypt.o $(OBJDIR)/sha512crypt.o \
$(PICODIR)/sha256crypt.o $(PICODIR)/sha512crypt.o: CFLAGS+=-fno-strict-aliasing

LD_UNSET = env -u LD_RUN_PATH

dyn: dyn_lib

# added dynamic linker
$(OBJDIR)/libdl.a: $(LIBDLOBJ)
	$(CROSS)ar cru $@ $(LIBDLOBJ)

dyn_lib: $(PICODIR) $(PICODIR)/libc.so $(PICODIR)/dstart.o \
	$(PICODIR)/dyn_so_start.o $(PICODIR)/dyn_start.o $(PICODIR)/dyn_stop.o \
	$(PICODIR)/libpthread.so $(PICODIR)/libdl.so $(PICODIR)/libcompat.so \
	$(PICODIR)/libm.so $(PICODIR)/diet-dyn $(PICODIR)/diet-dyn-i

$(PICODIR)/%.o: %.S $(ARCH)/syscalls.h | $(PICODIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -fPIC -D__DYN_LIB $(ASM_CFLAGS) -c $< -o $@

$(PICODIR)/pthread_%.o: libpthread/pthread_%.c | $(PICODIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -fPIC -D__DYN_LIB -c $< -o $@
	$(STRIP) -x -R .comment -R .note $@

$(PICODIR)/%.o: %.c | $(PICODIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -fPIC -D__DYN_LIB -c $< -o $@
	$(STRIP) -x -R .comment -R .note $@

$(PICODIR)/dstart.o: start.S | $(PICODIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -fPIC -D__DYN_LIB $(ASM_CFLAGS) -c $< -o $@

$(PICODIR)/dyn_so_start.o: dyn_start.c | $(PICODIR)
	$(CCC) $(INC) $(CCFLAGS) $(EXTRACFLAGS) -fPIC -D__DYN_LIB -D__DYN_LIB_SHARED -c $< -o $@
	$(STRIP) -x -R .comment -R .note $@

DYN_LIBC_PIC = $(LIBOBJ) $(LIBSTDIOOBJ) $(LIBUGLYOBJ) \
$(LIBCRUFTOBJ) $(LIBCRYPTOBJ) $(LIBSHELLOBJ) $(LIBREGEXOBJ)

DYN_LIBC_OBJ = $(PICODIR)/dyn_syscalls.o $(PICODIR)/errlist.o \
	$(patsubst $(OBJDIR)/%.o,$(PICODIR)/%.o,$(DYN_LIBC_PIC))

DYN_PTHREAD_OBJS = $(patsubst $(OBJDIR)/%.o,$(PICODIR)/%.o,$(LIBPTHREAD_OBJS))

DYN_LIBDL_OBJS = $(patsubst $(OBJDIR)/%.o,$(PICODIR)/%.o,$(LIBDLOBJ))

DYN_LIBCOMPAT_OBJS = $(patsubst $(OBJDIR)/%.o,$(PICODIR)/%.o,$(LIBCOMPATOBJ))

DYN_LIBMATH_OBJS = $(patsubst $(OBJDIR)/%.o,$(PICODIR)/%.o,$(LIBMATHOBJ))

$(PICODIR)/libc.so: $(DYN_LIBC_OBJ)
	$(LD_UNSET) $(CCC) -nostdlib -shared -o $@ $(CCFLAGS) -fPIC $(DYN_LIBC_OBJ) -lgcc -Wl,-soname=libc.so

$(PICODIR)/libpthread.so: $(DYN_PTHREAD_OBJS) dietfeatures.h $(PICODIR)/libc.so
	$(LD_UNSET) $(CCC) -nostdlib -shared -o $@ $(CCFLAGS) -fPIC $(DYN_PTHREAD_OBJS) -L$(PICODIR) -lc -Wl,-soname=libpthread.so

$(PICODIR)/libdl.so: libdl/_dl_main.c dietfeatures.h $(PICODIR)/libc.so
	$(LD_UNSET) $(CCC) -D__OD_CLEAN_ROOM -DNODIETREF -fPIC -nostdlib -shared -Bsymbolic -Wl,-Bsymbolic \
		-o $@ $(SAFE_CFLAGS) $(INC) libdl/_dl_main.c -Wl,-soname=libdl.so

$(OBJDIR)/pthread_create.o $(PICODIR)/pthread_create.o: dietfeatures.h
$(OBJDIR)/pthread_internal.o $(PICODIR)/pthread_internal.o: dietfeatures.h

#$(PICODIR)/libdl.so: $(DYN_LIBDL_OBJS) dietfeatures.h
#	$(CCC) -nostdlib -shared -o $@ $(CCFLAGS) -fPIC $(DYN_LIBDL_OBJS) -L$(PICODIR) -ldietc -Wl,-soname=libdl.so

$(PICODIR)/libcompat.so: $(DYN_LIBCOMPAT_OBJS) dietfeatures.h $(PICODIR)/libc.so
	$(LD_UNSET) $(CCC) -nostdlib -shared -o $@ $(CCFLAGS) -fPIC $(DYN_LIBCOMPAT_OBJS) -L$(PICODIR) -lc -Wl,-soname=libcompat.so

$(PICODIR)/libm.so: $(DYN_LIBMATH_OBJS) dietfeatures.h $(PICODIR)/libc.so
	$(LD_UNSET) $(CCC) -nostdlib -shared -o $@ $(CCFLAGS) -fPIC $(DYN_LIBMATH_OBJS) -L$(PICODIR) -lc -Wl,-soname=libm.so


$(SYSCALLOBJ): syscalls.h

$(OBJDIR)/elftrunc: $(OBJDIR)/diet contrib/elftrunc.c
	bin-$(MYARCH)/diet $(CCC) $(CFLAGS) -o $@ contrib/elftrunc.c

$(OBJDIR)/dnsd: $(OBJDIR)/diet contrib/dnsd.c
	bin-$(MYARCH)/diet $(CCC) $(CFLAGS) -o $@ contrib/dnsd.c

VERSION=dietlibc-$(shell head -n 1 CHANGES|sed 's/://')
CURNAME=$(notdir $(shell pwd))

$(OBJDIR)/diet: $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o diet.c $(OBJDIR)/dietlibc.a $(OBJDIR)/dyn_stop.o
	$(CCC) -isystem include $(CFLAGS) -nostdlib -o $@ $^ -DDIETHOME=\"$(DIETHOME)\" -DVERSION=\"$(VERSION)\" -lgcc
	$(STRIP) -R .comment -R .note $@

$(OBJDIR)/diet-i: $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o diet.c $(OBJDIR)/dietlibc.a $(OBJDIR)/dyn_stop.o
	$(CCC) -isystem include $(CFLAGS) -nostdlib -o $@ $^ -DDIETHOME=\"$(prefix)\" -DVERSION=\"$(VERSION)\" -DINSTALLVERSION -lgcc
	$(STRIP) -R .comment -R .note $@

$(PICODIR)/diet-dyn: $(PICODIR)/start.o $(PICODIR)/dyn_start.o diet.c
	$(LD_UNSET) $(CCC) -isystem include $(CFLAGS) -fPIC -nostdlib -o $@ $^ -DDIETHOME=\"$(DIETHOME)\" -D__DYN_LIB -DVERSION=\"$(VERSION)\" -L$(PICODIR) -lc -lgcc $(PICODIR)/dyn_stop.o -Wl,-dynamic-linker=$(DIETHOME)/$(PICODIR)/libdl.so
	$(STRIP) -R .command -R .note $@

$(PICODIR)/diet-dyn-i: $(PICODIR)/start.o $(PICODIR)/dyn_start.o diet.c
	$(LD_UNSET) $(CCC) -isystem include $(CFLAGS) -fPIC -nostdlib -o $@ $^ -DDIETHOME=\"$(prefix)\" -D__DYN_LIB -DVERSION=\"$(VERSION)\" -L$(PICODIR) -lc -lgcc $(PICODIR)/dyn_stop.o -Wl,-dynamic-linker=$(ILIBDIR)/libdl.so -DINSTALLVERSION
	$(STRIP) -R .command -R .note $@

$(OBJDIR)/djb: $(OBJDIR)/compile $(OBJDIR)/load

$(OBJDIR)/compile:
	echo 'exec' $(CC) '$(CCFLAGS) -I$(PWD)/$(OBJDIR)/include -c $${1+"$$@"}' > $@
	chmod 755 $@

$(OBJDIR)/load:
	echo 'main="$$1"; shift; exec' $(CC) '-nostdlib -o "$$main" $(PWD)/$(OBJDIR)/start.o "$$main".o $${1+"$$@"} $(PWD)/$(OBJDIR)/dietlibc.a -lgcc'  > $@
	chmod 755 $@

clean:
	rm -f *.o *.a t t1 compile load elftrunc exports mapfile libdietc.so include/errno_definition.h
	rm -rf bin-* pic-*
	$(MAKE) -C examples clean
	$(MAKE) -C dynlinker clean
	$(MAKE) -C libdl clean

tar: clean rename
	cd ..; tar cvvf $(VERSION).tar.bz2 --use=bzip2 --exclude CVS $(VERSION)

rename:
	if test $(CURNAME) != $(VERSION); then cd .. && mv $(CURNAME) $(VERSION); fi

$(OBJDIR)/exports: $(OBJDIR)/dietlibc.a
	nm -g $(OBJDIR)/dietlibc.a | grep '\<T\>' | awk '{ print $$3 }' | sort -u > $(OBJDIR)/exports

.PHONY: t t1
t:
	$(CCC) -g $(CFLAGS) -fno-builtin -nostdlib -isystem include -o t t.c $(OBJDIR)/start.o $(OBJDIR)/dyn_start.o $(OBJDIR)/dietlibc.a -lgcc $(OBJDIR)/dyn_stop.o -Wl,-Map,mapfile

t1:
	$(CCC) -g -o t1 t.c

install-bin: $(OBJDIR)/start.o $(OBJDIR)/dietlibc.a $(OBJDIR)/librpc.a $(OBJDIR)/liblatin1.a $(OBJDIR)/libcompat.a $(OBJDIR)/elftrunc $(OBJDIR)/diet-i $(OBJDIR)/stackgap-g.o
	$(INSTALL) -d $(DESTDIR)$(ILIBDIR) $(DESTDIR)$(MAN1DIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 644 $(OBJDIR)/start.o $(DESTDIR)$(ILIBDIR)/
	-$(INSTALL) -m 644 $(OBJDIR)/start-pie.o $(DESTDIR)$(ILIBDIR)/
	-$(INSTALL) -m 644 $(OBJDIR)/stackgap-g.o $(DESTDIR)$(ILIBDIR)/
	$(INSTALL) -m 644 $(OBJDIR)/dyn_start.o $(OBJDIR)/dyn_stop.o $(DESTDIR)$(ILIBDIR)/
	$(INSTALL) -m 644 $(OBJDIR)/libm.a $(OBJDIR)/libpthread.a $(OBJDIR)/librpc.a \
$(OBJDIR)/liblatin1.a $(OBJDIR)/libcompat.a $(OBJDIR)/libcrypt.a $(DESTDIR)$(ILIBDIR)
	$(INSTALL) -m 644 $(OBJDIR)/dietlibc.a $(DESTDIR)$(ILIBDIR)/libc.a
ifeq ($(MYARCH),$(ARCH))
	$(INSTALL) $(OBJDIR)/diet-i $(DESTDIR)$(BINDIR)/diet
	-$(INSTALL) $(PICODIR)/diet-dyn-i $(DESTDIR)$(BINDIR)/diet-dyn
endif
	$(INSTALL) -m 755 $(OBJDIR)/elftrunc $(OBJDIR)/dnsd $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 644 diet.1 $(DESTDIR)$(MAN1DIR)/diet.1

install-profiling:
	-$(INSTALL) $(OBJDIR)/pstart.o $(OBJDIR)/libgmon.a $(DESTDIR)$(ILIBDIR)

install-pic:
	-$(INSTALL) $(PICODIR)/libc.so $(DESTDIR)$(ILIBDIR)/libc.so
	-$(INSTALL) $(PICODIR)/libpthread.so $(DESTDIR)$(ILIBDIR)/libpthread.so
	-$(INSTALL) $(PICODIR)/libdl.so $(DESTDIR)$(ILIBDIR)/libdl.so
	-$(INSTALL) $(PICODIR)/libcompat.so $(DESTDIR)$(ILIBDIR)/libcompat.so
	-$(INSTALL) $(PICODIR)/libm.so $(DESTDIR)$(ILIBDIR)/libm.so
	-$(INSTALL) $(PICODIR)/dyn_start.o $(DESTDIR)$(ILIBDIR)/dyn_dstart.o
	-$(INSTALL) $(PICODIR)/dyn_stop.o  $(DESTDIR)$(ILIBDIR)/dyn_dstop.o
	-$(INSTALL) $(PICODIR)/dstart.o $(PICODIR)/dyn_so_start.o $(DESTDIR)$(ILIBDIR)
	if test -f $(PICODIR)/libc.so -a ! -f $(DESTDIR)/etc/diet.ld.conf; then echo "$(ILIBDIR)" > $(DESTDIR)/etc/diet.ld.conf; fi

install-headers:
	for i in `find include -name \*.h`; do install -m 644 -D $$i $(DESTDIR)$(prefix)/$$i; done


install: install-bin install-profiling install-pic install-headers

uninstall:
	for i in start.o libm.a libpthread.a librpc.a liblatin1.a libcompat.a libcrypt.a libc.a; do rm -f $(DESTDIR)$(ILIBDIR)/$$i; done
	rm -f $(DESTDIR)$(BINDIR)/diet $(DESTDIR)$(BINDIR)/diet-dyn
	for i in libgmon.a dyn_start.o dyn_stop.o libc.so libpthread.so libdl.so libcompat.so dyn_dstart.o dyn_dstop.o dyn_so_start.o; do rm -f $(DESTDIR)$(ILIBDIR)/$$i; done
	rm -f $(DESTDIR)$(MAN1DIR)/diet.1 $(DESTDIR)/etc/diet.ld.conf
	for i in `find include -name \*.h`; do rm -f $(DESTDIR)$(prefix)/$$i; done
	-rmdir $(DESTDIR)$(ILIBDIR) $(DESTDIR)$(MAN1DIR) $(DESTDIR)$(BINDIR)

.PHONY: sparc ppc mips arm alpha i386 parisc mipsel powerpc s390 sparc64
.PHONY: x86_64 ia64 ppc64 s390x

arm sparc alpha mips parisc s390 sparc64 x86_64 ia64 ppc64 s390x:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all

.PHONY: x32
x32:
ifeq ($(MYARCH),x86_64)
	$(MAKE) ARCH=$@ CC="$(CC) -mx32" all
else
	$(MAKE) ARCH=$@ CROSS=x86_64-linux- CC="$(CC) -mx32" all
endif

i386:
ifeq ($(MYARCH),x86_64)
	$(MAKE) ARCH=$@ CC="$(CC) -m32" all
else
	$(MAKE) ARCH=$@ CROSS=$@-linux- all
endif

ppc:
ifeq ($(MYARCH),ppc64)
	$(MAKE) ARCH=$@ CC="$(CC) -m32" all
else
	$(MAKE) ARCH=$@ CROSS=$@-linux- all
endif

# Cross compile for little endian MIPS
mipsel:
	$(MAKE) ARCH=$@ CROSS=mips-linux- all

mips-gnu:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all

# Some people named their cross compiler toolchain powerpc-linux-gcc
powerpc:
	$(MAKE) ARCH=ppc CROSS=powerpc-linux- all

hppa:
	ln -sf bin-parisc bin-hppa
	$(MAKE) ARCH=parisc CROSS=hppa-linux- all

CROSS_ARCH=arm sparc ppc alpha i386 mips sparc64 x86_64 s390 parisc
cross:
	$(MAKE) $(subst $(ARCH),,$(CROSS_ARCH))

# DOES NOT WORK YET
mips64:
	$(MAKE) ARCH=mips64 CROSS=mips64-linux- CC="gcc -mabi=64"


# these depend on dietfeatures.h for large file backward compatibility
$(OBJDIR)/__fstat64.o $(OBJDIR)/__lstat64.o $(OBJDIR)/__stat64.o $(OBJDIR)/lseek64.o $(OBJDIR)/readdir64.o $(OBJDIR)/stat64.o $(OBJDIR)/lstat64.o $(OBJDIR)/fstat64.o $(OBJDIR)/truncate64.o $(OBJDIR)/__truncate64.o $(OBJDIR)/ftruncate64.o $(OBJDIR)/__ftruncate64.o $(OBJDIR)/sendfile64.o $(OBJDIR)/__sendfile64.o $(PICODIR)/dyn_syscalls.o $(PICODIR)/__truncate64.o $(PICODIR)/__ftruncate64.o $(PICODIR)/__stat64.o $(PICODIR)/__lstat64.o $(PICODIR)/__fstat64.o $(OBJDIR)/__sendfile64.o $(OBJDIR)/fstatfs64.o $(OBJDIR)/statfs64.o: dietfeatures.h

# these depend on dietfeatures.h for thread support
$(OBJDIR)/alloc.o $(OBJDIR)/perror.o $(OBJDIR)/logging.o $(OBJDIR)/unified.o $(OBJDIR)/clone.o $(OBJDIR)/set_errno.o: dietfeatures.h

# these depend on dietfeatures.h for linker warnings
$(OBJDIR)/assert_fail.o $(OBJDIR)/sprintf.o $(OBJDIR)/vsnprintf.o $(OBJDIR)/___div.o $(OBJDIR)/fflush.o $(OBJDIR)/setvbuf.o $(OBJDIR)/system.o $(OBJDIR)/sendfile.o $(OBJDIR)/setenv.o: dietfeatures.h

# these depend on dietfeatures.h for buffered stdio
fclose.o $(OBJDIR)/fdglue.o $(OBJDIR)/fflush.o $(OBJDIR)/fgetc.o $(OBJDIR)/fputc.o $(OBJDIR)/fread.o $(OBJDIR)/fseek.o $(OBJDIR)/printf.o $(OBJDIR)/setvbuf.o $(OBJDIR)/stderr.o $(OBJDIR)/stdin.o $(OBJDIR)/stdout.o $(OBJDIR)/fwrite.o $(OBJDIR)/puts.o: dietfeatures.h

# these depend on dietfeatures.h for fast string routines
strcasecmp.o $(OBJDIR)/strcat.o $(OBJDIR)/strchr.o $(OBJDIR)/strcmp.o $(OBJDIR)/strcpy.o $(OBJDIR)/strlen.o $(OBJDIR)/strncasecmp.o $(OBJDIR)/strncat.o $(OBJDIR)/strrchr.o $(OBJDIR)/memchr.o: dietfeatures.h

# these depend on dietfeatures.h for /proc
$(OBJDIR)/ttyname.o $(OBJDIR)/sysconf_cpus.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_TZFILE_PARSER
$(OBJDIR)/localtime_r.o $(OBJDIR)/strftime.o $(OBJDIR)/tzfile.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_SMALL_STDIO_BUFS
$(LIBSTDIOOBJ): dietfeatures.h include/stdio.h dietstdio.h

# these depend on dietfeatures.h for WANT_FULL_RESOLV_CONF
$(OBJDIR)/dnscruft.o $(OBJDIR)/dnscruft2.o $(OBJDIR)/res_search.o $(OBJDIR)/res_query.o: dietfeatures.h

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
$(OBJDIR)/netent.o $(OBJDIR)/system.o $(OBJDIR)/stdin.o $(OBJDIR)/stdout.o \
$(OBJDIR)/stderr.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_I386_SOCKETCALL
$(OBJDIR)/__bind.o $(OBJDIR)/__connect.o $(OBJDIR)/__listen.o \
$(OBJDIR)/__accept.o $(OBJDIR)/__getsockname.o $(OBJDIR)/__getpeername.o \
$(OBJDIR)/__socketpair.o $(OBJDIR)/__send.o $(OBJDIR)/__recv.o \
$(OBJDIR)/__sendto.o $(OBJDIR)/__recvfrom.o $(OBJDIR)/__shutdown.o \
$(OBJDIR)/__setsockopt.o $(OBJDIR)/__getsockopt.o $(OBJDIR)/__sendmsg.o \
$(OBJDIR)/__recvmsg.o $(OBJDIR)/__accept4.o $(OBJDIR)/__recvmmsg.o \
$(OBJDIR)/__sendmmsg.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_CRYPT_MD5
$(OBJDIR)/crypt.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_FREAD_OPTIMIZATION
$(OBJDIR)/fread.o $(OBJDIR)/fwrite.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_DYNAMIC
$(OBJDIR)/start.o $(OBJDIR)/dyn_start.o $(OBJDIR)/dyn_stop.o: dietfeatures.h

$(OBJDIR)/unified.o: dietuglyweaks.h

$(OBJDIR)/adjtimex.o: include/sys/timex.h

$(OBJDIR)/fgetc_unlocked.o $(OBJDIR)/fread.o $(OBJDIR)/ungetc.o: dietstdio.h

# these depend on dietfeatures.h for WANT_LINKER_WARNINGS
$(OBJDIR)/setlinebuf.o $(OBJDIR)/bzero.o $(OBJDIR)/setegid.o \
$(OBJDIR)/seteuid.o $(OBJDIR)/toascii.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_FULL_POSIX_COMPAT
$(OBJDIR)/strncpy.o $(OBJDIR)/stpncpy.o: dietfeatures.h
$(OBJDIR)/strxfrm.o: dietfeatures.h

$(OBJDIR)/stat.o $(OBJDIR)/fstat.o $(OBJDIR)/lstat.o: include/sys/stat.h
$(OBJDIR)/stat64.o $(OBJDIR)/fstat64.o $(OBJDIR)/lstat64.o: include/sys/stat.h

# these depend on dietfeatures.h for WANT_INET_ADDR_DNS
$(OBJDIR)/gethostbyname_r.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_VALGRIND_SUPPORT
bin-x86_64/strlen.o bin-x86_64/memchr.o bin-x86_64/gettimeofday.o: dietfeatures.h
bin-x32/strlen.o bin-x32/memchr.o bin-x32/gettimeofday.o: dietfeatures.h

# WANT_PLUGPLAY_DNS
$(OBJDIR)/getaddrinfo.o: dietfeatures.h

$(OBJDIR)/strsignal.o: include/signal.h

$(LIBPTHREAD_OBJS): include/pthread.h

# CFLAGS+=-W -Wshadow -Wid-clash-31 -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings

# WANT_LARGEFILE_BACKCOMPAT
$(OBJDIR)/fcntl64.o: dietfeatures.h

# WANT_SSP
# This facepalm brought to you by: Ubuntu!
$(PICODIR)/stackgap.o: EXTRACFLAGS:=-fno-stack-protector
$(OBJDIR)/stackgap.o: EXTRACFLAGS:=-fno-stack-protector -fno-pie -DNDEBUG
$(OBJDIR)/stackgap-pie.o: EXTRACFLAGS:=-fno-stack-protector -Dstackgap=stackgap_pie -fpie

$(OBJDIR)/stackgap-g.o: EXTRACFLAGS:=-fno-stack-protector -fno-pie
$(OBJDIR)/__get_elf_aux_value.o $(PICODIR)/__get_elf_aux_value.o: EXTRACFLAGS+=-fno-stack-protector

$(OBJDIR)/stackgap.o $(OBJDIR)/stackgap-pie.o $(OBJDIR)/stackgap-g.o $(PICODIR)/stackgap.o: lib/stackgap-common.h

# WANT_MALLOC_ZERO
$(OBJDIR)/strndup.o: dietfeatures.h

# dietdirent.h dependencies
$(OBJDIR)/closedir.o $(OBJDIR)/fdopendir.o $(OBJDIR)/ftw.o $(OBJDIR)/ftw64.o $(OBJDIR)/opendir.o $(OBJDIR)/readdir.o $(OBJDIR)/readdir64.o $(OBJDIR)/readdir_r.o $(OBJDIR)/rewinddir.o $(OBJDIR)/seekdir.o $(OBJDIR)/telldir.o $(OBJDIR)/dirfd.o: dietdirent.h

$(OBJDIR)/thrd_%.o: include/thread.h

$(OBJDIR)/__fcntl64.o $(OBJDIR)/__fstat64.o $(OBJDIR)/__fstatfs64.o $(OBJDIR)/__ftruncate64.o \
$(OBJDIR)/__getcwd.o $(OBJDIR)/__lstat64.o $(OBJDIR)/__mmap.o $(OBJDIR)/__ptrace.o \
$(OBJDIR)/__sendfile64.o $(OBJDIR)/__stat64.o $(OBJDIR)/__statfs64.o $(OBJDIR)/__stime.o \
$(OBJDIR)/__truncate64.o $(OBJDIR)/__utmp_io.o $(OBJDIR)/__v_printf.o $(OBJDIR)/addmntent.o \
$(OBJDIR)/alloc.o $(OBJDIR)/bindresvport.o $(OBJDIR)/cfsetispeed.o $(OBJDIR)/cfsetospeed.o \
$(OBJDIR)/clnt_generic.o $(OBJDIR)/clnt_tcp.o $(OBJDIR)/clnt_udp.o $(OBJDIR)/cnd_timedwait.o \
$(OBJDIR)/cnd_wait.o $(OBJDIR)/confstr.o $(OBJDIR)/dnscruft2.o $(OBJDIR)/dnsd.o \
$(OBJDIR)/err.o $(OBJDIR)/errno_location.o $(OBJDIR)/errx.o $(OBJDIR)/execl.o \
$(OBJDIR)/execle.o $(OBJDIR)/execlp.o $(OBJDIR)/execv.o $(OBJDIR)/execvp.o \
$(OBJDIR)/fdglue.o $(OBJDIR)/fdglue2.o $(OBJDIR)/fdopen.o $(OBJDIR)/freopen.o \
$(OBJDIR)/ftell.o $(OBJDIR)/fwrite.o $(OBJDIR)/getdelim.o $(OBJDIR)/gethostbyaddr.o \
$(OBJDIR)/gethostbyaddr_r.o $(OBJDIR)/gethostbyname.o $(OBJDIR)/gethostbyname2.o \
$(OBJDIR)/gethostbyname2_r.o $(OBJDIR)/gethostbyname_r.o $(OBJDIR)/gethostent.o \
$(OBJDIR)/getpass.o $(OBJDIR)/glob.o $(OBJDIR)/herrno_location.o $(OBJDIR)/iconv.o \
$(OBJDIR)/iconv_open.o $(OBJDIR)/inet_pton.o $(OBJDIR)/isatty.o $(OBJDIR)/lockf.o \
$(OBJDIR)/logging.o $(OBJDIR)/lseek64.o $(OBJDIR)/mbrlen.o $(OBJDIR)/mbrtowc.o \
$(OBJDIR)/mkdtemp.o $(OBJDIR)/mkstemp.o $(OBJDIR)/mktemp.o $(OBJDIR)/mmap64.o \
$(OBJDIR)/mtx_lock.o $(OBJDIR)/mtx_timedlock.o $(OBJDIR)/mtx_trylock.o \
$(OBJDIR)/mtx_unlock.o $(OBJDIR)/netent.o $(OBJDIR)/nice.o $(OBJDIR)/openpty.o \
$(OBJDIR)/perror.o $(OBJDIR)/pmap_getmaps.o $(OBJDIR)/pmap_rmt.o \
$(OBJDIR)/pthread_atfork.o $(OBJDIR)/pthread_attr_getdetachstate.o \
$(OBJDIR)/pthread_attr_getinheritsched.o $(OBJDIR)/pthread_attr_getschedparam.o \
$(OBJDIR)/pthread_attr_getschedpolicy.o $(OBJDIR)/pthread_attr_getscope.o \
$(OBJDIR)/pthread_attr_getstackaddr.o $(OBJDIR)/pthread_attr_getstacksize.o \
$(OBJDIR)/pthread_attr_setdetachstate.o $(OBJDIR)/pthread_attr_setinheritsched.o \
$(OBJDIR)/pthread_attr_setschedparam.o $(OBJDIR)/pthread_attr_setschedpolicy.o \
$(OBJDIR)/pthread_attr_setscope.o $(OBJDIR)/pthread_attr_setstackaddr.o \
$(OBJDIR)/pthread_attr_setstacksize.o $(OBJDIR)/pthread_cleanup.o \
$(OBJDIR)/pthread_cond_broadcast.o $(OBJDIR)/pthread_cond_destroy.o \
$(OBJDIR)/pthread_cond_init.o $(OBJDIR)/pthread_cond_signal.o \
$(OBJDIR)/pthread_cond_timedwait.o $(OBJDIR)/pthread_cond_wait.o \
$(OBJDIR)/pthread_condattr_getshared.o $(OBJDIR)/pthread_condattr_setshared.o \
$(OBJDIR)/pthread_create.o $(OBJDIR)/pthread_detach.o $(OBJDIR)/pthread_fdglue2.o \
$(OBJDIR)/pthread_internal.o $(OBJDIR)/pthread_join.o $(OBJDIR)/pthread_key.o \
$(OBJDIR)/pthread_mutex_destroy.o $(OBJDIR)/pthread_mutex_init.o \
$(OBJDIR)/pthread_mutex_lock.o $(OBJDIR)/pthread_mutex_trylock.o \
$(OBJDIR)/pthread_mutex_unlock.o $(OBJDIR)/pthread_mutexattr_getkind_np.o \
$(OBJDIR)/pthread_mutexattr_init.o $(OBJDIR)/pthread_mutexattr_setkind_np.o \
$(OBJDIR)/pthread_semaphore_destroy.o $(OBJDIR)/pthread_semaphore_getvalue.o \
$(OBJDIR)/pthread_semaphore_init.o $(OBJDIR)/pthread_semaphore_misc.o \
$(OBJDIR)/pthread_semaphore_post.o $(OBJDIR)/pthread_semaphore_trywait.o \
$(OBJDIR)/pthread_semaphore_wait.o $(OBJDIR)/pthread_setcancelstate.o \
$(OBJDIR)/pthread_setschedparam.o $(OBJDIR)/pthread_spinlock.o \
$(OBJDIR)/putenv.o $(OBJDIR)/putpwent.o $(OBJDIR)/readdir64.o \
$(OBJDIR)/realpath.o $(OBJDIR)/remove.o $(OBJDIR)/res_mkquery.o \
$(OBJDIR)/res_query.o $(OBJDIR)/res_search.o $(OBJDIR)/sigaddset.o \
$(OBJDIR)/sigdelset.o $(OBJDIR)/sigismember.o $(OBJDIR)/signalfd.o \
$(OBJDIR)/strerror.o $(OBJDIR)/strtol.o $(OBJDIR)/strtoll.o \
$(OBJDIR)/strtoul.o $(OBJDIR)/strtoull.o $(OBJDIR)/svc.o \
$(OBJDIR)/svc_run.o $(OBJDIR)/svc_tcp.o $(OBJDIR)/svc_udp.o \
$(OBJDIR)/sysconf.o $(OBJDIR)/sysconf_cpus.o $(OBJDIR)/system.o \
$(OBJDIR)/tcflow.o $(OBJDIR)/tcsendbreak.o $(OBJDIR)/tcsetattr.o \
$(OBJDIR)/tempnam.o $(OBJDIR)/thrd_exit.o $(OBJDIR)/thrd_join.o \
$(OBJDIR)/tmpnam.o $(OBJDIR)/utxent.o $(OBJDIR)/verr.o \
$(OBJDIR)/verrx.o $(OBJDIR)/vwarn.o $(OBJDIR)/warn.o \
$(OBJDIR)/wcsrtombs.o $(OBJDIR)/wcstombs.o $(OBJDIR)/eventfd.o: include/errno_definition.h

$(OBJDIR)/errno_location.o $(OBJDIR)/errno.o: dietfeatures.h

$(OBJDIR)/abort.o $(OBJDIR)/pselect.o $(OBJDIR)/__utmp_block_signals.o \
$(OBJDIR)/system.o $(OBJDIR)/utxent.o $(OBJDIR)/sigemptyset.o \
$(OBJDIR)/sigorset.o $(OBJDIR)/sigandset.o $(OBJDIR)/sigfillset.o \
$(OBJDIR)/sigisemptyset.o: include/signal.h

include/errno_definition.h: dietfeatures.h
	if grep -q '^#define WANT_TLS' $<; then echo "extern __thread int errno;"; else echo "extern int errno;"; fi > $@

ldso: ldso.c
	gcc -nostdlib -shared -g -DIN_LDSO -Iinclude.ldso -I. -isystem include x86_64/start.S -o ldso ldso.c -fPIC x86_64/dyn_syscalls.S lib/errno_location.c -D__thread=

GIT_CVSIMPORT=git cvsimport
CVS_EXTRA_bigo.ensc.de=;proxy=www-cache;proxyport=3128
CVS_EXTRA=$(CVS_EXTRA_$(shell hostname -d))
cvsimport:
	$(GIT_CVSIMPORT) -k -p '--cvs-direct' -d ':pserver$(CVS_EXTRA):cvs@cvs.fefe.de:/cvs' dietlibc
