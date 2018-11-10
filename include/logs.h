#ifndef LOGS_H
#define LOGS_H
#include <stdio.h>

#define err(...) fprintf(stderr, __VA_ARGS__)
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define log(...) fprintf(stdout, __VA_ARGS__)

#endif
