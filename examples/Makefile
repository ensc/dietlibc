sln: sln.o ../dietlibc.a
	gcc -nostdlib -o sln ../start.o sln.o ../dietlibc.a
	strip -R .note -R .comment sln

sln.o: sln.c
	gcc -pipe -O2 -c sln.c

clean:
	rm -f sln.o sln
