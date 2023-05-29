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
#include <math.h>

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
char** load_grid(FILE* fp, int* rows, int* columns);

char** load_player_grid(game_t* game);

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

char get_grid_value(game_t* game, int r, int c);

/*
* update_player_grid: 
* Takes in a char** player_grid, char** global_grid, int representing player rows, and int representing player columns
*/
void update_player_grid(char** player_grid, game_t* game, int pr, int pc);

void change_spot(game_t* game, int r, int c, char symbol);

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
* is_visible: takes in grid, player column, player row, spot column, spot row
* computes whether a spot is visible, based on where the player is
*/
bool is_visible(game_t* game, const int playerColumn, const int playerRow, const int column, const int row);

/*
* get_player_visible: loops over every position in the grid and calls "is_visible", changes what is stored at each grid point accordingly
*/
bool get_player_visible(game_t* game, client_t* player);

void grid_delete(char** grid, int rows);


#endif // __GRID_H_