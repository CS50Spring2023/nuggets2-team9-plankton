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

#include "libs/file.h"
#include "libs/mem.h"
#include "game.h"

// just to deal with executable compilation error, delete later
int main()
{
    return 1;
}

// /**************** functions: grid as a 2D array  ****************/

/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into an array of strings, each string in the array represents a row of the map
*/

char** load_grid(FILE* fp, int* rowp, int* columnp)
{
    // defensive check: file isn't null
    if (fp == NULL){
        fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
        exit(1);
    }

    // fill pointers with number of rows and columns
    *rowp = file_numLines(fp);               // number of lines in the file signifies the number of rowsv
    char* firstLine = file_readLine(fp);
    *columnp = strlen(firstLine);    // the length of every line (just use the first) signifies the number of columns

    // Create an array of strings, each string is one row of the map
    char** grid = mem_malloc_assert(*rowp * sizeof(char*), "Error allocating memory in load_grid.\n");
    char* newRow = NULL;
    int row = 0; // keeps track of our position while filling in grid array

    // reset pointer to beginning of the file
    rewind(fp);

    // fill in the grid
    while ((newRow = file_readLine(fp)) != NULL){
        grid[row] = newRow;
        row++;
    }
    return grid;
}

/*
* grid_toStr: converts a grid to a string that can be sent to and displayed by the player
* Takes in a char** global_grid and optionally a char** player_grid (which can be null) as well as the number of rows and columns in the grid
* If a player_grid is NULL the output string will just be the global_grid
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

/*
* assign_random_spot: 
* Takes in a char** global_grid, number of rows, number of columns, a "thing" to be placed, as well as x and y ptrs to be filled
*/
void
assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_x, int* spot_y)
{
    // assigns a "thing" to a random open spot, can be used to place either gold or a player
    bool placed = false;
    int max = columns*rows;
    while (placed == false){
        // get a pseudo-random x coordinate and y coordinate from stdlib
        int x = rand() % columns;
        int y = rand() % rows;
        // try to place the "thing" there
        if (strcmp(grid[x, y], '.')==0){
            grid[x, y]=thing;
            placed = true;
            // assign spot x and y
            spot_x = x;
            spot_y = y;
        }
        // try again with another random spot if it didn't work
    }
}

/*
* update_player_grid: 
* Takes in a char** player_grid, char** global_grid, int representing player rows, and int representing player columns
* 
*/
void
update_player_grid(char** player_grid, char** global_grid, int pr, int pc)
{

    /* LOGIC OUTLINE:
    updates player_grid to reflect all visible points
    calls helper method on all points to check if visible-- if so, sets to global value within helper method
    otherwise update points present in player_grid but now invisible to the player to the empty spot, with no item (player/gold) */

	// TODO: figure out how to keep track of boundary/wall elements; maybe make array of boundary/wall elements? !!!!
    // NOTE: i left things out of player view blank assuming each time player moves their visibility map starts blank - maybe not the best - might have to change

    // placeholders: need to figure out how to find wr, wc of current room!!
    int wr = 0;
    int wc = 0;

	// for wall point (wr, wc) in grid array
        isVisible(pr, pc, wr, wc);

}

/*
* update_grids: 
* loops through all clients and updates their grids
*/
void
update_grids(char** global_grid, game_t* game)
{
    char** clients = game->clients;
    // loops thru all players
    for (int i=0; i<game->playersJoined; i++){
        // calls update player grid 
        update_player_grid();
    }
}

/*
* get_symbol: takes in a grid object, x value, y value, number of rows, number of columns
* outputs whatever symbol is at that point in the grid
*
*/
char* get_symbol(char** grid, int rows, int columns, int x, int y)
{
    return grid[x,y];
}


/*
* change_spot: takes in grid, number of rows and columns, coordinate values, and a symbol
* changes whatever is at that spot to the new symbol
*/
void change_spot(char** grid, int rows, int columns, int x, int y, char* symbol)
{
    grid[x,y] = symbol;
}


