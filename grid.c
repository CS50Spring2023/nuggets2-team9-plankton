//
//
//
//


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "libs/file.h"
#include "libs/mem.h"


/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into an array of strings, each string in the array represents a row of the map
*
*/
char**
load_grid(FILE* fp)
{
    if (fp == NULL){
        fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
        exit(1);
    }

    // Create an array of strings, each string is one row of the map
    char** grid = mem_malloc_assert(file_numLines(fp) * sizeof(char*), "Error allocating memory in load_grid.\n");

    char* newRow = NULL;
    int row = 0; // keeps track of our position while filling in grid array

    while ((newRow = file_readLine(fp)) != NULL){
        grid[row] = newRow;
        row++;
    }

    return grid;

}


/*
* grid_toStr: converts a grid to a string that can be sent to and displayed by the client
* Takes in a char** global_grid and optionally a char** player_grid (which can be null) as well as the number of rows and columns in the grid
* If a player_grid is NULL the output string will just be the global_grid
*
*/
char*
grid_toStr(char** global_grid, char** player_grid, int rows, int columns)
{
    // Create string for string version of grid map, must have rows*columns characters plus new lines
   char* display = mem_malloc_assert((row*columns) + rows - 1, "Error allocating memory in grid_toStr.\n");

   for (int r = 0; r < rows; r++){
        for (int c = 0; c < columns; c++){
            // adding 1 because null terminator isn't included in column count
            display[(r* (columns + 1)) + c] = global_grid[r][c];
           
            if (player_grid != NULL){
                // if a player grid was passed in, set to player_grid value
                display[(r* (columns + 1)) + c] = player_grid[r][c];
            }
        }
        // don't add a new line after the last row
        if (r < rows - 1){
            display[(r* (columns + 1)) + columns + 1] = '\n'; // add new line to the end of each row
        }
    }

   return display;

}

void
assign_random_spot(char** grid, int rows, int columns, char thing)
{
    // assigns a "thing" to a random open spot, can be used to place either gold or a player

}

void
update_player_grid(char** player_grid, char** global_grind, int r, int c)
{
    // updates player_grid to reflect all visible points
    // calls helper method on all points to check if visible-- sets to global value if so
    // otherwise update points present in player_grid but now invisible to the player to the empty spot, with no item (player/gold)
}

bool
isVisible(int pos_X, int pos_Y, int r, int c)
{
    // checks if r, c is visible from position x, y
}