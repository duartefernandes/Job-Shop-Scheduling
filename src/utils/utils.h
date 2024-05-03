#ifndef UTILS_H
#define UTILS_H

#include "../data_structs.h"

extern Task tasks[MAX_JOBS][MAX_MACHINES];
extern int totalJobs, totalMachines;

double getClock();
int calculateMakespan();

#endif