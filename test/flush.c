#include <stdio.h>

main() {
  FILE* f=fopen("testing","w");
  fputs("testing",f);
}
