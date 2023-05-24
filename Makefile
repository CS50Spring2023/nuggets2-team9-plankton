# Makefile for nuggets modules: grid, game, and server
# compile into another .a library--> call common.a
# server side: get common.a, all other modules, make it an executable


############## build the common.a library ##########

OBJS_lib = grid.o game.o
LIBS = common.a

# build
$(LIBS): $(OBJS_lib)
	ar cr $(LIBS) $(OBJS_lib)

# dependencies
grid.o: grid.h 
game.o: game.h

############## build the server executable ##########

 server.o = server.h support/support.a common/common.a libs/libs.a 

# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST
CFLAGS = -Wall -pedantic -std=c11 -g -ggdb $(TESTING) -I../common
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# compile: $make
server: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server
	rm -f core