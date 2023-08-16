/*
    Title: Assignment 3
    Author: Jason Kwan
    Description: Finds prime factors and their sums between an interval. Implements threading.
    Date: Feb 19, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// number of unique values to pass to threads
// one for thread id, begin, and end points
#define NUM_VALUES_TO_THREAD 3
#define NUM_VALUES_FROM_THREAD 2

// new thread to do work
// returns null
void *findPrimesFromTo(void * args) {
    // read in id, begin, and end points
    int *values = (int *) args;
    int threadNum = 0;
    long long int low = 0;    // same as begin point
    long long int high = 0;   // same as end point

    // sum and counts for this thread only
    long long int threadSum = 0;
    int threadCount = 0;

    // read thread specific data
    // id, begin and end points
    threadNum = values[0];
    low = values[1];
    high = values[2];
    
    printf("Thread # %d is finding primes from low = %lld to high = %lld\n", threadNum, low, high);

    // find primes between low and high
    int flag, i;
    // iteration until low is not equal to high
    while (low < high) {
        flag = 0;

        // ignore numbers less than 2
        if (low <= 1) {
            ++low;
            continue;
        }

        // if low is a non-prime number, flag will be 1
        for (i = 2; i <= low / 2; ++i) {

            if (low % i == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            // add to thread sum and count
            threadSum += low;
            threadCount++;
        }

        // to check prime for the next number
        // increase low by 1
        ++low;
    }    

    free(values);
    printf("Sum of thread %d is %lld, Count is %d\n", threadNum, threadSum, threadCount);
    long long int* result = malloc(NUM_VALUES_FROM_THREAD * sizeof(long long int));

    // send thread sum and count back to main
    result[0] = threadSum;
    result[1] = threadCount;

    return (void *)result;
}

// always return 0
int main(int argc, char *argv[])
{
    if (argc != 3) { return 0; }    // only allow 2 command line arguments

    int numThreads = atoi(argv[1]); // # threads to make
    int maxNum = atoi(argv[2]);     // max number to find prime factors until

    if (numThreads <= 0) {
        printf("Number of threads specified must be greater than 0.\n");
        return 0;
    }

    // calculate how many numbers to assign to each thread. uses fast ceiling function
    int maxPerThread = maxNum/numThreads + (maxNum % numThreads != 0);  

    pthread_t threadNames[numThreads];          // all threadNames
    int curThreadNum;    // current thread number

    // create all threads
    for (curThreadNum = 0; curThreadNum < numThreads; curThreadNum++) {
        // declare begin and end points
        int beginAt;
        int endAt;

        // set begin and end points
        beginAt = curThreadNum * maxPerThread;
        endAt = (curThreadNum + 1) * maxPerThread;

        // overwrite to calculate only remainder interval if last thread to be made
        if (curThreadNum == numThreads - 1)
            endAt = maxNum;

        // pass these values to this thread
        int *values = (int *) malloc(NUM_VALUES_TO_THREAD * sizeof(int)); // malloc to ensure memory isnt passed by reference to thread
        values[0] = curThreadNum;
        values[1] = beginAt;
        values[2] = endAt;
        
        // create + join threads
        if (pthread_create(&threadNames[curThreadNum], NULL, findPrimesFromTo, (void *)values) != 0) {
            printf("Error while creating thread\n");
        }
    }

    // total sums and counts
    long long int grandSum = 0;
    int grandCount = 0;

    // join with threads and read their results
    for (int i = 0; i < numThreads; i++) {
        long long int* result;
        if (pthread_join(threadNames[i], (void **)&result) != 0){
            printf("Error joining with thread");
        }
        // read sum and count back from thread
        long long int sum = result[0];
        int count = result[1];

        // add thread sum and counts to total
        grandSum += sum;
        grandCount += count;

        free(result);
    }
    
    printf("\n\tGRAND SUM IS %lld, COUNT IS %d\n", grandSum, grandCount);

    return 0;
}