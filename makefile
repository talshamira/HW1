CC=gcc
OBJS=tool/HackEnrollment.o IsraeliQueue.o tool/main.o
EXEC=HackEnrollment 
DEBUG=
CFLAGS=-std=c99 -lm -I/home/tal.shamir/ex1 -Itool -Wall -Werror -pedantic-errors -DNDEBUG $(DEBUG)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJS) -o $@

HackEnrollment.o: tool/HackEnrollment.c tool/HackEnrollment.h IsraeliQueue.h
IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
main.o: tool/main.c IsraeliQueue.h tool/HackEnrollment.h

clean:
	rm -f $(OBJS) &(EXEC)
