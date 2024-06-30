/*
    Title: Assignment 2
    Author: Jason Kwan
    Description: Prints out product of 2 large integers using divide and conquer. Implements forking and piping.
    Date: Feb 4, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#define N 4

// always return 0
int main(int argc, char *argv[])
{
    if (argc != 3) { return 0; }    // only allow 2 command line arguments

    // First steps (1-3)
    // Step 1: Partition the two numbers in to a1, a2, b1, b2
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    // handle edge cases
    if (num1/1000 <= 0 || num2/1000 <= 0) { return 0; }   // only allow 4 digit inputs

    int a1 = (num1/100);
    int a2 = (num1%100);

    int b1 = (num2/100);
    int b2 = (num2%100);

    printf("Your integers are %d %d\n", num1, num2);

    // Step 2: Make 2 pipes
    int port1[2];
    int port2[2];

    if (pipe(port1) < 0 || pipe(port2)) {
        perror("pipe error");
        exit(1);
    }

    // Step 3: Fork
    pid_t pid = fork();

    // New steps (1-9)
    if (pid<0) {
        perror("fork error");
        exit(1);
    }
    else if (pid > 0) {    // parent
        printf("Parent (PID %d): created child (PID %d)\n", getpid(), pid);

        // Step 1:
        printf("\n###\n");
        printf("# Calculating X\n");
        printf("###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);
        write(port1[1], &a1, sizeof(a1));
        write(port1[1], &b1, sizeof(b1));

        // Step 2b:
        long int a;
        read(port2[0], &a, sizeof(a));   // Read A
        printf("Parent (PID %d): Received %ld from child\n", getpid(), a);
        long int x = a * pow(10, N);

        // Step 3:
        printf("\n###\n");
        printf("# Calculating Y\n");
        printf("###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);
        write(port1[1], &a2, sizeof(a2));
        write(port1[1], &b1, sizeof(b1));

        // Step 5:
        long int b;
        read(port2[0], &b, sizeof(b));   // Read B
        printf("Parent (PID %d): Received %ld from child\n", getpid(), b);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        write(port1[1], &a1, sizeof(a1));
        write(port1[1], &b2, sizeof(b2));

        long int c;
        // Step 6b:
        read(port2[0], &c, sizeof(c));   // Read C
        printf("Parent (PID %d): Received %ld from child\n", getpid(), c);
        long int y = (b + c) * pow(10, N/2);

        // Step 7:
        printf("\n###\n");
        printf("# Calculating Z\n");
        printf("###\n");
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        write(port1[1], &a2, sizeof(a2));
        write(port1[1], &b2, sizeof(b2));

        // Step 8b:
        // No variable named after D because no further computations needed
        long int z;  // initialized to d
        read(port2[0], &z, sizeof(z));   // Read D
        printf("Parent (PID %d): Received %ld from child\n\n", getpid(), z);
        printf("%d*%d == %ld + %ld + %ld == %ld\n", num1, num2, x, y, z, x + y + z);
    }
    else if (pid == 0) {    // child
        // Step 2a:
        int receivedA1;
        int receivedB1;
        read(port1[0], &receivedA1, sizeof(receivedA1));
        read(port1[0], &receivedB1, sizeof(receivedB1));
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedA1);
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedB1);

        long int a = receivedA1 * receivedB1;
        printf("\t Child (PID %d): Sending %ld to parent\n", getpid(), a);
        write(port2[1], &a, sizeof(a));  // Send A

        // Step 4: 
        int receivedSecondA2;
        int receivedSecondB1;
        read(port1[0], &receivedSecondA2, sizeof(receivedSecondA2));
        read(port1[0], &receivedSecondB1, sizeof(receivedSecondB1));
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedSecondA2);
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedSecondB1);

        long int b = receivedSecondA2 * receivedSecondB1;
        printf("\t Child (PID %d): Sending %ld to parent\n", getpid(), b);
        write(port2[1], &b, sizeof(b));  // Send B

        // Step 6a:
        int receivedThirdA1;
        int receivedThirdB2;
        read(port1[0], &receivedThirdA1, sizeof(receivedThirdA1));
        read(port1[0], &receivedThirdB2, sizeof(receivedThirdB2));
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedThirdA1);
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedThirdB2);
        long int c = receivedThirdA1 * receivedThirdB2;
        printf("\t Child (PID %d): Sending %ld to parent\n", getpid(), c);
        write(port2[1], &c, sizeof(c));  // Send C

        // Step 8:
        int receivedFourthA2;
        int receivedFourthB2;
        read(port1[0], &receivedFourthA2, sizeof(receivedFourthA2));
        read(port1[0], &receivedFourthB2, sizeof(receivedFourthB2));
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedFourthA2);
        printf("\t Child (PID %d): Received %d from parent\n", getpid(), receivedFourthB2);
        long int d = receivedFourthA2 * receivedFourthB2;
        printf("\t Child (PID %d): Sending %ld to parent\n", getpid(), d);
        write(port2[1], &d, sizeof(d));  // Send D

        exit(0);
    }

    return 0;
}