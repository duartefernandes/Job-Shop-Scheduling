#include "utils.h"

double getClock() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Uses CLOCK_MONOTONIC for accurate timing
    return (double)ts.tv_sec + (double)ts.tv_nsec / BILLION; // seconds with high precision
}

long long calculateMakespan() {
    long long totalMakespan = 0;

    // Calculate total makespan
    for (long long i = 0; i < totalJobs; i++) {
        for (long long j = 0; j < totalMachines; j++) {
            totalMakespan = (totalMakespan > (tasks[i][j].scheduling + tasks[i][j].time))
                ? totalMakespan
                : (tasks[i][j].scheduling + tasks[i][j].time);
        }
    }

    return totalMakespan;
}