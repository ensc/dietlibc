#DIET=diet
CC=gcc
CFLAGS=-Wall -fno-builtin

all: stringtest

TESTPROGRAMS=memccpy memchr memcmp memcpy memrchr strcasecmp strcmp strlen strncat strncpy strrchr strstr \
strspn strcspn strpbrk

stringtest: $(TESTPROGRAMS)

%: %.c
	$(DIET) $(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TESTPROGRAMS)

