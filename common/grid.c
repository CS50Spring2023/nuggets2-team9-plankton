/*
grid.c 
module for handling the global and player grids

Team 9: Plankton, May 2023
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "../libs/file.h"
#include "../libs/mem.h"

#include "../support/log.h"
#include "../support/message.h"

#include "structs.h"
#include "game.h"
#include "grid.h"

/**************** functions: grid as a 2D array  ****************/

/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into an array of strings, each string in the array represents a row of the map
*/
char** 
load_grid(FILE* fp, int* rows, int* columns)
{
    // defensive check: file isn't null
    if (fp == NULL){
        fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
        exit(1);
    }

    // Create an array of strings, each string is one row of the map
    *rows = file_numLines(fp);
    char** grid = mem_malloc_assert(*rows * sizeof(char*), "Error allocating memory in load_grid.\n");
    char* newRow = NULL;
    int row = 0; // keeps track of our position while filling in grid array

    char* line = file_readLine(fp);
    *columns = strlen(line);
    mem_free(line); // responsible for freeing this memory

    rewind(fp);  // reset pointer to beginning of the file

    while ((newRow = file_readLine(fp)) != NULL){
        grid[row] = newRow;
        row++;
    }
    // this is a 2D character array

    return grid;
}

/*
* grid_toStr: converts a grid to a string that can be sent to and displayed by the client
* Takes in a char** global_grid and optionally a char** player_grid (which can be null) as well as the number of rows and columns in the grid
* If a player_grid is NULL the output string will just be the global_grid
*/
char*
grid_toStr(char** global_grid, char** player_grid, int rows, int columns)
{
   // Create string for string version of grid map, must have rows*columns characters plus new lines & a terminating null
   char* display = mem_malloc_assert((rows* (columns + 1)) , "Error allocating memory in grid_toStr.\n");

   for (int r = 0; r < rows; r++){
        for (int c = 0; c < columns; c++){
            // adding 1 because new line isn't included in column count
            display[(r * (columns + 1)) + c] = global_grid[r][c];
           
            if (player_grid != NULL){
                // if a player grid was passed in, set to player_grid value
                display[(r* (columns + 1)) + c] = player_grid[r][c];
            }
        }

        display[(r* (columns + 1)) + columns] = '\n'; // add new line to the end of each row
    
    }

    display[(rows*columns) + rows - 1] = '\0'; // null terminate string

    // this is a string, in a format that can be sent directly to the client 
   return display;
}

/*
* assign_random_spot: assigns a "thing" to a random place on the grid where it's allowed
* Takes in a char** global_grid, number of rows, number of columns, a "thing" to be placed, as well as x and y pointers to be filled
*/
void
assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_r, int* spot_c)
{
    // assigns a "thing" to a random open spot, can be used to place either gold or a player
    bool placed = false;
    int r;
    int c;
    srand(time(NULL));

    while (!placed){
        // get a pseudo-random x coordinate and y coordinate from stdlib

        r = rand() % rows;
        c = rand() % columns;

        // try to place the "thing" there

        char value = grid[r][c];

        if (value == '.'){

            grid[r][c] = thing;

            placed = true;

            // assign spot x and y
            *spot_r = r;
            *spot_c = c;
        }
        // try again with another random spot if it didn't work
    }
}

/*
* get_grid_value: takes in a grid object, x value, y value, number of rows, number of columns
* outputs whatever symbol is at that point in the grid
*
*/
char 
get_grid_value(game_t* game, int r, int c)
{
    return game->grid[r][c];
}


/*
* change_spot: takes in grid, coordinate values, and a symbol
* changes whatever is at that spot to the new symbol
*/
void 
change_spot(game_t* game, int r, int c, char symbol)
{
    game->grid[r][c] = symbol;
}


/**************** NEW VISIBILITY FUNCTIONS ****************/



/*
* isOpen: takes in grid, player column, player row, spot column, spot row
* computes whether a spot is visible, based on where the player is
*/
bool static isOpen(game_t* game, const int c, const int r){
    if( '.' == get_grid_value(game, c, r) || '*' == get_grid_value(game, c, r) ){
        return true;
    }
    else if( isalpha(get_grid_value(game,c,r))){
        return true;
    }
    return false;
}

/*
* is_integer: takes in a value, determines whether it's an integer, returns boolean
*/
bool static is_integer(float num){
    int convertedNum = (int)num;
    return (convertedNum == num);
}

