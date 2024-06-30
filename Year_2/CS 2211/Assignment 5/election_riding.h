/*  
    Name: election_riding.c
    Purpose: Header containing macros and definitions for ridings
    Author: Jason Kwan - Student Number: 251173792
*/

#ifndef ELECTION_RIDING_H
#define ELECTION_RIDING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 50
#define MIN_POP 30000
#define MAX_POP 130000
#define MIN_AREA 10
#define MAX_AREA 100000

struct riding
{
    long long int number; // Synonymous with riding code
    char name[MAX_NAME_LENGTH];
    long int population;
    float area;

    int has_candidate;  // Always initialized to 0. Only updated when user registers candidate to riding.
    struct riding *next;
};

void insert_riding(struct riding **);
void search_riding(struct riding **);
void update_riding(struct riding **);
void print_riding(struct riding **);
void erase_riding(struct riding **);
void dump_riding(struct riding **);
void restore_riding(struct riding **);

#endif