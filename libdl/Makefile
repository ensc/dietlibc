# local "test" make file

SRC = $(wildcard *.c) test/test.c
OBJ = _dl_jump.o $(SRC:.c=.o)

#CFLAGS = -Wall -O2
CFLAGS = -g

%.o : %.S
	$(HOME)/bin/diet gcc $(CFLAGS) -c -o $@ $^

%.o : %.c
	$(HOME)/bin/diet gcc $(CFLAGS) -c -o $@ $^

all: test.out

test.g: test.c
	gcc -o $@ $^ -ldl

test.out: $(OBJ)
	$(HOME)/bin/diet gcc -o $@ $^

clean:
	$(RM) *.o *.a *.out *~ # *.so
