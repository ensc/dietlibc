# local "test" make file

CC  = gcc
DCC = diet $(CC)

SRC = $(wildcard *.c) # test/test.c
OBJ = _dl_jump.o $(SRC:.c=.o)

#CFLAGS = -Wall -O2
CFLAGS = -Wall -O2 -D__OD_CLEAN_ROOM -DNODIETREF -fPIC -frerun-loop-opt -fschedule-insns2 # -fomit-frame-pointer

%.o : %.S
	$(DCC) $(CFLAGS) -c -o $@ $^

%.o : %.c
	$(DCC) $(CFLAGS) -c -o $@ $^

it: _dl_main.o

all: test.out libdl.a

libdl.a: $(OBJ)
	ar cru $@ $^

test.g: test/test.c
	gcc -o $@ $^ -ldl

test.out: $(OBJ) test/test.o
	diet gcc -o $@ $^

clean:
	$(RM) *.o *.a *.out *~ # *.so
