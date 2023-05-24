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

#include "../libs/file.h"
#include "../libs/mem.h"

#include "../support/log.h"
#include "../support/message.h"

#include "game.h"
#include "grid.h"

/**************** functions: grid as a 2D array  ****************/

/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into an array of strings, each string in the array represents a row of the map
*/
char** 
load_grid(FILE* fp)
{
    // defensive check: file isn't null
    if (fp == NULL){
        fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
        exit(1);
    }

    // Create an array of strings, each string is one row of the map
    char** grid = mem_malloc_assert(file_numLines(fp) * sizeof(char*), "Error allocating memory in load_grid.\n");
    char* newRow = NULL;
    int row = 0; // keeps track of our position while filling in grid array
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
   // Create string for string version of grid map, must have rows*columns characters plus new lines
   char* display = mem_malloc_assert((rows*columns) + rows - 1, "Error allocating memory in grid_toStr.\n");

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
    // this is a string, in a format that can be sent directly to the client 
   return display;
}

/*
* assign_random_spot: assigns a "thing" to a random place on the grid where it's allowed
* Takes in a char** global_grid, number of rows, number of columns, a "thing" to be placed, as well as x and y pointers to be filled
*/
void
assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_x, int* spot_y)
{
    // assigns a "thing" to a random open spot, can be used to place either gold or a player
    bool placed = false;
    while (placed == false){
        // get a pseudo-random x coordinate and y coordinate from stdlib
        int x = rand() % columns;
        int y = rand() % rows;
        // try to place the "thing" there
        if (strcmp(grid[x][y], '.')==0){
            grid[x][y]=thing;
            placed = true;
            // assign spot x and y
            *spot_x = x;
            *spot_y = y;
        }
        // try again with another random spot if it didn't work
    }
}


/*
* update_player_grid: 
* Takes in a char** player_grid, char** global_grid, int representing player rows, and int representing player columns
* outputs nothing 
* calls getWalls to find room boundaries
* for each boundary, calls isVisible to update player's grid
*/
void
update_player_grid(char** player_grid, game_t* game, int pr, int pc)
{
    // change the player's '@' symbol to their new position.
    player_grid[pr][pc]='@';

    // then, change their local grid display in reference to where they are and what they can see

    // get walls
    char** walls = mem_malloc_assert(sizeof(game->rows * sizeof(char*)), "Error allocating memory in getWalls.\n");
    walls = getWalls(game, game->grid, pr, pc);

	// for each wall point (wr, wc) in walls array
    for(int wr = 0; wr < ; wr++) {
        for(int wc = 0; wc < ; wc++) {
            isVisible(game, player_grid, pr, pc, wr, wc);
        }
    }

}


/*
* update_grids: given a game object, loops through all clients and update their grids
*/
void
update_grids(game_t* game)
{
    char** clients = game->clients;
    // loops thru all players
    for (int i=0; i<game->playersJoined; i++){
        // calls update player grid 
        update_player_grid();
    }
}

/*
* get_grid_value: takes in a grid object, x value, y value, number of rows, number of columns
* outputs whatever symbol is at that point in the grid
*
*/
char* 
get_grid_value(game_t* game, int x, int y)
{
    return game->grid[x][y];
}


/*
* change_spot: takes in grid, coordinate values, and a symbol
* changes whatever is at that spot to the new symbol
*/
void 
change_spot(game_t* game, int x, int y, char* symbol)
{
    game->grid[x][y]=symbol;
}


