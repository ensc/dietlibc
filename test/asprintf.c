#define _GNU_SOURCE
#include <stdio.h>

int main(void){
char *path;
asprintf(&path, "/proc" "/%d/stat", 1);
return printf("%s\n",path);
}

