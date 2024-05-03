#ifndef IO_H
#define IO_H

#include "../data_structs.h"

extern Task tasks[MAX_JOBS][MAX_MACHINES];
extern int machineAvailability[MAX_MACHINES];
extern int totalJobs, totalMachines, threadCount;

void readInput(const char* filename);
void writeResult(const char* filename, double executionTime, int totalMakespan);

#endif