all: direnttest inettest stdiotests stdlibtests stringtests timetests \
test ffs getdelim stdlib 

DIET=diet
CC=gcc
CFLAGS=-nostdinc -Wall

LCOMPAT=-lcompat

TESTPROGRAMS=adjtime alarm argv asctime asprintf atexit atfile bsearch byteswap calloc confstr cycles empty fadvise flush fnmatch \
fputc ftruncate ftw fwrite getaddrinfo getenv getgrnam gethostbyaddr gethostbyname \
gethostbyname_r getmntent getopt getpass getpwnam getservbyname getservbyport getusershell \
glob grent hasmntopt hello iconv if_nameindex ltostr malloc-debugger math md5_testharness \
memccpy memcpy memchr memcmp memrchr memusage mktime mmap_test pipe printf printftest \
protoent prototypes putenv pwent rand48 read1 readdir readdir64 regex select sendfile servent setjmp siglist \
sigsetjmp speed spent sprintf sscanf stdarg strcasecmp strcmp strncat strncpy strptime strrchr \
strstr strtol sysconf sysenter ungetc utime waitpid

TESTPROGRAMS += startup-test

test: $(TESTPROGRAMS)

ffs: ffs.c
	$(DIET) $(CC) $(CFLAGS) $^ $(LCOMPAT) -o $@
	
getdelim: getdelim.c
	$(DIET) $(CC) $(CFLAGS) $^ $(LCOMPAT) -o $@ 

syscall: syscall.c
	$(DIET) $(CC) $^ -o $@

testandset: testandset.c
	$(DIET) $(CC) $^ -o $@ 

direnttest:
	 $(MAKE) -C dirent CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)

inettest:
	$(MAKE) -C inet CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)

stdiotests:
	$(MAKE) -C stdio CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)

stdlibtests:
	$(MAKE) -C stdlib CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)

stringtests:
	$(MAKE) -C string CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)

timetests:
	$(MAKE) -C time CC="$(CC)" CFLAGS="$(CFLAGS)" DIET=$(DIET) LCOMPAT=$(LCOMPAT)
	
	
%: %.c
	$(DIET) $(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TESTPROGRAMS) getdelim ffs
	$(MAKE) -C dirent clean
	$(MAKE) -C stdio clean
	$(MAKE) -C stdlib clean
	$(MAKE) -C string clean
	$(MAKE) -C time clean

