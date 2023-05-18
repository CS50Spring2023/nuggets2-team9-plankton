# Makefile for nuggets modules: grid, game, and server

# only grid is inclued right now

OBJS = grid.o libs/file.o libs/mem.o 

# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I/libs
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# compile: $make
grid: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

grid.o: grid.c 
../lib/file.o: ../lib/file.c ../lib/file.h
../lib/mem.o: ../lib/mem.c ../lib/mem.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f grid
	rm -f core