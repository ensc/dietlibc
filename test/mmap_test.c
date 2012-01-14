#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

#define TESTSTRING0 "This is a test string"
#define TESTSTRING1 "Another string for testing"


int main (int argc, char * argv[])
{
   int fd;
   void *filememory_1;
   void *filememory_2;
   void *filememory_3;
   unsigned int pg_sz = sysconf(_SC_PAGESIZE);
   char fname[] = "/tmp/test-mmap.XXXXXX";

   fd = mkstemp(fname);
   if (fd < 0)
   {
      fprintf (stderr, "Couldn't open %s for writing\n", fname);
      return (1);
   }

   unlink(fname);

   write (fd, TESTSTRING0, sizeof TESTSTRING0);

   ftruncate(fd, pg_sz);
   lseek(fd, pg_sz, SEEK_SET);
   write(fd, TESTSTRING1, sizeof TESTSTRING1);

   ftruncate(fd, 2*pg_sz);

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
      Test mapping at a given offset
    */

   filememory_3 = mmap (NULL, 0x0100, PROT_READ, MAP_PRIVATE, fd, pg_sz);

   if (filememory_3 == (void *) -1)
   {
      perror("mmap (pg_sz) returned error");
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

   if (memcmp(filememory_1, TESTSTRING0, sizeof TESTSTRING0) != 0)
   {
      fprintf (stderr, "mmap doesn't give expected data...\n");
      return (1);
   }

   if (memcmp(filememory_3, TESTSTRING1, sizeof TESTSTRING1) != 0)
   {
      fprintf (stderr, "mmap (pg_sz) doesn't give expected data...\n");
      return (1);
   }

   if (munmap(filememory_3, 0x0100) < 0 ||
       munmap(filememory_1, 0x0100) < 0)
   {
      perror("munmap()");
      return 1;
   }

   /*
      Clean up.
   */
   close (fd);

   return (0);
}


