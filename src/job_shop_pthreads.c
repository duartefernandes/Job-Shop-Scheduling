/*
 * Parallel Job Shop Scheduling Program
 * MEI 2023/2024 - CAD
 * Authors: Duarte Fernandes (14858), Rafael Azevedo (21197)
 * Date: 05-05-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "io/io.h"
#include "utils/utils.h"

Task tasks[MAX_JOBS][MAX_MACHINES];
long long machineAvailability[MAX_MACHINES] = {0};

pthread_mutex_t machine_mutexes[MAX_MACHINES];

long long totalJobs = 0, totalMachines = 0, threadCount = 0;

void* scheduleJobs(void* thread) {
    long long threadIndex = *(long long*)thread;
    long long jobsPerThread = totalJobs / threadCount;
    long long remainderJobs = totalJobs % threadCount;

    long long firstJobIndex = threadIndex * jobsPerThread + (threadIndex < remainderJobs ? threadIndex : remainderJobs);
    long long lastJobIndex = firstJobIndex + jobsPerThread + (threadIndex < remainderJobs ? 1 : 0);

    for (long long job = firstJobIndex; job < lastJobIndex; job++)
    {
        for (long long task = 0; task < totalMachines; task++) {
            int machineIndex = tasks[job][task].machine;
            int timeUnits = tasks[job][task].time;

            // Simulate work
            long long counter = 0;
            for (long long i = 0; i < 100000; ++i) {
                counter += i;
            }

            long long schedule = 0;

            // Verify if it's the first task of the job
            long long previousTaskEnd = tasks[job][task - 1].scheduling + tasks[job][task - 1].time;

            if (previousTaskEnd == 0) {
                pthread_mutex_lock(&machine_mutexes[machineIndex]);
                // The first task of each job is scheduled based on the machine availability
                schedule = machineAvailability[machineIndex];
            } else {
                pthread_mutex_lock(&machine_mutexes[machineIndex]);
                // Compare the end time of the previous operation with the availability of the machine
                schedule = (previousTaskEnd > machineAvailability[machineIndex])
                    ? previousTaskEnd
                    : machineAvailability[machineIndex];
            }

            machineAvailability[machineIndex] = schedule + timeUnits;
            pthread_mutex_unlock(&machine_mutexes[machineIndex]);

            tasks[job][task].scheduling = schedule;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <thread_count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    readInput(argv[1]);

    long long requestedThreads = strtol(argv[3], NULL, 10);
    // Use the smaller of totalJobs or requestedThreads to avoid unnecessary threads
    threadCount = (requestedThreads > totalJobs) ? totalJobs : requestedThreads;

    double initTime = getClock();

    pthread_t threads[threadCount];
    static long long threadIndices[MAX_JOBS];

    for (long long i = 0; i < totalMachines; i++) {
        pthread_mutex_init(&machine_mutexes[i], NULL);
    }

    for (long long i = 0; i < threadCount; i++) {
        threadIndices[i] = i;
        pthread_create(&threads[i], NULL, scheduleJobs, &threadIndices[i]);
    }

    // Wait for all threads to finish
    for (long long i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    for (long long i = 0; i < totalMachines; i++) {
        pthread_mutex_destroy(&machine_mutexes[i]);
    }

    double endTime = getClock();

    long long totalMakespan = calculateMakespan();

    writeResult(argv[2], endTime - initTime, totalMakespan);

    return 0;
}