/*
* isVisible: takes in grid, player column, player row, spot column, spot row
* computes whether a spot is visible, based on where the player is
*/
bool is_visible(game_t* game, const int playerColumn, const int playerRow, const int column, const int row) {

    // char** grid = game->grid;

    // calculate difference between where the player is and where the spot is
    int changeY = row - playerRow;
    int changeX = column - playerColumn;
    int slope = 0;
    int constant, columnStart, columnEnd, rowStart, rowEnd;

    // if the x coordinate doesn't change (the line is vertical)
    if (changeX == 0) { 
        columnStart = 0;
        columnEnd = 0;
        // either move up or down based on where the player is at
        if (playerRow > row) {
            rowStart = row + 1;
            rowEnd = playerRow;
        } else {
            rowStart = playerRow + 1;
            rowEnd = row;
        }
    // if the y coordinate doesn't change (the line is horizontal)
    } else if (changeY == 0) { 

        rowStart = 0;
        rowEnd = 0;

        // either move left or right based on where the player is at 
        if (playerColumn > column) {
            columnStart = column + 1;
            columnEnd = playerColumn;
        } else {
            columnStart = playerColumn + 1;
            columnEnd = column;
        }
    
    // if both the x and y coordinate change (the line is diagonal)
    } else {
        // calculate slope: rise over run
        slope = changeY / changeX;
        constant = playerRow - (slope * playerColumn);
        columnStart = 0;
        columnEnd = 0;
        rowStart = 0;
        rowEnd = 0;

        if (playerColumn > column) {
            columnStart = column + 1;
            columnEnd = playerColumn;
        } else if (playerColumn < column) {
            columnStart = playerColumn + 1;
            columnEnd = column;
        }

        if (playerRow > row) {
            rowStart = row + 1;
            rowEnd = playerRow;
        } else if (playerRow < row) {
            rowStart = playerRow + 1;
            rowEnd = row;
        }
    }

    for (; columnStart < columnEnd; columnStart++) {
        float newY;

        // if the line is horizontal
        if (rowStart == 0 && rowEnd == 0) {
            newY = playerRow;
        } else {
            newY = slope * columnStart + constant;
        }

        // check for grid point
        if ((is_integer(columnStart)) && (newY)) {
            if (!isOpen(game, columnStart, newY)) {
                return false;
            }
        } else {
            // if not a grid point, check the top and bottom
            if (!(isOpen(game, columnStart, floor(newY)) || isOpen(game, columnStart, ceil(newY)))) {
                return false;
            }
        }
    }

    for (; rowStart < rowEnd; rowStart++) {
        float newX;

        // if the line is vertical
        if (columnStart == 0 && columnEnd == 0) {
            newX = playerColumn;
        } else {
            newX = (rowStart - constant) / slope;
        }

        // check that both points are ints
        if ((is_integer(newX)) && (is_integer(rowStart))){
            // if it's not an "open" point, then return false
            if (!isOpen(game, newX, rowStart)) {
                return false;
            }
        }
        else {
            // if both points are not ints, check the top and bottom for obstructions
            if (!(isOpen(game, floor(newX), rowStart) || isOpen(game, ceil(newX), rowStart))) {
                return false;
            }
        }
    }
    // spot is visible
    return true;
}

/*
* get_player_visible: loops over every position in the grid and calls "is_visible", changes what is stored at each grid point accordingly
*/
void get_player_visible(game_t* game, int columns, int rows, int x, int y){
    // for every column (x value)
    for (int i = 0; i < columns ; i++){
        // for every row (y value)
        for (int j = 0; j < rows+1 ; j++){
            // if this is where the player now is, update the location of the "@" symbol
            if (x == i && j == y){
                change_spot(game, i,j, '@');
            // if it's not where the player is currently standing, and the spot is not visible
            } else if (!is_visible(game, x,y, i, j)) {
                // change its representation on the player grid to " "
                change_spot(game, i, j, ' ');
            }
        }
    }                                                                                                                                                                             
}

/*
* update_grids: given a game object, loops through all clients and update their grids
*/
void update_grids(game_t* game)
{
    // char** clients = malloc(sizeof(char*)*26);
    client_t** clients = game->clients;

    // loops thru all players
    for (int i=1; i < game->playersJoined; i++){

        client_t* client = clients[i];

        if (client != NULL){
           get_player_visible(game, game->columns, game->rows, client->c, client->r);
        }
    }
}

/*
* grid_delete: delete grid, free all memory
*/
void
grid_delete(char** grid, int rows)
{
    for (int r = 0; r < rows; r++){
        mem_free(grid[r]);
    }

    mem_free(grid);
}

