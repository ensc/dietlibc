#include <stdio.h>
#include <mntent.h>

int endmntent(FILE *filep) {
  return fclose(filep);
}

