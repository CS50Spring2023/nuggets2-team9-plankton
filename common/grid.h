/*
grid.c 
TODO: add details here

Team 9: Plankton, May 2023
*/

#ifndef __GRID_H_
#define __GRID_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../libs/file.h"
#include "../libs/mem.h"

#include "../support/log.h"
#include "../support/message.h"

#include "structs.h"
#include "game.h"


/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into an array of strings, each string in the array represents a row of the map
*/
char** load_grid(FILE* fp, int* rowp, int* columnp);


/*
* grid_toStr: converts a grid to a string that can be sent to and displayed by the player
* Takes in a char** global_grid and optionally a char** player_grid (which can be null) as well as the number of rows and columns in the grid
* If a player_grid is NULL the output string will just be the global_grid
*/
char* grid_toStr(char** global_grid, char** player_grid, int rows, int columns);


/*
* assign_random_spot: 
* Takes in a char** global_grid, number of rows, number of columns, a "thing" to be placed, as well as x and y ptrs to be filled
*/
void assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_x, int* spot_y);


/*
* update_player_grid: 
* Takes in a char** player_grid, char** global_grid, int representing player rows, and int representing player columns
*/
void update_player_grid(char** player_grid, game_t* game, int pr, int pc);


/*
* update_grids: 
* loops through all clients and updates their grids
*/
void update_grids(char** global_grid, game_t* game);


/*
* get_symbol: takes in a grid object, x value, y value, number of rows, number of columns
* outputs whatever symbol is at that point in the grid
*
*/
char get_grid_value(game_t* game, int x, int y);


/*
* change_spot: takes in grid, number of rows and columns, coordinate values, and a symbol
* changes whatever is at that spot to the new symbol
*/
void change_spot(game_t* game, int x, int y, char symbol);


/*
* getWalls: takes in player row & col
* outputs array with room boundaries necessary to determine visibility
*/
// char** getWalls(game_t* game, char** grid, int pr, int pc);


/*
* isVisible: takes in player, player row & col, wall (boundary) row & column and game
* checks if wr, wc is visible from position player position pr, pc
* calls helpers which update the player->grid accordingly
* outputs a boolean which is true if the visibility on the player's grid changes (e.g. new points visible, less points visible, new gold seen etc.)
*/
// bool isVisible(game_t* game, char** player_grid, int pr, int pc, int wr, int wc);


/* compareGrids: takes in global grid rows & columns, the new player grid, and their former grid
* helper for isVisble to compare current to previous player grids for changes in visibility 
*/
// bool compareGrids(int rows, int columns, char** playerGrid, char** oldGrid);


/* visCol: takes in the player, the player's current column and the column of a wall being investigated
* helper to compute visibility across columns
* outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
*/ 
// void visCol(char** global_grid, char** player_grid, int pc, int pr, int wc, int wr);


/* visRow: takes in the player, the player's current row and the row of a wall being investigated
* helper to compute visibility across rows
* outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
*/ 
// void visRow(char** global_grid, char** player_grid, int pr, int pc, int wr, int wc);

void grid_delete(char** grid, int rows);


#endif // __GRID_H_