# local "test" make file

SRC = $(wildcard *.c) # test/test.c
OBJ = _dl_jump.o $(SRC:.c=.o)

#CFLAGS = -Wall -O2
CFLAGS = -Wall -g

%.o : %.S
	$(HOME)/bin/diet gcc $(CFLAGS) -c -o $@ $^

%.o : %.c
	$(HOME)/bin/diet gcc $(CFLAGS) -c -o $@ $^

all: test.out libdl.a

libdl.a: $(OBJ)
	ar cru $@ $^

test.g: test/test.c
	gcc -o $@ $^ -ldl

test.out: $(OBJ) test/test.o
	$(HOME)/bin/diet gcc -o $@ $^

clean:
	$(RM) *.o *.a *.out *~ # *.so
