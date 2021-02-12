/* Functions and definitions related to getopt */
#ifndef GETOPT_WIN
#define GETOPT_WIN
#include <stddef.h>

extern char optopt, *optarg;
extern int optind, opterr;

int getopt(int argc, char **argv, char *format);


/* Helper functions */
int isflag(char *arg);

/* Error codes */
#define NOT_FLAG 1
#define MISS_ARG 2
#define INVALID_FLAG 3

#endif
