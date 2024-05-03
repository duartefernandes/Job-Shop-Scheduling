#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <time.h>

#define MAX_JOBS 1000
#define MAX_MACHINES 1000
#define BILLION 1000000000.0

typedef struct {
    int machine;
    int time;
    int scheduling;
} Task;

#endif