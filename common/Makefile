# Makefile for 'common.a'
# CS50 project 'Nuggets'
#
# Plankton, May 2023
#

############## build the common.a library ##########

OBJS = grid.o game.o
L = ../libs
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I $L
CC = gcc
LLIBS = $L/libs.a
MAKE = make

common.a: $(OBJS) $(LLIBS)
	ar cr $@ $^

# grid.o: grid.c grid.h game.h ../libs/file.h ../libs/mem.h
# 	$(CC) $(CFLAGS) -c grid.c

# game.o: game.c game.h grid.h ../libs/file.h ../libs/mem.h
# 	$(CC) $(CFLAGS) -c game.c

game.o: game.h
grid.o: grid.h 

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f common.a

