all: start.o dietlibc.a elftrunc

ARCH=$(shell uname -m | sed 's/i[4-9]86/i386/')

CFLAGS=-pipe
CROSS=

VPATH=syscalls.c

SYSCALLOBJ=$(patsubst syscalls.s/%.S,%.o,$(wildcard syscalls.s/*.S))

LIBOBJ=$(patsubst lib/%.c,%.o,$(wildcard lib/*.c))

include $(ARCH)/Makefile.add

ifeq ($(CFLAGS),-pipe)
CFLAGS+=-O -fomit-frame-pointer
endif

#CFLAGS += -g
CFLAGS += -Wall

.SUFFIXES:
.SUFFIXES: .S .c

%.c : %,v

%.o: %.S
	$(CROSS)gcc -I. -Iinclude $(CFLAGS) -c $<

%.o: %.c
	$(CROSS)gcc -I. -Iinclude $(CFLAGS) -c $<
#	$(CROSS)strip -x -R .comment -R .note $@

DIETLIBC_OBJ = $(SYSCALLOBJ) $(LIBOBJ) \
__longjmp.o setjmp.o unified.o mmap.o clone.o

dietlibc.a: $(DIETLIBC_OBJ) start.o
	$(CROSS)ar cru dietlibc.a $(DIETLIBC_OBJ)

libdietc.so: dietlibc.a
	$(CROSS)ld -whole-archive -shared -o $@ $^

$(SYSCALLOBJ): syscalls.h

elftrunc: contrib/elftrunc.c start.o dietlibc.a
	$(CROSS)gcc $(CFLAGS) -nostdlib -o $@ $^

djb: compile load

compile:
	echo 'exec gcc $(CFLAGS) -I$(HOME)/projects/dietlibc/include -c $${1+"$$@"}' > $@
	chmod 755 $@

load:
	echo 'main="$$1"; shift; exec gcc -nostdlib -o "$$main" $(HOME)/projects/dietlibc/start.o "$$main".o $${1+"$$@"} $(HOME)/projects/dietlibc/dietlibc.a -lgcc'  > $@
	chmod 755 $@

clean:
	rm -f *.o *.a t t1 compile load elftrunc exports mapfile libdietc.so
	$(MAKE) -C examples clean

tar: clean
	cd .. && tar cvvf dietlibc.tar.bz2 dietlibc --use=bzip2

exports: dietlibc.a
	nm -g dietlibc.a | grep -w T | awk '{ print $$3 }' | sort -u > exports

.PHONY: t t1
t:
	$(CROSS)gcc -g $(CFLAGS) -fno-builtin -nostdlib -Iinclude -o t t.c start.o dietlibc.a -lgcc -Wl,-Map,mapfile

t1:
	$(CROSS)gcc -g -o t1 t.c

install:
	cp start.o $(prefix)/lib/dietstart.o
	cp dietlibc.a $(prefix)/lib/libdietc.a

.PHONY: sparc ppc mips arm alpha i386

sparc ppc alpha i386:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all t libdietc.so

mips arm:
	$(MAKE) ARCH=$@ CROSS=$@-linux-gnu- all t libdietc.so

