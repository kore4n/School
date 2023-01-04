/*  
    Name: election_candidate.c
    Purpose: Program containing all the functions used to manage candidates
    Author: Jason Kwan - Student Number: 251173792
*/

#include "election_candidate.h"

static void print_heading(void)
{
    // Fantastic spacing values to match OWL example
    printf("Candidate Code   Candidate Name                 Age                   Party\n");
}

static void print_single_candidate(const struct candidate **c)
{
    // Fantastic values to match OWL example
    printf("%-17lld%-31s%-22d%-c\n", (*c)->number, (*c)->name, (*c)->age, (*c)->party);
    return;
}

// Return 0 for try again (t), 1 for return to main menu (r), 2 for invalid input and return to main menu
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

void insert_candidate(struct candidate **candidate_tbl)
{
    long long int code;
    char name[MAX_NAME_LENGTH];
    int age;
    char party;

    askCode:
    
    printf("\tEnter candidate code: ");
    scanf(" %lld", &code);

    // No maximum, just cant be negative or already existing
    if (code < 0)
    {
        printf("Invalid code.\n");

        if (try_again() != 0)
        {
            return;
        }

        // Equal to 0 - Try again
        goto askCode;
    }

    const struct candidate *cur;
    for (cur = *candidate_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            printf("Candidate already exists.\n");

            if (try_again() != 0)
            {
                return;
            }
            goto askCode;
        }
    }
    
    printf("\tEnter candidate name: ");
    scanf(" %[^\t\n]s", name);
    
    askAge:
    printf("\tEnter age: ");
    scanf("%d", &age);
    if (age < MIN_AGE)
    {
        printf("Age too low.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askAge;
    }
    else if (age > MAX_AGE)
    {
        printf("Age too high.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askAge;
    }

    askParty:
    printf("\tEnter the party: ");
    scanf(" %c", &party);

    int partyValid = 0;

    // Check if party is valid
    for (int i = 0; i < PARTIES; i++)
    {
        if (parties[i] == party)
        {
            // it is n
            partyValid = 1;
            break;
        }
    }

    // Not a valid party
    if (partyValid != 1)
    {
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askParty;
    }

    struct candidate *new_node;
    new_node = malloc(sizeof(struct candidate));
    if (new_node == NULL) 
    {
        printf("Error: malloc failed in add_to_list\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askCode;
    }

    // Leave riding code for a separate command. Initialize to -1;
    new_node->riding_code = -1;
    new_node->number = code;
    new_node->age = age;
    new_node->party = party;
    
    strcpy(new_node->name, name);

    // Insert node at the beginning of the linked list
    new_node->next = *candidate_tbl;
    *candidate_tbl = new_node;
}

void search_candidate(struct candidate **candidate_tbl)
{
    long long int code;
    printf("\tEnter candidate code: ");
    scanf(" %lld", &code);

    const struct candidate *cur;
    for (cur = *candidate_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            // Print out newly found candidate
            print_heading();
            print_single_candidate(&cur);
            return;
        }
    }

    // Candidate was not found
    printf("Candidate was not found.\n");\
}

void update_candidate(struct candidate **candidate_tbl)
{
    askCode:

    long long int code;
    printf("\tEnter candidate code: ");
    scanf(" %lld", &code);

    struct candidate *cur;
    for (cur = *candidate_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            // We found candidate, now update
            char name[MAX_NAME_LENGTH];
            int age;
            char party;
            
            printf("\tEnter candidate name: ");
            scanf(" %[^\t\n]s", name);
            
            askAge:

            printf("\tEnter age: ");
            scanf("%d", &age);
            if (age < MIN_AGE)
            {
                printf("Age too low.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askAge;
            }
            else if (age > MAX_AGE)
            {
                printf("Age too high.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askAge;
            }

            askParty:

            printf("\tEnter the party [R/O/Y/G/B/I/V]: ");
            scanf("%c", &party);

            int partyValid = 0;

            // Check if party is valid
            for (int i = 0; i < PARTIES; i++)
            {
                if (parties[i] == party)
                {
                    // it is n
                    partyValid = 1;
                    break;
                }
            }

            // Not a valid party
            if (partyValid != 1)
            {
                printf("Invalid party.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askParty;
            }

            // Finally update candidate
            cur->age = age;
            cur->party = party;
            strcpy(cur->name, name);
            // Leave riding code alone

            return;
        }
    }

    // Not found (bc function doesnt return)
    printf("\tCandidate not found.\n");
    if (try_again() != 0)
    {
        return;
    }
    
    // Equal to 0 - Try again
    goto askCode;
}

void print_candidate(struct candidate **candidate_tbl)
{
    print_heading();
    const struct candidate *cur;
    for (cur = *candidate_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL)
        {
            // Print current candidate
            print_single_candidate(&cur);
        }
    }
}

void erase_candidate(struct candidate **candidate_tbl)
{
    long long int code;

    askErase:
    printf("\tEnter candidate code: ");
    scanf(" %lld", &code);

    struct candidate *cur, *prev;
    for (cur = *candidate_tbl, prev = NULL; cur != NULL && cur->number != code; prev = cur, cur = cur->next)
    {
    }

    // Candidate code not found
    if (cur == NULL)
    {
        printf("Candidate was not found.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askErase;
    }
    // Candidate was found in first node
    if (prev == NULL)
    {
        (*candidate_tbl) = (*candidate_tbl)->next;
    }
    // Candidate was found in not the first node
    else
    {
        prev->next = cur->next;
    }
    free(cur);
}

// 10% bonus
// void register_candidate(struct candidate **candidate_tbl, struct riding **riding_tbl)
// {

// }

/* dump: Dumps contents of the candidate_tbl to specified
 *       file. Prints error message and does not
 *       terminate program upon failure. Does not store
 *       next pointer.
 */
// void dump_candidate(struct candidate **candidate_tbl)
// {
//     FILE *fp;
//     char filename[255];
//     struct candidate **node = candidate_tbl;

//     printf("Enter name of output file: ");
//     read_line(filename, 255);

//     if ((fp = fopen(filename, "wb")) == NULL)
//     {
//         fprintf(stderr, "File %s could not be written\n", filename);
//         return;
//     }

//     while (*node)
//     {
//         fwrite(*node, sizeof(struct candidate) - sizeof(struct candidate *), 1, fp);
//         node = &(*node)->next;
//     }
//     fclose(fp);
//     return;
// }

/* restore: Restores contents from specified file. Does
 *          not terminate upon file open failure, but
 *          terminates upon malloc failure.
 */
// void restore_candidate(struct candidate **candidate_tbl)
// {
//     FILE *fp;
//     char filename[255];
//     struct candidate buffer;
//     struct candidate *temp;
//     struct candidate **node;

//     printf("Enter name of input file: ");
//     read_line(filename, 255);

//     if ((fp = fopen(filename, "rb")) == NULL)
//     {
//         fprintf(stderr, "Error: file %s cannot be opened\n", filename);
//         return;
//     }

//     while (*candidate_tbl)
//     {
//         temp = *candidate_tbl;
//         *candidate_tbl = (*candidate_tbl)->next;
//         free(temp);
//     }

//     node = candidate_tbl;

//     while (fread(&buffer,sizeof(struct candidate) - sizeof(struct candidate *),1,fp) == 1)
//     {
//         if ((*node = malloc(sizeof(struct candidate))) == NULL)
//         {
//             fprintf(stderr, "Error: malloc failed in restore\n");
//             exit(EXIT_FAILURE);
//         }
//         (*node)->number = buffer.number;
//         strcpy((*node)->name, buffer.name);
//         (*node)->age = buffer.age;
//         (*node)->party = buffer.party;
//         (*node)->next = NULL;
//         node = &(*node)->next;
//     }
//     fclose(fp);
//     return;
// }