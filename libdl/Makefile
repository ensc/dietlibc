# local "test" make file

SRC = $(wildcard *.c) # test/test.c
OBJ = _dl_jump.o $(SRC:.c=.o)

#CFLAGS = -Wall -O2
CFLAGS = -Wall -O2 -D__OD_CLEAN_ROOM -DNODIETREF -fPIC -fomit-frame-pointer

%.o : %.S
	diet gcc $(CFLAGS) -c -o $@ $^

%.o : %.c
	diet gcc $(CFLAGS) -c -o $@ $^

it: _dl_main.o

all: test.out libdl.a

libdl.a: $(OBJ)
	ar cru $@ $^

test.g: test/test.c
	gcc -o $@ $^ -ldl

test.out: $(OBJ) test/test.o
	$(HOME)/bin/diet gcc -o $@ $^

clean:
	$(RM) *.o *.a *.out *~ # *.so
