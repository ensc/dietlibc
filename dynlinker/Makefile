#CFLAGS = -g
CFLAGS = -Os # -fomit-frame-pointer

DYNLINK = /tmp/test.so

all: $(DYNLINK)

$(DYNLINK): ldso_start.S ldso_test.c ../libdl/libdl.a
	$(CC) -I.. -I../include $(CFLAGS) -c ldso_test.c
	gcc -nostdlib $(CFLAGS) -o $@ \
		ldso_start.S ldso_test.o ../libdl/libdl.a ../bin-i386/dietlibc.a -lgcc \
		-Wl,-Ttext=0x1080,-N,-eldso_start
	strip -R .comment -R .note $@

clean:
	$(RM) *.o *.a *.so *.out *~ /tmp/test.so
