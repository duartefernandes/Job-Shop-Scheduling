#ifndef MAKESPAN_H
#define MAKESPAN_H

#include "data_structs.h"

extern Task tasks[MAX_JOBS][MAX_MACHINES];
extern int totalJobs, totalMachines;

int calculateMakespan();

#endif