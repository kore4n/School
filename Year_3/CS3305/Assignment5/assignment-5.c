/*
    Title: Assignment 5
    Author: Jason Kwan
    Description: Fill all stations from 1-4 with passengers using mutex locks.
    Date: March 24, 2023
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_STATIONS 5
// should be 0.1 for submitting
#define TIME_FOR_ONE_PASSENGER 0.1

struct train {
    int id;
    int currentPassengers;
    int maxPassengers;
};

struct station {
    int id;
    int currentPassengers;
    int requestedPassengers;    // max capacity of passengers
};

pthread_t thread_1;
pthread_t thread_2;

struct station *station_0;
struct station *station_1;
struct station *station_2;
struct station *station_3;
struct station *station_4;

struct station stations[NUM_STATIONS];
pthread_mutex_t station_locks[NUM_STATIONS];

struct train *train_0;
struct train *train_1;

pthread_mutex_t stdoutLock;
pthread_mutex_t station_0_lock;
pthread_mutex_t station_1_lock;
pthread_mutex_t station_2_lock;
pthread_mutex_t station_3_lock;
pthread_mutex_t station_4_lock;

void *transportTrain(void *train) {
    struct train *thisTrain = ((struct train *) train);

    while (true) {

        // we are finished, break out of while loop
        if (station_0->currentPassengers == 0) return 0;

        int i = 0;
        // 0 for ++, 1 for --
        int direction = 0;

        loopBegin:

        if (i == 0) {
            // visit station 0
            pthread_mutex_lock(&station_0_lock);
            int initialPassengers = station_0->currentPassengers;
            // surely train always has 0 passengers when visiting station 0
            // pick up as many passengers as possible
            if (station_0->currentPassengers >= thisTrain->maxPassengers) {
                // passengers from station 0 -> train
                thisTrain->currentPassengers = thisTrain->maxPassengers;

                // remove taken passengers from station 0
                station_0->currentPassengers -= thisTrain->maxPassengers;
            } else {
                // station 0 has less than the max passengers left
                thisTrain->currentPassengers = station_0->currentPassengers;
                station_0->currentPassengers = 0;
            }

            pthread_mutex_lock(&stdoutLock);
            sleep(TIME_FOR_ONE_PASSENGER * thisTrain->currentPassengers);

            printf(" Train %d ENTERS Station %d\n", thisTrain->id, 0);
            printf("\tStation %d has %d passengers to pick up\n", 0, initialPassengers);
            if (thisTrain->currentPassengers != 0) printf("\tPicking up passengers...\n");
            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", thisTrain->id, 0, thisTrain->currentPassengers, thisTrain->maxPassengers);
            printf("\tStation %d has %d passengers to pick up\n", 0, station_0->currentPassengers);
            printf(" Train %d LEAVES Station %d\n", thisTrain->id, 0);

            pthread_mutex_unlock(&stdoutLock);

            pthread_mutex_unlock(&station_0_lock);

            // sometimes train will leave with 0 passengers
            // to fix this perform the following:
            // if this train has no passengers, then this thread is done and we can kill
            if (thisTrain->currentPassengers == 0) return (void *)0;

            bool done = true;
            for (int k = 1; k < NUM_STATIONS; k++) {
                if (stations[k].currentPassengers != stations[k].requestedPassengers) {
                    done = false;
                    break;
                }
            }

            if (done) {
                return (void *)0;
            }

            direction = 0;
            i++;
        }

        // how many were missing before drop off
        int initialRemainingLeft = stations[i].requestedPassengers - stations[i].currentPassengers;
        // how many missing after drop off
        int remainingLeft;

        // lock current station
        pthread_mutex_lock(&station_locks[i]);

        int passengersDroppedOff = 0;
        if (stations[i].currentPassengers < stations[i].requestedPassengers && direction == 0) {
            int passengersUntilFull = stations[i].requestedPassengers - stations[i].currentPassengers;

            // train has enough to fill all the rest
            // train will continue with 0+ passengers
            if (passengersUntilFull < thisTrain->currentPassengers) { 
                stations[i].currentPassengers = stations[i].requestedPassengers; 
                passengersDroppedOff = passengersUntilFull;
                thisTrain->currentPassengers -= passengersUntilFull;
            } else {
                // train does not have enough passengers to fill the station
                stations[i].currentPassengers += thisTrain->currentPassengers;
                passengersDroppedOff = thisTrain->currentPassengers;
                thisTrain->currentPassengers -= thisTrain->currentPassengers;

                // dont break. move back station by station to station 0
                direction = 1;
            }
        }

        pthread_mutex_unlock(&station_locks[i]);
        
        pthread_mutex_lock(&stdoutLock);
        sleep(TIME_FOR_ONE_PASSENGER * passengersDroppedOff);

        remainingLeft = stations[i].requestedPassengers - stations[i].currentPassengers;

        printf(" Train %d ENTERS Station %d\n", thisTrain->id, stations[i].id);
        printf("\tStation %d has %d passengers to drop off\n", stations[i].id, initialRemainingLeft);
        if (passengersDroppedOff != 0) printf("\tUnloading passengers...\n");
        printf("\tTrain %d is at Station %d and has %d/%d passengers\n", thisTrain->id, stations[i].id, thisTrain->currentPassengers, thisTrain->maxPassengers);

        printf("\tStation %d has %d passengers to drop off\n", stations[i].id, remainingLeft);
        printf(" Train %d LEAVES Station %d\n", thisTrain->id, stations[i].id);

        pthread_mutex_unlock(&stdoutLock);

        // 0 for ++
        if (direction == 0) {
            i++;
        }
        // 1 for --
        else i--;

        goto loopBegin;
    }

    return 0;
}

// set up stations, trains and mutex locks
int setup() {
    station_0 = malloc(sizeof(struct station));
    station_0->id = 0;
    station_0->currentPassengers = 500;

    station_1 = malloc(sizeof(struct station));
    station_1->id = 1;
    station_1->requestedPassengers = 50;
    station_1->currentPassengers = 0;

    station_2 = malloc(sizeof(struct station));
    station_2->id = 2;
    station_2->requestedPassengers = 100;
    station_2->currentPassengers = 0;

    station_3 = malloc(sizeof(struct station));
    station_3->id = 3;
    station_3->requestedPassengers = 250;
    station_3->currentPassengers = 0;
    
    station_4 = malloc(sizeof(struct station));
    station_4->id = 4;
    station_4->requestedPassengers = 100;
    station_4->currentPassengers = 0;

    train_0 = malloc(sizeof(struct train));
    train_0->id = 0;
    train_0->currentPassengers = 0;
    train_0->maxPassengers = 100;

    train_1 = malloc(sizeof(struct train));
    train_1->id = 1;
    train_1->currentPassengers = 0;
    train_1->maxPassengers = 50;

    stations[0] = *station_0;
    stations[1] = *station_1;
    stations[2] = *station_2;
    stations[3] = *station_3;
    stations[4] = *station_4;

    station_locks[0] = station_0_lock;
    station_locks[1] = station_1_lock;
    station_locks[2] = station_2_lock;
    station_locks[3] = station_3_lock;
    station_locks[4] = station_4_lock;

    for (int i = 0; i < NUM_STATIONS; i++) {
        if (pthread_mutex_init(&station_locks[i], NULL) != 0) {
            printf("\n mutex init has failed\n");
            return 1;
        }
    }

    return 0;
}

int tearDown() {
    // destroy station mutex locks
    for (int i = 0; i < NUM_STATIONS; i++) { pthread_mutex_destroy(&station_locks[i]); }

    // destroy stdout mutex lock
    pthread_mutex_destroy(&stdoutLock);

    return 0;
}

int main() {
    setup();


    
    if (pthread_mutex_init(&stdoutLock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    pthread_create(&thread_1, NULL, transportTrain, train_0);
    pthread_create(&thread_2, NULL, transportTrain, train_1);

    if (pthread_join(thread_1, NULL) != 0) {
        printf("Error joining thread.\n");
    }
    if (pthread_join(thread_2, NULL) != 0) {
        printf("Error joining thread.\n");
    }

    tearDown();

    return 0;
}