/*
* getWalls: takes in player row & col
* outputs array with room boundaries necessary to determine visibility
*/
char**
getWalls(game_t* game, char** grid, int pr, int pc)
{

    // assuming coord (0,0) is in the top left corner

    // get walls
    char** walls = mem_malloc_assert(sizeof(game->rows * sizeof(char*)), "Error allocating memory in getWalls.\n");


    while ( (grid[pr][pc] != "-") || (grid[pr][pc] != "#") || (grid[pr][pc] != "x") ){
        // while inside playing field, go down
        pr++;
    }

    // save element in walls array
    walls[pc][pr] = grid[pr][pc];

    // save coordinates as first coords
    int pr1 = pr;
    int pc1 = pc;


    while ((pr != pr1) && (pc != pc1)){
        
        // if wall type to the right
        if ( (grid[pr][pc+1] == "-") || (grid[pr][pc+1] == "#") || (grid[pr][pc+1] == "x") ){
            // if curr element not also a tunel, don't move
            if (grid[pr][pc] != "#"){
                // move one step in that direction
                pc++;
            }  
        }

        // if wall type up
        else if ( (grid[pr-1][pc] == "-") || (grid[pr-1][pc] == "#") || (grid[pr-1][pc] == "x") ){
            // if curr element not also a tunel, don't move
            if (grid[pr][pc] != "#"){
                // move one step in that direction
                pr--;
            }  
        }  

        // if wall type to the left
        else if ( (grid[pr][pc-1] == "-") || (grid[pr][pc-1] == "#") || (grid[pr][pc-1] == "x") ){
            // if curr element not also a tunel, don't move
            if (grid[pr][pc] != "#"){
                // move one step in that direction
                pc--;
            }  
        }

        // if wall type down
        else if ( (grid[pr+1][pc] == "-") || (grid[pr+1][pc] == "#") || (grid[pr+1][pc] == "x") ){
            // if curr element not also a tunel, don't move
            if (grid[pr][pc] != "#"){
                // move one step in that direction
                pr++;
            }  
        }

        // save element into wall 2d array
        walls[pr][pc] = grid[pr][pc];

    } 

}

/*
* isVisible: takes in player, player row & col, wall (boundary) row & column and game
* checks if wr, wc is visible from position player position pr, pc
* calls helpers which update the player->grid accordingly
* outputs a boolean which is true if the visibility on the player's grid changes (e.g. new points visible, less points visible, new gold seen etc.)
*/
bool
isVisible(game_t* game, char** player_grid, int pr, int pc, int wr, int wc)
{
    // assume (0,0) point in top left corner, as in example presented in REQUIREMENTS.md
    
	bool gridChanged;  // switch for determining if visibility changed
    char** global_grid = game->grid;
    int globalGrid_rows = game->rows;
    int globalGrid_columns = game->columns;

    // set up another grid to player's grid before updates for latter comparison
    char** oldGrid = mem_malloc_assert(globalGrid_rows * sizeof(char*), "Error allocating memory in isVisible.\n");
    oldGrid = player_grid;

    // update player grid
    visCol(global_grid, player_grid, pc, pr, wc, wr);  // visCol handles pc < wc AND pc > wc cases
    visRow(global_grid, player_grid, pr, pc, wr, wc);  // visRow handles pr < wr AND pr > wr cases

    // check if new player map has changed from previous version
    gridChanged = compareGrids(globalGrid_rows, globalGrid_columns, player_grid, oldGrid);

    // boolean, will be used to determine whether a messsage should be sent to this player
    return gridChanged;

}

/* compareGrids: takes in global grid rows & columns, the new player grid, and their former grid
* helper for isVisble to compare current to previous player grids for changes in visibility 
*/
bool 
compareGrids(int rows, int columns, char** playerGrid, char** oldGrid)
{
    // for every gridspot
    for (int i = 0; i < rows; i++){
        for (int j; j < columns; j++){
            // if there has been a change
            if (playerGrid[i][j] != oldGrid[i][j]){
                // return true, a new message needs to be sent to this player
                return true;
            }
        }
    }
    // if nothing has changed, then no message needs to be sent
    return false;
}
   
