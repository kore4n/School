/*  
    Name: election_riding.c
    Purpose: Program containing all the functions used to manage ridings
    Author: Jason Kwan - Student Number: 251173792
*/

#include "election_riding.h"

static void print_heading(void)
{
    // Fantastic spacing values to match OWL example
    printf("Riding Code   Riding Name                 Population                   Area\n");
}

static void print_single_riding(const struct riding **r)
{
    // Fantastic values to match OWL example
    printf("%-14lld%-28s%-29ld%-.2f\n", (*r)->number, (*r)->name, (*r)->population, (*r)->area);
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

void insert_riding(struct riding **riding_tbl)
{
    long long int code;
    char name[MAX_NAME_LENGTH];
    long int pop;
    float area;

    askCode:
    
    printf("\tEnter riding code: ");
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

    const struct riding *cur;
    for (cur = *riding_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            printf("Riding already exists.\n");

            if (try_again() != 0)
            {
                return;
            }
            goto askCode;
        }
    }
    
    printf("\tEnter riding name: ");
    scanf(" %[^\t\n]s", name);
    
    askPop:
    printf("\tEnter population: ");
    scanf("%ld", &pop);
    if (pop < MIN_POP)
    {
        printf("Population too low.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askPop;
    }
    else if (pop > MAX_POP)
    {
        printf("Population too high.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askPop;
    }

    askArea:
    printf("\tEnter the area: ");
    scanf("%f", &area);

    if (area < MIN_AREA)
    {
        printf("Area too low.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askArea;
    }
    else if (area > MAX_AREA)
    {
        printf("Area too large.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askArea;
    }

    struct riding *new_node;
    new_node = malloc(sizeof(struct riding));
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

    new_node->number = code;
    new_node->area = area;
    new_node->population = pop;
    strcpy(new_node->name, name);

    // Insert node at the beginning of the linked list
    new_node->next = *riding_tbl;
    *riding_tbl = new_node;
}

void search_riding(struct riding **riding_tbl)
{
    long long int code;
    printf("\tEnter riding code: ");
    scanf(" %lld", &code);

    const struct riding *cur;
    for (cur = *riding_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            // Print out newly found riding
            print_heading();
            print_single_riding(&cur);
            return;
        }
    }

    // Riding was not found
    printf("Riding was not found.\n");\
}

void update_riding(struct riding **riding_tbl)
{
    askCode:
    
    long long int code;
    printf("\tEnter riding code: ");
    scanf(" %lld", &code);

    struct riding *cur;
    for (cur = *riding_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL && code == cur->number)
        {
            // We found riding, now update
            char name[MAX_NAME_LENGTH];
            long int pop;
            float area;
            
            printf("\tEnter riding name: ");
            // TODO:
            scanf(" %49s", name);
            
            askPop:
            printf("\tEnter population: ");
            scanf("%ld", &pop);
            if (pop < MIN_POP)
            {
                printf("Population too low.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askPop;
            }
            else if (pop > MAX_POP)
            {
                printf("Population too high.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askPop;
            }

            askArea:
            printf("\tEnter the area: ");
            scanf("%f", &area);

            if (area < MIN_AREA)
            {
                printf("Area too low.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askArea;
            }
            else if (area > MAX_AREA)
            {
                printf("Area too large.\n");
                if (try_again() != 0)
                {
                    return;
                }
                
                // Equal to 0 - Try again
                goto askArea;
            }

            // Finally update riding
            cur->area = area;
            cur->population = pop;
            strcpy(cur->name, name);

            return;
        }
    }

    // Not found (bc function doesnt return)
    printf("\tRiding not found.\n");
    if (try_again() != 0)
    {
        return;
    }
    
    // Equal to 0 - Try again
    goto askCode;
}

void print_riding(struct riding **riding_tbl)
{
    print_heading();
    const struct riding *cur;
    for (cur = *riding_tbl; cur != NULL; cur = cur->next)
    {
        if (cur != NULL)
        {
            // Print current riding
            print_single_riding(&cur);
        }
    }
}

void erase_riding(struct riding **riding_tbl)
{
    long long int code;

    askErase:
    printf("\tEnter riding code: ");
    scanf(" %lld", &code);

    struct riding *cur, *prev;
    for (cur = *riding_tbl, prev = NULL; cur != NULL && cur->number != code; prev = cur, cur = cur->next)
    {
    }

    // Riding code not found
    if (cur == NULL)
    {
        printf("Riding was not found.\n");
        if (try_again() != 0)
        {
            return;
        }
        
        // Equal to 0 - Try again
        goto askErase;
    }
    // Riding was found in first node
    if (prev == NULL)
    {
        // TODO: Remove, didnt work for some reason
        // riding_tbl = &(*riding_tbl)->next;
        (*riding_tbl) = (*riding_tbl)->next;
    }
    // Riding was found in not the first node
    else
    {
        prev->next = cur->next;
    }
    free(cur);
}

/* dump: Dumps contents of the riding_tbl to specified
 *       file. Prints error message and does not
 *       terminate program upon failure. Does not store
 *       next pointer.
 */
// void dump_riding(struct riding **riding_tbl)
// {
//     FILE *fp;
//     char filename[255];
//     struct riding **node = riding_tbl;

//     printf("Enter name of output file: ");
//     read_line(filename, 255);

//     if ((fp = fopen(filename, "wb")) == NULL)
//     {
//         fprintf(stderr, "File %s could not be written\n", filename);
//         return;
//     }

//     while (*node)
//     {
//         fwrite(*node, sizeof(struct riding) - sizeof(struct riding *), 1, fp);
//         node = &(*node)->next;
//     }
//     fclose(fp);
//     return;
// }

/* restore: Restores contents from specified file. Does
 *          not terminate upon file open failure, but
 *          terminates upon malloc failure.
 */
// void restore_riding(struct riding **riding_tbl)
// {
//     FILE *fp;
//     char filename[255];
//     struct riding buffer;
//     struct riding *temp;
//     struct riding **node;

//     printf("Enter name of input file: ");
//     read_line(filename, 255);

//     if ((fp = fopen(filename, "rb")) == NULL)
//     {
//         fprintf(stderr, "Error: file %s cannot be opened\n", filename);
//         return;
//     }

//     while (*riding_tbl)
//     {
//         temp = *riding_tbl;
//         *riding_tbl = (*riding_tbl)->next;
//         free(temp);
//     }

//     node = riding_tbl;

//     while (fread(&buffer,sizeof(struct riding) - sizeof(struct riding *),1,fp) == 1)
//     {
//         if ((*node = malloc(sizeof(struct riding))) == NULL)
//         {
//             fprintf(stderr, "Error: malloc failed in restore\n");
//             exit(EXIT_FAILURE);
//         }
//         (*node)->number = buffer.number;
//         strcpy((*node)->name, buffer.name);
//         (*node)->population = buffer.population;
//         (*node)->area = buffer.area;
//         (*node)->next = NULL;
//         node = &(*node)->next;
//     }
//     fclose(fp);
//     return;
// }