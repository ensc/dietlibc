#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int
main (int argc, char *argv[])
{
  DIR * dirp;
  off_t save3 = 0;
  off_t cur;
  int i = 0;
  int result = 0;
  struct dirent *dp;
  char const *test_entry = NULL;

  dirp = opendir (".");
  for (dp = readdir (dirp); dp != NULL; dp = readdir (dirp))
    {
      /* save position 3 (after fourth entry) */
      switch (i) {
      case 3:
	save3 = telldir (dirp);
        break;
      case 4:
        test_entry = strdup(dp->d_name);
        break;
      }

      ++i;

      if (i < 10)
        printf ("%s\n", dp->d_name);

      /* stop at 400 (just to make sure dirp->__offset and dirp->__size are
	 scrambled */
      if (i == 400)
	break;
    }

  printf ("going back past 4-th entry...\n");

  /* go back to saved entry */
  seekdir (dirp, save3);

  /* Check whether telldir equals to save3 now.  */
  cur = telldir (dirp);
  if (cur != save3)
    {
      fprintf(stderr, "seekdir (d, %lld); telldir (d) == %lld\n",
              (long long)save3, (long long)cur);
      result = 1;
    }

  /* print remaining files (3-last) */
  i = 0;
  for (dp = readdir (dirp); dp != NULL; dp = readdir (dirp)) {
    if (i == 0 && strcmp(dp->d_name, test_entry) != 0) {
      fprintf(stderr, "fs entry mismatch: '%s' vs. '%s'\n",
              dp->d_name, test_entry);
      result = 1;
    }
    if (i < 10)
      printf ("%s\n", dp->d_name);
    ++i;
  }


  closedir (dirp);
  return result;
}
