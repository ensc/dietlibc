#ifndef __GETOPT_H__
#define __GETOPT_H__

extern int optind,opterr;
extern char *optarg;
int getopt(int argc, char *argv[], char *options);

#endif
