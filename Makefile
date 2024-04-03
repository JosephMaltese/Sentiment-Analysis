CC = gcc
CFLAGS = -Wall -Wextra -g

all: out

out: sentimentAnalysis.o functions.o
	$(CC) $(CFLAGS) -o out sentimentAnalysis.o functions.o

sentimentAnalysis.o: sentimentAnalysis.c functions.h
	$(CC) $(CFLAGS) -c sentimentAnalysis.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean: 
	rm -f out *.o