/* visCol: takes in the player, the player's current column and the column of a wall being investigated
* helper to compute visibility across columns
* outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
*/ 
void
visCol(char** global_grid, char** player_grid, int pc, int pr, int wc, int wr)
{

    float slope;

    // calculate and set slope based on the player in reference to the wall being investigated
	if (pc != wc) {
        slope = (wr-pr)/(wc-pc);
    }

    // if the player is to the left of the wall
    if (pc < wc){
		// check point on each column between and not including pc and wc
	    for (int col = pc+1; col < wc; col++){

            float row = slope*(col-pc) + pr; // compute row

			if (is_integer(row)){  // check if row is integer aka point on grid - also takes care of pr == wr case
				if ( (player_grid[(int)row][col] == "|") || (player_grid[(int)row][col] == "-" ) || (player_grid[(int)row][col] == "x") || (player_grid[(int)row][col] == "#") ){
					// this and all future locations are invisible: set to " "
                    player_grid[(int)row][col] == " ";
                }
                else{
                    player_grid[(int)row][col] = global_grid[(int)row][col];
                }
            }

			else{  // point not on grid
				if ( (player_grid[(int)row-1][col] == ".") || (player_grid[(int)row-1][col] == "*") || (player_grid[(int)row+1][col] == ".") || (player_grid[(int)row+1][col] == "*") ){
					// player grid at row, col gets the global grid's value at row, col
			        player_grid[(int)row][col] = global_grid[(int)row][col];
                }
				else{
                    // this and all future locations are invisible to player: make them " "
                    player_grid[(int)row][col] = " ";
                }
            } 
        }			
    }

    else if (pc > wc){
        // check point on each column between and not including pc and wc
		for (int col = pc-1; col > wc; col--){

            float row = slope*(col-pc) + pr;

            if (is_integer(row)){  // aka point on grid
                if ( (player_grid[(int)row][col] == "|") || (player_grid[(int)row][col] == "-" ) || (player_grid[(int)row][col] == "x") || (player_grid[(int)row][col] == "#") ){
                    // this and all future locations are invisible to player: make them " "
                    player_grid[(int)row][col] = " ";
                }
                else{
                    player_grid[(int)row][col] = global_grid[(int)row][col];
                }
            }

            else{  // point not on grid
                if ( (player_grid[(int)row-1][col] == ".") || (player_grid[(int)row-1][col] == "*") || (player_grid[(int)row+1][col] == ".") || (player_grid[(int)row+1][col] == "*") ){
                    player_grid[(int)row][col] = global_grid[(int)row][col];
                }
                else{
                    // this and all future locations are invisible to player: make them " "
                    player_grid[(int)row][col] = " ";
                }
            }
        }
    }
    
}


/* visRow: takes in the player, the player's current row and the row of a wall being investigated
* helper to compute visibility across rows
* outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
*/ 
void
visRow(char** global_grid, char** player_grid, int pr, int pc, int wr, int wc)
{

    float slope;

    // set slope
	if (pc != wc) {
        slope = (wr-pr)/(wc-pc);
    }
    
    if (pr < wr){
        // check points on each row between and not including pr and wr
        for (int row = pr+1; row < wr; row++){

            float col = pc + (row - pr)/slope;  // compute col

            if (is_integer(col)){  // aka a point on the grid - also takes care of pc == wc case
                if ( (player_grid[row][(int)col] == "|") || (player_grid[row][(int)col] == "-" ) || (player_grid[row][(int)col] == "x") || (player_grid[row][(int)col] == "#") ){
                    // this and all future locations are invisible: set to " "
                    player_grid[row][(int)col] = " ";
                }
                else{
                    // player grid at row, col gets the global grid's value at row, col
                    player_grid[row][(int)col] = global_grid[row][(int)col];
                }
            }

            else{  // point not on grid
                if( (player_grid[row][(int)col-1] == ".") || (player_grid[row][(int)col-1] =="*") || (player_grid[row][(int)col+1] == ".") || (player_grid[row][(int)col+1] == "*") ){
                    // player grid at row, col gets the global grid's value at row, col
                    player_grid[row][(int)col] = global_grid[row][(int)col];
                }

                else{
                    // this and all future locations are invisible: set to " "
                    player_grid[row][(int)col] = " ";
                }
            }
        }
    }
        
    
    else if (pr > wr){
        // check points on each row between and not including pr and wr
        for (int row = pr-1; row > wr; row--){

            float col = pc + (row - pr)/slope;  // compute col

            if (is_integer(col)){  // aka a point on the grid
                if ( (player_grid[row][(int)col] == "|") || (player_grid[row][(int)col] == "-" ) || (player_grid[row][(int)col] == "x") || (player_grid[row][(int)col] == "#") ){
                    // this and all future locations are invisible: skip all future rows and columns
                    player_grid[row][(int)col] = " ";
                }
                else{
                    player_grid[row][(int)col] = global_grid[row][(int)col];
                }
            }

            else{  // point not on grid
                if ( (player_grid[row][(int)col-1] == ".") || (player_grid[row][(int)col-1] =="*") || (player_grid[row][(int)col+1] == ".") || (player_grid[row][(int)col+1] == "*") ){
                    // player grid at row, col gets the global grid's value at row, col
                    player_grid[row][(int)col] = global_grid[row][(int)col];
                }
                else{
                    // this and all future locations are invisible: set to " "
                    player_grid[row][(int)col] = " ";
                }
            }
        }
            
    }
    
}

