/*
 * Parallel Job Shop Scheduling Program
 * MEI 2023/2024 - CAD
 * Authors: Duarte Fernandes (14858), Rafael Azevedo (21197)
 * Date: 05-05-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_JOBS 1000
#define MAX_MACHINES 1000
#define getClock() ((double)clock() / CLOCKS_PER_SEC)

typedef struct {
    int machine;
    int time;
    int scheduling;
} Task;

Task tasks[MAX_JOBS][MAX_MACHINES];
int machineAvailability[MAX_MACHINES] = {0};

pthread_mutex_t machine_mutexes[MAX_MACHINES];

int totalJobs = 0, totalMachines = 0, threadCount = 0;

void readInput(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", &totalJobs, &totalMachines);

    for (int i = 0; i < totalJobs; i++) {
        for (int j = 0; j < totalMachines; j++) {
            fscanf(file, "%d %d", &tasks[i][j].machine, &tasks[i][j].time);
        }
    }

    fclose(file);

    // Print the interpreted data to the console
    puts("--- Interpreted Data:");
    for (int i = 0; i < totalJobs; i++) {
        printf("Job%d -> ", i);
        for (int j = 0; j < totalMachines; j++) {
            printf("(M%d, %2d) ", tasks[i][j].machine, tasks[i][j].time);
        }
        printf("\n");
    }
}

void* scheduleJobs(void* thread) {
    int threadIndex = *(int*)thread;
    int jobsPerThread = totalJobs / threadCount;
    int remainderJobs = totalJobs % threadCount;

    int firstJobIndex = threadIndex * jobsPerThread + (threadIndex < remainderJobs ? threadIndex : remainderJobs);
    int lastJobIndex = firstJobIndex + jobsPerThread + (threadIndex < remainderJobs ? 1 : 0);

    for (int job = firstJobIndex; job < lastJobIndex; job++)
    {
        for (int task = 0; task < totalMachines; task++) {
            int machineIndex = tasks[job][task].machine;
            int timeUnits = tasks[job][task].time;

            // Simulate work
            long long counter = 0;
            for (long long i = 0; i < 1000000; ++i) {
                counter += i;
            }

            int schedule = 0;

            // Compare the end time of the previous task with the availability of the machine
            int previousTaskEnd = tasks[job][task - 1].scheduling + tasks[job][task - 1].time;

            if (previousTaskEnd == 0) {
                pthread_mutex_lock(&machine_mutexes[machineIndex]);
                // The first task of each job is scheduled based on the machine availability
                schedule = machineAvailability[machineIndex];
            } else {
                pthread_mutex_lock(&machine_mutexes[machineIndex]);
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

void writeResult(const char* filename, double executionTime, int totalMakespan) {
    // Write the result to the console
    puts("--- Result:");
    for (int i = 0; i < totalJobs; i++) {
        printf("Job%d : ", i);
        for (int j = 0; j < totalMachines; j++) {
            printf("(M%d - %d) -> %d", tasks[i][j].machine, tasks[i][j].time, tasks[i][j].scheduling);
            if (j + 1 < totalMachines)
            {
                printf(" , ");
            }
        }
        printf("\n");
    }

    printf("\nTotal makespan: %d\n", totalMakespan);
    printf("Execution time: %.6f s\n", executionTime);

    // Write the result to the file on the requested structure
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int job = 0; job < totalJobs; job++) {
        for (int task = 0; task < totalMachines; task++) {
            fprintf(outputFile, "%d", tasks[job][task].scheduling);
            if (task + 1 < totalMachines)
            {
                fprintf(outputFile, " ");
            }
        }
        fprintf(outputFile, "\n");
    }

    fprintf(outputFile, "\nMakespan: %d\n", totalMakespan);
    fprintf(outputFile, "Execution time (including threads creation): %.6f s", executionTime);

    fclose(outputFile);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <thread_count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    readInput(argv[1]);

    printf("Total Jobs: %d\n", totalJobs);
    printf("Total Machines: %d\n\n", totalMachines);

    int requestedThreads = strtol(argv[3], NULL, 10);
    // Use the smaller of totalJobs or requestedThreads to avoid unnecessary threads
    threadCount = (requestedThreads > totalJobs) ? totalJobs : requestedThreads;

    double initTime = getClock();

    pthread_t threads[threadCount];
    static int threadIndices[MAX_JOBS];

    for (int i = 0; i < totalMachines; i++) {
        pthread_mutex_init(&machine_mutexes[i], NULL);
    }

    int start = 0;
    for (int i = 0; i < threadCount; i++) {
        threadIndices[i] = i;
        pthread_create(&threads[i], NULL, scheduleJobs, &threadIndices[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < totalMachines; i++) {
        pthread_mutex_destroy(&machine_mutexes[i]);
    }

    double endTime = getClock();

    int totalMakespan = 0;

    // Calculate total makespan
    for (int i = 0; i < totalJobs; i++) {
        for (int j = 0; j < totalMachines; j++) {
            totalMakespan = (totalMakespan > (tasks[i][j].scheduling + tasks[i][j].time))
                ? totalMakespan
                : (tasks[i][j].scheduling + tasks[i][j].time);
        }
    }

    writeResult(argv[2], endTime - initTime, totalMakespan);

    return 0;
}
