/*
    Title: Assignment 1
    Author: Jason Kwan
    Description: Hacks into hackme program by brute forcing. Implements forking.
*/

#include "checkPassword.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// first and last ascii characters to brute force between
#define FIRST_ASCII 33
#define LAST_ASCII 126

// brute forces for every combination of ascii characters from 33 to 126
// returns 0 on success, -1 on failure
int myCheckPassword(int start)
{
    char first;
    char second;
    char third;

    char tryPassword[4];
    tryPassword[3] = '\0';
    for (int i = FIRST_ASCII; i < LAST_ASCII + 1; i++)
    {
        first = i;
        tryPassword[0] = first;

        for (int j = FIRST_ASCII; j < LAST_ASCII + 1; j++)
        {
            second = j;
            tryPassword[1] = second;
            
            for (int k = FIRST_ASCII; k < LAST_ASCII + 1; k++)
            {
                third = k;
                tryPassword[2] = third;
                int result = checkPassword(tryPassword, start);

                if (result == 0)
                {
                    printf("%s\n", tryPassword);
                    return 0;
                }
            }
        }
    }

    // printf("Could not find a 3-digit password.");    // debug
    return -1;
}

// brute forces password with forking. creates 4 processes.
// prints out PIDs of all processes
// always return 0
int mainWithF()
{
    pid_t pid1 = fork();
    pid_t pid2 = fork();

    if (pid1 > 0)
    {
        if (pid2 > 0)   // parent (1.0)
        {
            printf("PID 1.0 IS %d. CHILD 1.1 IS %d\n", getpid(), pid1);
            printf("PID 1.0 IS %d. CHILD 1.2 IS %d\n", getpid(), pid2);
            printf("PID 1.2 IS %d. PPID 1.0 IS %d\n", pid2, getpid());

            myCheckPassword(0);

            while (wait(NULL)>0);
        }
        else    // child 1 (1.2)
        {
            myCheckPassword(3);
        }
    }
    if (pid1 == 0)
    {
        if (pid2 > 0)   // child 2 (1.1)
        {
            printf("PID 1.1 IS %d. PPID 1.0 IS %d\n", getpid(), getppid());
            printf("PID 1.1 IS %d. CHILD 1.1.1 IS %d\n", getpid(), pid2);
            myCheckPassword(6);

            wait(NULL);
        }
        else    // child 3 (1.1.1)
        {
            printf("PID 1.1.1 IS %d. PPID 1.1 IS %d\n", getpid(), getppid());

            myCheckPassword(9);
        }
    }

    return 0;
}

// always return 0
int main(int argc, char *argv[])
{
    // argc is always at least one
    if (argc == 1)
    {
        myCheckPassword(0);
        myCheckPassword(3);
        myCheckPassword(6);
        myCheckPassword(9);
        return 0;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-f") == 0) // run with forking if -f is included
        {
            mainWithF();

            return 0;
        }
        else if (strcmp(argv[1], "-p") == 0)    // run execl if -p is included
        {
            myCheckPassword(0);
            myCheckPassword(3);
            myCheckPassword(6);
            myCheckPassword(9);

            execl("hackme", "", NULL);
        }
    }
    else if (argc == 3)
    {
        // run with forking and run execl if -f and -p are included
        if ((strcmp(argv[1], "-f") == 0 || strcmp(argv[2], "-f") == 0) && (strcmp(argv[1], "-p") == 0 || strcmp(argv[2], "-p") == 0))
        {
            // copy pasted function to add execl in
            pid_t pid1 = fork();
            pid_t pid2 = fork();

            if (pid1 > 0)
            {
                if (pid2 > 0)   // parent (1.0)
                {
                    printf("PID 1.0 IS %d. CHILD 1.1 IS %d\n", getpid(), pid1);
                    printf("PID 1.0 IS %d. CHILD 1.2 IS %d\n", getpid(), pid2);
                    printf("PID 1.2 IS %d. PPID 1.0 IS %d\n", pid2, getpid());

                    myCheckPassword(0);

                    while (wait(NULL)>0);

                    execl("hackme", "", NULL);  
                }
                else    // child 1 (1.2)
                {
                    myCheckPassword(3);
                }
            }
            if (pid1 == 0)
            {
                if (pid2 > 0)   // child 2 (1.1)
                {
                    printf("PID 1.1 IS %d. PPID 1.0 IS %d\n", getpid(), getppid());
                    printf("PID 1.1 IS %d. CHILD 1.1.1 IS %d\n", getpid(), pid2);
                    myCheckPassword(6);

                    wait(NULL);
                }
                else    // child 3 (1.1.1)
                {
                    printf("PID 1.1.1 IS %d. PPID 1.1 IS %d\n", getpid(), getppid());

                    myCheckPassword(9);
                }
            }
        }
    }

    return 0;
}