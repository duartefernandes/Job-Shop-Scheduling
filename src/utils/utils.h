#ifndef UTILS_H
#define UTILS_H

#include "../data_structs.h"

extern Task tasks[MAX_JOBS][MAX_MACHINES];
extern long long totalJobs, totalMachines;

double getClock();
long long calculateMakespan();

#endif