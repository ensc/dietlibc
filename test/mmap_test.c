
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define FILENAME   "/tmp/zz_temp_mmap_test"
#define TESTSTRING "This is a test string"


int main (int argc, char * argv[])
{
   int fd;
   void *filememory_1;
   void *filememory_2;
   
   fd = open (FILENAME, O_RDWR | O_CREAT, 0600);
   
   if (fd < 0)
   {
      fprintf (stderr, "Couldn't open %s for writing\n", FILENAME);
      return (1);
   }

   unlink (FILENAME);

   write (fd, TESTSTRING, sizeof(TESTSTRING));
   lseek(fd,64*1024,SEEK_SET);
   write(fd,"fnord",5);

   /*
      Try mmapping the newly created file...
   */

   filememory_1 = mmap (NULL, 0x0100, PROT_READ, MAP_PRIVATE, fd, 0);
   
   if (filememory_1 == (void *) -1)
   {
      perror("mmap returned error");
      return (1);
   }

   /*
      Try mmapping with a bogus file descriptor... (should fail)
   */

   filememory_2 = mmap (NULL, 0x0100, PROT_READ, MAP_PRIVATE, fd+10, 0);
   
   if ((filememory_2 != (void *) -1) || (errno != 9))
   {
      fprintf (stderr, "mmap allowed a bogus file descriptor...\n");
      return (1);
   }

   /*
      Check that we can read back from the file OK
   */

   if ((*(unsigned char *) filememory_1) != TESTSTRING[0])
   {
      fprintf (stderr, "mmap doesn't give expected data...\n");
      return (1);
   }

   {
     char* c=mmap(NULL,5,PROT_READ,MAP_PRIVATE,fd,64*1024);
     if (c == MAP_FAILED) {
       perror("mmap failed");
       return 1;
     }
     if (memcmp(c,"fnord",5)) {
       fprintf(stderr,"page offset didn't work");
       return 1;
     }
   }
   
   close (fd);

   /*
      Clean up.
   */

   return (0);
}


