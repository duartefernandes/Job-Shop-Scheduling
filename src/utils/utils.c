#include "utils.h"

double getClock() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / BILLION;
}

int calculateMakespan() {
    int totalMakespan = 0;

    // Calculate total makespan
    for (int i = 0; i < totalJobs; i++) {
        for (int j = 0; j < totalMachines; j++) {
            totalMakespan = (totalMakespan > (tasks[i][j].scheduling + tasks[i][j].time))
                ? totalMakespan
                : (tasks[i][j].scheduling + tasks[i][j].time);
        }
    }

    return totalMakespan;
}