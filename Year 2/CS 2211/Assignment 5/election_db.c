/*********************************************************
 * From C PROGRAMMING: A MODERN APPROACH, Second Edition *
 * By K. N. King                                         *
 * Copyright (c) 2008, 1996 W. W. Norton & Company, Inc. *
 * All rights reserved.                                  *
 * This program may be freely distributed for class use, *
 * provided that this copyright notice is retained.      *
 *********************************************************/

/* Adopted from inventory2.c (Chapter 17, page 434) by Will Beldman */
/* Maintains an Election database (linked list version) */

#include "election_db.h"
#include <stdlib.h>

static int try_again(void)
{
    char decision;
    printf("Try again or return to main menu [t/r]: ");
    scanf(" %c", &decision);

    switch (decision)
    {
        case 't':
            return 0;
            break;
        case 'r':
            printf("Returning to main menu.\n");
            return 1;
            break;
        default:
            printf("Invalid input. returning to main menu. \n");
            return 2;
            break;
    }
}

int main(void)
{
    // Where linked lists are stored
    struct riding *riding_tbl = NULL;   /* points to first riding */
    struct candidate *candidate_tbl = NULL;   /* points to first candidate */

    char entity, code;
    printf("*****************\n");
    printf("* 2211 Election *\n");
    printf("*****************\n\n\n");

    for (;;) {
        printf("Enter entity type (h for help, q to quit, r for riding, c for candidates): ");
        scanf(" %c", &entity);
        while (getchar() != '\n')   /* skips to end of line */
        ;
        if (entity == 'h'){
            print_help();
        }
        // Riding menu
        else if (entity == 'r'){
            printf("Riding:\n\t");
            // printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase): ");
            printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\tr to print registrants of riding, e to erase): ");
            scanf(" %c", &code);
            while (getchar() != '\n')   /* skips to end of line */
                ;

            switch (code) {
                case 'i': insert_riding(&riding_tbl);
                            break;
                case 's': search_riding(&riding_tbl);
                            break;
                case 'u': update_riding(&riding_tbl);
                            break;
                case 'p': print_riding(&riding_tbl);
                            break;
                // case 'd': dump_riding(&riding_tbl);
                //             break;
                // Changes: Changed from 'r' to 'b'
                // case 'b': restore_riding(&riding_tbl);
                //             break;
                case 'e': erase_riding(&riding_tbl);
                            break;
                case 'r': print_riding_candidates(&candidate_tbl, &riding_tbl);
                            break;
                default:  printf("Illegal code\n");
            }
        }
        // Candidate menu
        else if (entity == 'c'){
            printf("Candidate:\n\t");
            // printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase): ");
            printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\tr to register candidate into riding, e to erase): ");
            scanf(" %c", &code);
            while (getchar() != '\n')   /* skips to end of line */
                ;

            switch (code) {
                case 'i': insert_candidate(&candidate_tbl);
                            break;
                case 's': search_candidate(&candidate_tbl);
                            break;
                case 'u': update_candidate(&candidate_tbl);
                            break;
                case 'p': print_candidate(&candidate_tbl);
                            break;
                // case 'd': dump_candidate(&candidate_tbl);
                //             break;
                // Changes: Changed from 'r' to 'b'
                // case 'b': restore_candidate(&candidate_tbl);
                //             break;
                case 'e': erase_candidate(&candidate_tbl);
                            break;
                // 10% Bonus
                case 'r': register_candidate(&candidate_tbl, &riding_tbl);
                            break;
                default:  printf("Illegal code\n");
            }
        }
        else if (entity == 'q'){
            return 0;
        }
        else{
            printf("Illegal entity\n");
        }
        printf("\n");
    }
}

void print_help(void){
    printf("\nEnter r for riding, c for candidate, then\n");
    // printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase)\n\n ");
    printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\tr to register candidate (if in candidate menu) or print registrants of riding (if in riding menu),\n\t e to erase)\n\n ");
}

// Return 0 if successful, 1 if candidate party is already registered
int verifyParty(char party1, char party2, int index)
{
    // Ensure only one candidate per party can be represented in one riding.
    if (party1 == party2)
    {
        if (parties_registered[index] != 0)
        {
            return 1;
        }

        // Valid! Candidate's party hasn't been represented in a riding yet
        parties_registered[index] = 1;
        return 0;
    }

    // Not checking correct party anyways, means can be no error
    return 0;
}

void register_candidate(struct candidate **candidate_tbl, struct riding **riding_tbl)
{
    askCode:

    long long int code;
    printf("\tEnter candidate code: ");
    scanf(" %lld", &code);

    struct candidate *curC;
    struct riding *curR;
    for (curC = *candidate_tbl; curC != NULL; curC = curC->next)
    {
        if (curC != NULL && code == curC->number)
        {
            // We found candidate, now update ONLY RIDING CODE

            // Riding code we want to register candidate into
            long long int r_code;

            printf("Enter riding code to register candidate into. Note that riding must exist: \n");
            scanf(" %lld", &r_code);

            int riding_exists = 0;
            // Ensure riding exists
            for (curR = *riding_tbl; curR != NULL; curR = curR->next)
            {
                // Ensure the riding we want to register the candidate into exists and matches
                if (curR != NULL && r_code == curR->number)
                {
                    riding_exists = 1;
                    break;
                }
            }

            if (!riding_exists)
            {
                printf("Riding doesn't exist.\n");

                if (try_again() != 0)
                {
                    return;
                }

                // Equal to 0 - Try again
                goto askCode;
            }

            // Ensure only one candidate per party can be represented in one riding.
            int result = 0;
            result = verifyParty(curC->party, 'R', 0) + verifyParty(curC->party, 'O', 1) + verifyParty(curC->party, 'Y', 2) + verifyParty(curC->party, 'G', 3) + verifyParty(curC->party, 'B', 4) + verifyParty(curC->party, 'I', 5) + verifyParty(curC->party, 'V', 6);
            if (result != 0)
            {
                printf("Candidate's party is already registered in a riding.\n");

                if (try_again() != 0)
                {
                    return;
                }

                // Equal to 0 - Try again
                goto askCode;
            }
            // Finally update candidate 
            curC->riding_code = r_code;
            return;
        }
    }

    // Not found (bc function doesnt return)
    printf("\tEnter candidate code: ");
}

void print_riding_candidates(struct candidate **candidate_tbl, struct riding **riding_tbl)
{
    long long int code;
    printf("\tEnter riding code: ");
    scanf(" %lld", &code);

    struct candidate *curC;
    struct riding *curR;
    for (curC = *candidate_tbl; curC != NULL; curC = curC->next)
    {
        if (curC != NULL && code == curC->riding_code)
        {
            // We found candidate whose riding code matches
            printf("Candidate Code   Candidate Name                 Age                   Party\n");
            // Fantastic values to match OWL example
            printf("%-17lld%-31s%-22d%-c\n", curC->number, curC->name, curC->age, curC->party);
            return;
        }
    }
}