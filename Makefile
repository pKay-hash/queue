# modify this makefile for your implementation
#   as described in the handout

llist.o:  list.h llist.c
	gcc -c llist.c

sq.o: sq.h sq.c
	gcc -c sq.c

driver: driver.c sq.o
	gcc driver.c sq.o -lc -o driver

clean:
	rm -f *.o driver
