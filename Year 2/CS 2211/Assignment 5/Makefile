CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -std=c99

default:
	make all

clean:
	rm -f *.o election_riding election_candidate election_db

all: election_db.o election_riding.o election_candidate.o
	$(CC) $(CFLAGS) -o election_db election_db.o election_riding.o election_candidate.o

election_db.o: election_db.c election_db.h election_riding.h election_candidate.h
	$(CC) $(CFLAGS) -c election_db.c

election_riding.o: election_riding.c election_riding.h
	$(CC) $(CFLAGS) -c election_riding.c

election_candidate.o: election_candidate.c election_candidate.h
	$(CC) $(CFLAGS) -c election_candidate.c