/*
* isVisible: takes in player, player row & col, wall (boundary) row & column and game
* checks if wr, wc is visible from position player position pr, pc
* calls helpers which update the player->grid accordingly
* outputs a boolean which is true if the visibility on the player's grid changes (e.g. new points visible, less points visible, new gold seen etc.)
*/
bool
isVisible(client_t* player, game_t* game, int pr, int pc, int wr, int wc)
{
    // assume (0,0) point in top left corner, as in example presented in REQUIREMENTS.md
    
	bool gridChanged;  // switch for determining if visibility changed

    // set up another grid to player's grid before updates for latter comparisson
    // CAN WE DO THIS WITHOUT HAVING TO PASS game_t* game TO FUNCTION???
    char** oldGrid = mem_malloc_assert(game->rows * sizeof(char*), "Error allocating memory in isVisible.\n");
    oldGrid = player->grid;
	

    // update player grid
    visCol(player, pc, wc);  // visCol handles pc < wc AND pc > wc cases
    visRow(player, pr, wr);  // visRow handles pr < wr AND pr > wr cases


    // check if new player map has changed from previous version
    gridChanged = compareGrids(game->rows, game->columns, player->grid, oldGrid);

    return gridChanged;

}


/* compareGrids: takes in global grid rows & columns, the new player grid, and their former grid
* helper for isVisble to compare current to previous player grids for changes in visibility 
*/
bool compareGrids(int rows, int columns, char** playerGrid, char** oldGrid){

    for (int i = 0; i < rows; i++){
        for (int j; j < columns; j++){
            if (playerGrid[i][j] != oldGrid[i][j]){
                return true;
            }
        }
    }

    return false;

}

   
/* visCol: takes in the player, the player's current column and the column of a wall being investigated
* helper to compute visibility across columns
* outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
*/ 
void
visCol(client_t* player, int pc, int wc)
{
    // set slope
	if (pc != wc) {
        slope = (wr-pr)/(wc-pc);
    }

    if (pc < wc){
		// check point on each column between and not including pc and wc
	    for (int col = pc+1; col < wc; col++){

            row = slope*(col-pc) + pr; // compute row

			if (is_integer(row)){  // check if row is integer aka point on grid - also takes care of pr == wr case
				if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") || (grid[row][col] == "#") ){
					// this and all future locations are invisible: set to " "
                    player->grid[row][col] == " ";
                }
                else{
                    player->grid[row][col] = grid[row][col];
                }
            }

			else{  // point not on grid
				if ( (grid[(int)row-1][col] == ".") || (grid[(int)row-1][col] == "*") || (grid[(int)row+1][col] == ".") || (grid[(int)row+1][col] == "*") ){
					// player grid at row, col gets the global grid's value at row, col
			        player->grid[row][col] = grid[row][col];
                }
				else{
                    // this and all future locations are invisible to player: make them " "
                    player->grid[row][col] == " ";
                }
            }
            
        }
						
    }

    else if (pc > wc){
        // check point on each column between and not including pc and wc
		for (int col = pc-1; col > wc; col--){

            row = slope*(col-pc) + pr;

            if (is_integer(row)){  // aka point on grid
                if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") || (grid[row][col] == "#") ){
                    // this and all future locations are invisible to player: make them " "
                    player->grid[row][col] == " ";
                }
                else{
                    player->grid[row][col] = grid[row][col];
                }
            }

            else{  // point not on grid
                if ( (grid[(int)row-1][col] == ".") || (grid[(int)row-1][col] == "*") || (grid[(int)row+1][col] == ".") || (grid[(int)row+1][col] == "*") ){
                    player->grid[row][col] = grid[row][col];
                }
                else{
                    // this and all future locations are invisible to player: make them " "
                    player->grid[row][col] == " ";
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
visRow(client_t* player, int pr, int wr)
{
    
    if (pr < wr){
        // check points on each row between and not including pr and wr
        for (int row = pr+1; row < wr; row++){

            col = pc + (row - pr)/slope;  // compute col

            if (is_integer(col)){  // aka a point on the grid - also takes care of pc == wc case
                if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") || (grid[row][col] == "#") ){
                    // this and all future locations are invisible: set to " "
                    player->grid[row][col] == " ";
                }
                else{
                    // player grid at row, col gets the global grid's value at row, col
                    player->grid[row][col] = grid[row][col];
                }
            }

            else{  // point not on grid
                if( (grid[row][(int)col-1] == ".") || (grid[row][(int)col-1] =="*") || (grid[row][(int)col+1] == ".") || (grid[row][(int)col+1] == "*") ){
                    // player grid at row, col gets the global grid's value at row, col
                    player->grid[row][col] = grid[row][col];
                }

                else{
                    // this and all future locations are invisible: set to " "
                    player->grid[row][col] == " ";
                }
            }
        }
    }
        
    
    else if (pr > wr){
        // check points on each row between and not including pr and wr
        for (int row = pr-1; row > wr; row--){

            col = pc + (row - pr)/slope;  // compute col

            if (is_integer(col)){  // aka a point on the grid
                if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") || (grid[row][col] == "#") ){
                    // this and all future locations are invisible: skip all future rows and columns
                    player->grid[row][col] == " ";
                }
                else{
                    player->grid[row][col] = grid[row][col];
                }
            }

            else{  // point not on grid
                if ( (grid[row][(int)col-1] == ".") || (grid[row][(int)col-1] =="*") || (grid[row][(int)col+1] == ".") || (grid[row][(int)col+1] == "*") ){
                    // player grid at row, col gets the global grid's value at row, col
                    player->grid[row][col] = grid[row][col];
                }
                else{
                    // this and all future locations are invisible: set to " "
                    player->grid[row][col] == " ";
                }
            }
        }
            
    }
    
}


/**************** OLD FUNCTIONS  ****************/
// rewritten for the new grid format: commented out, everything should be above this line

/*
* load_grid: takes in a FILE* fp to a map file which is assumed to be valid
* Reads the file into a string, with with a null character representing the end of every row
*
*/

// char* load_grid(FILE* fp, int* rowp, int* columnp)
// {

//     if (fp == NULL){
//         fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
//         exit(1);
//     }

//     // get rows and columns
//     rowp = file_numLines(fp);               // number of lines in the file signifies the number of rows
//     // store first line in a string
//     char* firstLine = file_readLine(fp);
//     columnp = strlen(firstLine);    // the length of every line (just use the first) signifies the number of columns

//     // Create string for to hold the grid, must have rows*columns characters plus memory for new lines
//     char* grid = mem_malloc_assert((rows*columns) + rows - 1, "Error allocating memory in load_grid.\n");
	
//     // add the first line to the map, so that it isn't skipped
// 	for (int i = 0; i < columns; i++){
// 	    // calculate the appropriate index and store the char in the grid string
// 	    grid[strPos]=firstLine[i];
// 	}

//     char* line = NULL;
//     int rowIndex = 1; // what "row" of the map are we currently adding to the grid

//     // loop through every row in the map
//     while ((line = file_readLine(fp)) != NULL){
//         // loop through every point in the row
//         for (int i = 0; i < columns; i++){
//             // calculate the appropriate index and store the char in the grid string
//             int strPos = rowIndex * i;
//             grid[strPos]=line[i];
//         }
//         // add null char if this isn't the last row
//         if (rowIndex < rows){
//             int nullPos = (rowIndex * columns)+1;
//             grid[nullPos]= '\n';
//         }
//         rowIndex++;
//     }
//     return grid;
// }


// void
// assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_x, int* spot_y)
// {
//     // assigns a "thing" to a random open spot, can be used to place either gold or a player
//     bool placed = false;

//     int max = columns*rows;
//     while (placed == false){
//         // get a pseudo-random number from stdlib, between 0 and the largest index
//         int r = rand() % max;
//         // try to place the "thing" there
//         if (strcmp(grid[r], '.')==0){
//             grid[r]=='*';
//             placed = true;
//             // assign spot x and y
//             player->x = r/columns;
//             player->y = r%rows;
//         }
//         // try again with another random spot if it didn't work
//     }
// }

// add a function to compute x and y value based on index


// void
// update_player_grid(char** player_grid, char** global_grid, int pr, int pc)
// {
//     /* LOGIC OUTLINE:
//     updates player_grid to reflect all visible points
//     calls helper method on all points to check if visible-- if so, sets to global value within helper method
//     otherwise update points present in player_grid but now invisible to the player to the empty spot, with no item (player/gold) */

// 	// TODO: figure out how to keep track of boundary/wall elements; maybe make array of boundary/wall elements? !!!!
//     // NOTE: i left things out of player view blank assuming each time player moves their visibility map starts blank - maybe not the best - might have to change

//     // placeholders: need to figure out how to find wr, wc of current room!!
//     int wr = 0;
//     int wc = 0;

// 	// for wall point (wr, wc) in grid array
//         isVisible(pr, pc, wr, wc);

// }

// void
// update_grids(char** global_grid, game_t* game)
// {
//     char** players = game->players;
//     // loops thru all players
//     for (int i=0; i<game->playersJoined; i++){
//          // calls update player grid 
//          update_player_grid();
//     }
// }


// bool
// isVisible(int pr, int pc, int wr, int wc)
// {
    // checks if wr, wc is visible from position player position pr, pc
    // assume (0,0) point in top right corner, as in example presented in REQUIREMENTS.md

    // for wall element (wr, wc) in grid array{
    /*
	invisible = 0;  // switch for determining visibility
		
	if (pc != wc) {
        slope = (wr-pr)/(wc-pc);
    }
        

    if (pr == wr){
        if (pc != wc){
            visCol(pc, wc);  // visCol handles pc < wc AND pc > wc cases
        }
    }	
    
    else {  // pr > wr OR pr < wr
        if (pc != wc){
            visRow(pr, wr);  // visRow handles pr < wr AND pr > wr cases
            if (invisible == false){
                visCol(pc, wc);  // visCol handles pc < wc AND pc > wc cases
            }
        }

        else{  // pc == wc
            if (pr < wr){
                // check each row between and not including pr and wr
                for (int row = pr+1; row < wr; row++){
                    pcOnwc(); 
                }
            }
        }
        
        else if (pr > wr){
            // check each row between and not including pr and wr
            for (int row = pr+1; row < wr; row++){
                pcOnwc();
            }
        }
    }
    */
//}

   

// bool
// visCol(int pc, int wc)
// {
    
//     bool invisible = false;
//     if (pc < wc){
// 		// check each column between and not including pc and wc
// 	    for (int col = pc+1; col < wc; col++){
// 			if (is_integer(col)){  // check if col is integer aka point on grid
// 				if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") ){
// 					// this and all future locations are invisible: skip all future rows and columns
// 					// invisible = true;
//                     player->grid[row][col] == " ";
//                 }
//             }

// 			else{  // point not on grid
// 				if ( (grid[row][(int)col-1] == ".") || (grid[row][(int)col-1] =="*") || (grid[row][(int)col+1] == ".") || (grid[row][(int)col+1] == "*") ){
// 					// player grid at row, col gets the global grid's value at row, col
// 			        player->grid[row][col] = grid[row][col];
//                 }
// 				else{
//                     // this and all future locations are invisible to player: make them " "
//                     player->grid[row][col] == " ";
//                     // invisible = true
//                 }
//             }

                
//             //if (invisible == false){
// 				// player grid at row, col gets the global grid's value at row, col
// 			    //player_grid[row][col] = grid[row][col]
//             //}
//             //else return invisible;
            
//         }
						
//     }

//     else if (pc > wc){
//         // check each column between and not including pc and wc
// 		for (int col = pc-1; col > wc; col--){
//             if (is_integer(col)){  // aka point on grid
//                 if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") ){
//                     // this and all future locations are invisible to player: make them " "
//                     player->grid[row][col] == " ";
//                     // invisible = true;
//                 }
//             }
//             else{  // point not on grid
//                 if ( (grid[row][(int)col-1] == ".") || (grid[row][(int)col-1] =="*") || (grid[row][(int)col+1] == ".") || (grid[row][(int)col+1] == "*") ){
//                     player->grid[row][col] = grid[row][col];
//                 }
//                 else{
//                     // this and all future locations are invisible: skip all future rows and columns
//                     invisible = true;
//                 }
//             }
//             if (invisible == false){
//                 // player grid at row, col gets the global grid's value at row, col
//                 player_grid[row][col] = grid[row][col]
//             }
//             else return invisible;
//         }
//     }
//     return invisible;
    
// }

// bool
// visRow(int pr, int wr)
// {
    
//     bool invisible = false

//     if pr < wr
//         // check each row between and not including pr and wr
// 				for (int row = pr+1; row < wr; row++)
// 					if row is integer  // aka a point on the grid
// 						if grid[row][col] is "|" or "-" or "x"
// 							// this and all future locations are invisible: skip all future rows and columns
// 							invisible = true
// 					else  // point not on grid
// 						if ( ((int)row-1 is "." or "*") or ((int)row+1 is "." or "*")

// 						else
// 							// this and all future locations are invisible: skip all future rows and columns
// 							invisible = true

// 					if (invisible == true)
// 						exit for loop: all following points are invisible
// 					else
// 						// player grid at row, col gets the global grid's value at row, col
// 						player_grid[row][col] = grid[row][col]
    
//     else if pr > wr
//         // check each row between and not including pr and wr
// 				for (int row = pr-1; row > wr; row--)
// 					col = pc + (row - pr)/slope
// 					if col is integer  // aka a point on the grid
// 						if grid[row][col] is "|" or "-" or "x"
// 							// this and all future locations are invisible: skip all future rows and columns
// 							invisible = true
// 					else  // point not on grid
// 						if ( ((int)col-1 is "." or "*") or ((int)col+1 is "." or "*")
//                             continue
// 						else
// 							// this and all future locations are invisible: skip all future rows and columns
// 							invisible = true

// 					if (invisible == true)
// 						exit for loop: all following points are invisible
// 					else
// 						// player grid at row, col gets the global grid's value at row, col
// 						player_grid[row][col] = grid[row][col]
    
//     return invisible
    
// }

// /* pcONwc: takes in the player, a row, and the player's current column
// * helper to compute visibility when the player is on the same column as the wall
// * outputs nothing, but updates the player->grid to either the global grid if point is visible or to " " otherwise
// */ 
// void pcONwc (client_t* player, int row, int col)

// {
//     // point always on grid in this case
//     if ( (grid[row][col] == "|") || (grid[row][col] == "-" ) || (grid[row][col] == "x") || (grid[row][col] == "#") ){
//         // this and all future locations are invisible: set to " "
//         player->grid[row][col] == " ";
//     }
//     else{
//         player->grid[row][col] = grid[row][col];
//     }
// }


/**************** old functions  ****************/

// /*
// * load_grid: takes in a FILE* fp to a map file which is assumed to be valid
// * Reads the file into an array of strings, each string in the array represents a row of the map
// *
// */
// char**
// load_grid(FILE* fp)
// {
//     if (fp == NULL){
//         fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
//         exit(1);
//     }

//     // Create an array of strings, each string is one row of the map
//     char** grid = mem_malloc_assert(file_numLines(fp) * sizeof(char*), "Error allocating memory in load_grid.\n");

//     char* newRow = NULL;
//     int row = 0; // keeps track of our position while filling in grid array

//     while ((newRow = file_readLine(fp)) != NULL){
//         grid[row] = newRow;
//         row++;
//     }

//     return grid;

// }


// idk if i need this
// char**
// empty_player_grid(int rows, int columns)
// {
//     char** grid = mem_malloc_assert(rows * sizeof(char*), "Error allocating memory in empty_player_grid.\n");

//     for (int r = 0; r < rows; r++){

//         for(int c = 0; c < columns; c++){

//         }
//     }
// }

/*
* grid_toStr: converts a grid to a string that can be sent to and displayed by the player
* Takes in a char** global_grid and optionally a char** player_grid (which can be null) as well as the number of rows and columns in the grid
* If a player_grid is NULL the output string will just be the global_grid
*
*/
// char*
// grid_toStr(char** global_grid, char** player_grid, int rows, int columns)
// {
//     // Create string for string version of grid map, must have rows*columns characters plus new lines
//    char* display = mem_malloc_assert((row*columns) + rows - 1, "Error allocating memory in grid_toStr.\n");

//    for (int r = 0; r < rows; r++){
//         for (int c = 0; c < columns; c++){
//             // adding 1 because null terminator isn't included in column count
//             display[(r* (columns + 1)) + c] = global_grid[r][c];
           
//             if (player_grid != NULL){
//                 // if a player grid was passed in, set to player_grid value
//                 display[(r* (columns + 1)) + c] = player_grid[r][c];
//             }
//         }
//         // don't add a new line after the last row
//         if (r < rows - 1){
//             display[(r* (columns + 1)) + columns + 1] = '\n'; // add new line to the end of each row
//         }
//     }

//    return display;

// }
