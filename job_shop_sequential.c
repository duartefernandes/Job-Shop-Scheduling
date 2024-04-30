/*
 * Sequential Job Shop Scheduling Program
 * MEI 2023/2024 - CAD
 * Authors: Duarte Fernandes (14858), Rafael Azevedo (21197)
 * Date: 05-05-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "makespan.h"

#define getClock() ((double)clock() / CLOCKS_PER_SEC)

Task tasks[MAX_JOBS][MAX_MACHINES];
int machineAvailability[MAX_MACHINES] = {0};

int totalJobs = 0, totalMachines = 0;

void scheduleJobs() {
    // Iterate over all jobs
    for (int job = 0; job < totalJobs; job++) {
        // Iterate over all tasks of the job
        for (int task = 0; task < totalMachines; task++) {
            int machineIndex = tasks[job][task].machine;
            int timeUnits = tasks[job][task].time;

            // Simulate work
            long long counter = 0;
            for (long long i = 0; i < 1000000; ++i) {
                counter += i;
            }

            int schedule = 0;

             if (task == 0) {
                // The first task of each job is scheduled based on the machine availability
                schedule = machineAvailability[machineIndex];
            } else {
                // Compare the end time of the previous operation with the availability of the machine
                int previousOperationEnd = tasks[job][task - 1].scheduling + tasks[job][task - 1].time;
                schedule = (previousOperationEnd > machineAvailability[machineIndex])
                    ? previousOperationEnd
                    : machineAvailability[machineIndex];
            }

            tasks[job][task].scheduling = schedule;
            machineAvailability[machineIndex] = schedule + timeUnits;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s Examples/<input_file> Results/<output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    readInput(argv[1]);

    double initTime = getClock();
    scheduleJobs();
    double endTime = getClock();

    int totalMakespan = calculateMakespan();

    writeResult(argv[2], endTime - initTime, totalMakespan);

    return 0;
}
