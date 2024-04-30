/*
 * Sequential Job Shop Scheduling Program
 * MEI 2023/2024 - CAD
 * Authors: Duarte Fernandes (14858), Rafael Azevedo (21197)
 * Date: 05-05-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int totalJobs = 0, totalMachines = 0;

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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s Examples/<input_file> Results/<output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    readInput(argv[1]);

    double initTime = getClock();
    scheduleJobs();
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
