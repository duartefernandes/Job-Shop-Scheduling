/*
 * One-by-One Job Shop Scheduling Program
 * MEI 2023/2024 - CAD
 * Authors: Duarte Fernandes (14858), Rafael Azevedo (21197)
 * Date: 05-05-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "utils.h"

Task tasks[MAX_JOBS][MAX_MACHINES];
long long machineAvailability[MAX_MACHINES] = {0};

long long totalJobs = 0, totalMachines = 0;

void scheduleJobs() {
    for (long long job = 0; job < totalJobs; job++) {
        for (long long task = 0; task < totalMachines; task++) {
            int machineIndex = tasks[job][task].machine;
            int timeUnits = tasks[job][task].time;

            long long schedule = 0;

            if (task == 0 && job == 0) {
                schedule = 0;
            }
            // If it's the first task of a job, schedule it after the last task of the previous job
            else if (task == 0) {
                // Assuming tasks are executed sequentially, the last task of the previous job ends at the same time as the last task of the current job
                schedule = tasks[job-1][totalMachines - 1].scheduling + tasks[job-1][totalMachines - 1].time;
            }
            else {
                schedule = tasks[job][task - 1].scheduling + tasks[job][task - 1].time;
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

    long long totalMakespan = calculateMakespan();

    writeResult(argv[2], endTime - initTime, totalMakespan);

    return 0;
}
