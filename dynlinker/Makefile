#CFLAGS = -g
CFLAGS = -Os # -fomit-frame-pointer

DYNLINK = diet-linux.so

all: $(DYNLINK)

DLIBS   = ../bin-i386/libdl.a ../bin-i386/dietlibc.a
LINKOPS = -Wl,-Ttext=0x1080,-N,-eldso_start

$(DYNLINK): ldso_start.S ldso_test.c $(DLIBS)
	$(CC) -I.. -I../include $(CFLAGS) -c ldso_test.c
	gcc -nostdlib $(CFLAGS) -o $@ ldso_start.S ldso_test.o $(DLIBS) -lgcc $(LINKOPS)
	strip -R .comment -R .note $@

install: $(DYNLINK)
	cp $(DYNLINK) /lib

clean:
	$(RM) *.o *.a *.so *.out *~
