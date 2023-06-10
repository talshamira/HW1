CC=gcc
OBJS=HackEnrollment.o IsraeliQueue.o main.o
EXEC=HackEnrollment 
DEBUG=-g
CFLAGS=-std=c99 -lm -I/home/tal.shamir/ex1 -Itool -Wall -Werror -pedantic-errors -DNDEBUG $(DEBUG)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS)  $(DEBUG_FLAG) $(OBJS) -o $@

HackEnrollment.o: HackEnrollment.c HackEnrollment.h IsraeliQueue.h
IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
main.o: main.c IsraeliQueue.h HackEnrollment.h

clean:
	rm -f $(OBJS) $(EXEC)
