CC=gcc
OBJS=HackEnrollment.o IsraeliQueue.o main.o
EXEC=HackEnrollment 
DEBUG_FLAG=-g
COMP_FLAG=-std=c99 -I/new_home/mtm/public/2223b/ex1 -Itool -Wall -Werror -pedantic-errors -DNDEBUG $(DEBUG) -lm

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ -lm

HackEnrollment.o: tool/HackEnrollment.c tool/HackEnrollment.h 
				  $(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tool/$*.c 
IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
				$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
main.o: tool/main.c tool/HackEnrollment.h
		$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tool/$*.c

clean:
	rm -f $(OBJS) $(EXEC)

