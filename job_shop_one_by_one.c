/*
 * One-by-One Job Shop Scheduling Program
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
    for (int job = 0; job < totalJobs; job++) {
        for (int task = 0; task < totalMachines; task++) {
            int machineIndex = tasks[job][task].machine;
            int timeUnits = tasks[job][task].time;

            int schedule = 0;

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

void writeResult(const char* filename, double executionTime, int totalMakespan) {
    // Write the result to the console
    puts("--- Result:");
    for (int i = 0; i < totalJobs; i++) {
        printf("Job%d : ", i);
        for (int j = 0; j < totalMachines; j++) {
            printf("(M%2d, %2d) -> %2d ", tasks[i][j].machine, tasks[i][j].time, tasks[i][j].scheduling);
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
            fprintf(outputFile, "%2d ", tasks[job][task].scheduling);
        }
        fprintf(outputFile, "\n");
    }

    fprintf(outputFile, "\nTotal makespan: %d\n", totalMakespan);
    fprintf(outputFile, "Execution time: %.6f s\n", executionTime);

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
