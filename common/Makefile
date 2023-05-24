# Makefile for 'common.a'
# CS50 project 'Nuggets'
#
# Plankton, May 2023
#

############## build the common.a library ##########

OBJS = grid.o game.o
LIBS = common.a

# build
$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

# dependencies
grid.o: grid.h 
game.o: game.h
