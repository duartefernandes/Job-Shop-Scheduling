#include "io.h"
#include <stdio.h>
#include <stdlib.h>

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

    if (totalJobs <= 50)
    {
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

    printf("\nTotal jobs: %d\n", totalJobs);
    printf("Total machines: %d\n", totalMachines);
}

void writeResult(const char* filename, double executionTime, long long totalMakespan) {
    if (totalJobs <= 50)
    {
        // Write the result to the console
        puts("--- Result:");
        for (long long i = 0; i < totalJobs; i++) {
            printf("Job%lldd : ", i);
            for (long long j = 0; j < totalMachines; j++) {
                printf("(M%d - %d) -> %lld", tasks[i][j].machine, tasks[i][j].time, tasks[i][j].scheduling);
                if (j + 1 < totalMachines)
                {
                    printf(" , ");
                }
            }
            printf("\n");
        }
    }

    printf("\nTotal makespan: %lld\n", totalMakespan);
    printf("Execution time: %.6f s\n", executionTime);

    // Write the result to the file on the requested structure
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int job = 0; job < totalJobs; job++) {
        for (int task = 0; task < totalMachines; task++) {
            fprintf(outputFile, "%lld", tasks[job][task].scheduling);
            if (task + 1 < totalMachines)
            {
                fprintf(outputFile, " ");
            }
        }
        fprintf(outputFile, "\n");
    }

    fprintf(outputFile, "\nMakespan: %lld\n", totalMakespan);
    fprintf(outputFile, "Execution time (including threads creation): %.6f s", executionTime);

    fclose(outputFile);
}