# Makefile for 'server'
# CS50 project 'Nuggets'
#
# Plankton, May 2023

OBJS = server.o 
LIBS = ../common/common.a ../libs/libs.a ../support/support.a

.PHONY: all test valgrind clean

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -g -ggdb -I ../common/common.a -I ../libs/libs.a -I ../support/support.a
MAKE = make

# for memory-leak tests
VALGRIND = valgrind -s --leak-check=full --show-leak-kinds=all

# compile: $make
server: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server

