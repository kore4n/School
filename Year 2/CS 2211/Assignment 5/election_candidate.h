/*  
    Name: election_candidate.h
    Purpose: Header containing macros and definitions for candidates
    Author: Jason Kwan - Student Number: 251173792
*/

#ifndef ELECTION_CANDIDATE_H
#define ELECTION_CANDIDATE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 50
#define MIN_AGE 18
#define MAX_AGE 99
#define PARTIES 7

static char parties[] = {'R', 'O', 'Y', 'G', 'B', 'I', 'V'};

struct candidate
{
    long long int number, riding_code; 
    char name[MAX_NAME_LENGTH];

    int age;
    char party;

    struct candidate *next;
};

void insert_candidate(struct candidate **);
void search_candidate(struct candidate **);
void update_candidate(struct candidate **);
void print_candidate(struct candidate **);
void erase_candidate(struct candidate **);

// Register the candidate into a riding (by number)
// Search through riding list for specified candidate. Then assign riding code to whatever riding user specifies. Ensure riding exists by searching through riding list.
// void register_candidate(struct candidate **, struct riding **);

void dump_candidate(struct candidate **);
void restore_candidate(struct candidate **);

#endif