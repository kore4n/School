/*
    Title: Assignment 4
    Author: Jason Kwan
    Description: Automatically simulate multiple CPU scheduling algorithms for any given set of processes.
    Date: March 8, 2023
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PROCESSES 20

double round(double var)
{
    double value = (int)(var * 10 + .5);
    return (double)value / 10;
}

struct Pcb {
    int name;   // 0 is P0, 1 is P1, etc...
    int initialBurst;   // burst as it appears in the file
    int arrivalTime;
    int burstLeft;
    int startTime;
    int waitTime;
    int turnAroundTime;
};

// always return 0
int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4) { return 0; }    // only allow 2 or 3 command line arguments

    if (argc == 3)
    {
        if (strcmp(argv[1], "-f") == 0) // FCFS
        {
            printf("First Come First Served\n");
            char buf[999];
            FILE* ptr = fopen(argv[2], "r");
            if (ptr == NULL) {
                printf("Could not open %s\n", argv[2]);
                return 0;
            }

            int sumWaitTime = 0;
            int sumTurnAroundTime = 0;

            int totalTimeTaken = 0;
            int currentWaitTime = 0;

            int lastSumProcessBurstTime = 0;    // sum of all previous burst times

            int processesWaitAroundTime[NUM_PROCESSES] = {0};
            int processesTurnAroundTime[NUM_PROCESSES] = {0};

            // i is also equal to arrival time
            for (int i = 0; i < NUM_PROCESSES; i++) {
                fscanf(ptr, "%s", buf);

                char *processName = strtok(buf, ",");
                int processBurstTime = atoi(strtok(NULL, ","));
                int currentTurnAroundTime = lastSumProcessBurstTime - i - 1;    // equal to previous sum of burst times minus current arrival time

                // current wait time = last burst time - current arrival time
                currentWaitTime = lastSumProcessBurstTime - i;    // remember i is also arrival time

                // runs processBurstTime times
                for (int k = processBurstTime; k >= 1; k--)
                {
                    totalTimeTaken += 1;
                    currentTurnAroundTime += 1;
                    printf("T%-2d: %-3s - Burst left %2d, Wait time %2d, Turnaround time %2d\n", totalTimeTaken - 1, processName, k, currentWaitTime, currentTurnAroundTime);
                }

                currentTurnAroundTime += 1;
                processesWaitAroundTime[i] = currentWaitTime;
                processesTurnAroundTime[i] = currentTurnAroundTime;
                sumWaitTime += currentWaitTime;
                sumTurnAroundTime += currentTurnAroundTime;

                lastSumProcessBurstTime += processBurstTime;
                
            }

            printf("\n");

            for (int i = 0; i < NUM_PROCESSES; i++) {
                printf("P%d\n", i);
                printf("\tWaiting time:\t\t%d\n", processesWaitAroundTime[i]);
                printf("\tTurnaround time:\t%d\n", processesTurnAroundTime[i]);
                printf("\n");
            }

            double avgWaitTime = (double)sumWaitTime/NUM_PROCESSES;
            double avgTurnAroundTime = (double)sumTurnAroundTime/NUM_PROCESSES;

            avgWaitTime = round(avgWaitTime);
            avgTurnAroundTime = round(avgTurnAroundTime);
            printf("Total average waiting time:\t%.1f\n", avgWaitTime);
            printf("Total average turnaround time:\t%.1f\n", avgTurnAroundTime);

            return 0;
        }
        else if (strcmp(argv[1], "-s") == 0)    // SJF
        {
            printf("Shortest Job First\n");
            char buf[999];
            FILE* ptr = fopen(argv[2], "r");
            if (ptr == NULL) {
                printf("Could not open %s\n", argv[2]);
                return 0;
            }

            // i is also equal to arrival time

            struct Pcb processControlBlocks[NUM_PROCESSES]; 
            int sumBurst = 0;
            // set up all pcbs
            for (int i = 0; i < NUM_PROCESSES; i++) {
                fscanf(ptr, "%s", buf);
                strtok(buf, ",");
                int processBurstTime = atoi(strtok(NULL, ","));

                struct Pcb *new_pcb;
                new_pcb = malloc(sizeof(struct Pcb));
                new_pcb->name = i;
                new_pcb->initialBurst = processBurstTime;
                new_pcb->arrivalTime = i;
                new_pcb->turnAroundTime = 0;
                new_pcb->waitTime = 0;
                new_pcb->burstLeft = processBurstTime;
                processControlBlocks[i] = *new_pcb;
                sumBurst += processBurstTime;
            }

            // do SJF
            int PIDToRun = 0;
            int curBurstLeft = processControlBlocks[0].burstLeft;
            for (int timePassed = 0; timePassed < sumBurst; timePassed++) {
                for (int k = 1; k < NUM_PROCESSES; k++) {
                    if (processControlBlocks[k].burstLeft == 0) continue;
                    if (processControlBlocks[k].arrivalTime != timePassed) continue;    // must have just arrived to switch to new process
                    if (processControlBlocks[k].burstLeft >= curBurstLeft) continue;    // new process to switch to must have less burst left than current

                    PIDToRun = k;
                }

                printf("T%-2d: P%-2d - Burst left %2d, Wait time %2d, Turnaround time %2d\n", 
                    timePassed, 
                    processControlBlocks[PIDToRun].name, 
                    processControlBlocks[PIDToRun].burstLeft,
                    processControlBlocks[PIDToRun].waitTime, 
                    processControlBlocks[PIDToRun].turnAroundTime
                );

                processControlBlocks[PIDToRun].burstLeft -= 1; // fine to subtract after printing - never show burst left == 0

                // increase wait time of every other valid process
                for (int p = 0; p < NUM_PROCESSES; p++) {
                    if (processControlBlocks[p].burstLeft == 0) continue;
                    if (processControlBlocks[p].arrivalTime > timePassed) continue;
                    if (PIDToRun == p) continue;

                    // increase wait time of every other process
                    processControlBlocks[p].waitTime += 1;
                }

                // increase turn around time of every other valid process
                for (int p = 0; p < NUM_PROCESSES; p++) {
                    if (processControlBlocks[p].burstLeft == 0) continue;
                    if (processControlBlocks[p].arrivalTime > timePassed) continue;    // hasnt started yet

                    // process has started, increase turn around time
                    processControlBlocks[p].turnAroundTime += 1;
                }
               
                if (processControlBlocks[PIDToRun].burstLeft != 0) continue;    // process is ending

                curBurstLeft = 999;
                // choose new process to run after this one ends
                for (int k = 0; k < NUM_PROCESSES; k++) {
                    if (processControlBlocks[k].burstLeft == 0) continue;
                    if (processControlBlocks[k].arrivalTime > timePassed) continue;     // new process to switch to must have arrived
                    if (processControlBlocks[k].burstLeft >= curBurstLeft) continue;    // burst left must be less than to switch to a different process that has just arrived

                    curBurstLeft = processControlBlocks[k].burstLeft;
                    PIDToRun = k;
                }
            }

            printf("\n");

            int sumWaitTime = 0;
            int sumTurnAroundTime = 0;
            for (int i = 0; i < NUM_PROCESSES; i++) {
                processControlBlocks[i].turnAroundTime += 1;    // must add one to make calculation correct

                printf("P%d\n", i);
                printf("\tWaiting time:\t\t%d\n", processControlBlocks[i].waitTime);
                printf("\tTurnaround time:\t%d\n", processControlBlocks[i].turnAroundTime);
                printf("\n");

                sumWaitTime += processControlBlocks[i].waitTime;
                sumTurnAroundTime += processControlBlocks[i].turnAroundTime;
            }

            double avgWaitTime = (double)sumWaitTime/NUM_PROCESSES;
            double avgTurnAroundTime = (double)sumTurnAroundTime/NUM_PROCESSES;

            avgWaitTime = round(avgWaitTime);
            avgTurnAroundTime = round(avgTurnAroundTime);
            printf("Total average waiting time:\t%.1f\n", avgWaitTime);
            printf("Total average turnaround time:\t%.1f\n", avgTurnAroundTime);
        }
    }
    else if (argc == 4 && strcmp(argv[1], "-r") == 0)   // Round Robin
    {
        int timeQuantum = atoi(argv[2]);
        printf("Round Robin with Quantum %d\n", timeQuantum);
        char buf[999];
        FILE* ptr = fopen(argv[3], "r");
        if (ptr == NULL) {
            printf("Could not open %s\n", argv[3]);
            return 0;
        }


        // i is also equal to arrival time

        struct Pcb processControlBlocks[NUM_PROCESSES]; 
        int sumBurst = 0;
        // set up all pcbs
        for (int i = 0; i < NUM_PROCESSES; i++) {
            fscanf(ptr, "%s", buf);
            strtok(buf, ",");
            int processBurstTime = atoi(strtok(NULL, ","));

            struct Pcb *new_pcb;
            new_pcb = malloc(sizeof(struct Pcb));
            new_pcb->name = i;
            new_pcb->initialBurst = processBurstTime;
            new_pcb->arrivalTime = i;
            new_pcb->turnAroundTime = 0;
            new_pcb->waitTime = 0;
            new_pcb->burstLeft = processBurstTime;
            processControlBlocks[i] = *new_pcb;
            sumBurst += processBurstTime;
        }

        // do round robin 
        int PIDToRun = 0;
        int timeLeft = timeQuantum;
        for (int timePassed = 0; timePassed < sumBurst; timePassed++) {
            
            printf("T%-2d: P%-2d - Burst left %2d, Wait time %2d, Turnaround time %2d\n", 
                timePassed, 
                processControlBlocks[PIDToRun].name, 
                processControlBlocks[PIDToRun].burstLeft,
                processControlBlocks[PIDToRun].waitTime, 
                processControlBlocks[PIDToRun].turnAroundTime
            );

            processControlBlocks[PIDToRun].burstLeft -= 1;


            // increase wait time of every other valid process
            for (int p = 0; p < NUM_PROCESSES; p++) {
                if (processControlBlocks[p].burstLeft == 0) continue;
                if (processControlBlocks[p].arrivalTime > timePassed) continue;
                if (PIDToRun == p) continue;

                // increase wait time of every other process
                processControlBlocks[p].waitTime += 1;
            }

            // increase turn around time of every other valid process
            for (int p = 0; p < NUM_PROCESSES; p++) {
                if (processControlBlocks[p].burstLeft == 0) continue;
                if (processControlBlocks[p].arrivalTime > timePassed) continue;    // hasnt started yet

                // process has started, increase turn around time
                processControlBlocks[p].turnAroundTime += 1;
            }

            timeLeft -= 1;

            // choose next process to run
            if (timeLeft > 0 && processControlBlocks[PIDToRun].burstLeft != 0) {
                continue;
            }
            else {
                timeLeft = timeQuantum;
            }

            // if time ran out, choose a different process
            int PIDToCheck = PIDToRun;
            for (int k = 0; k < NUM_PROCESSES; k++) {
                PIDToCheck = (PIDToCheck + 1) % NUM_PROCESSES;

                if (processControlBlocks[PIDToCheck].burstLeft == 0) continue;
                if (processControlBlocks[PIDToCheck].arrivalTime > timePassed) continue;     // new process to switch to must have arrived

                PIDToRun = PIDToCheck;
                break;
            }
        }

        printf("\n");

        int sumWaitTime = 0;
        int sumTurnAroundTime = 0;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            processControlBlocks[i].turnAroundTime += 1;

            printf("P%d\n", i);
            printf("\tWaiting time:\t\t%d\n", processControlBlocks[i].waitTime);
            printf("\tTurnaround time:\t%d\n", processControlBlocks[i].turnAroundTime);
            printf("\n");

            sumWaitTime += processControlBlocks[i].waitTime;
            sumTurnAroundTime += processControlBlocks[i].turnAroundTime;
        }
        

        double avgWaitTime = (double)sumWaitTime/NUM_PROCESSES;
        double avgTurnAroundTime = (double)sumTurnAroundTime/NUM_PROCESSES;

        printf("Total average waiting time:\t%.1f\n", avgWaitTime);
        printf("Total average turnaround time:\t%.1f\n", avgTurnAroundTime);
    }
}