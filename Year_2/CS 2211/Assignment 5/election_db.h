#ifndef ELECTION_DB_H
#define ELECTION_DB_H

#include "election_riding.h"
#include "election_candidate.h"

static char parties_registered[] = {0, 0, 0, 0, 0, 0, 0};   // 0 if not registered, 1 if registered.

int verify(char , char , int );

// Register the candidate into a riding.
// Search through candidate list for specified candidate. Ensure riding exists by searching through riding list. Then assign riding code to whatever candidate the user specifies.
void register_candidate(struct candidate **, struct riding **);

// Print all the registrants of the riding the user chooses (by number).
// Search through candidate list for candidates that match riding code. Should only return 1 candidate
void print_riding_candidates(struct candidate **, struct riding **);

void print_help(void);

#endif