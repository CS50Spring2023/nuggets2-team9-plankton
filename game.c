/*
game.c 
TODO: add details here

Team 9: Plankton, April 2023
*/

/**************** local types ****************/
typedef struct playernode {
    char* id;
    char* real_name;
    int* gold;
    char** grid;
    
} playernode_t;


/**************** global types ****************/
typedef struct players {
  char players[26];  // array of players from 0-25
  // each player slot points to: id, real_name, gold, and the player's personal grid array;

} players_t;


/**************** local functions ****************/
/* not visible outside this file */
static playernode_t*  
playernode_new(int players_array_index)
{
    // add playernode elements to specific players_array index
}

static playernode_t*  
playernode_delete(int players_array_index)
{
    // delete playernode at specific index when server receives QUIT message
}


/**************** global player functions ****************/
players_t*
players_new(void){
    // create a new player; use every time a new client wants to join, if player limit unreached
}

players_t*
players_delete(players_t* players_array){  // do we also need: void (*itemdelete)(void *item)
    // delete players array; when all nuggests have been collected OR if all players have quit the game
}

// updating player handled in handle_movement function


/**************** global movement functions ****************/
void
handle_movement(player struct, key, global grid, game struct)
{
//    check whats is in the spot we want to move to
//    If boundary
//      Don’t move


//    If gold
// increment player’s purse gold count
// change grid representation at location to “.”
// decrement count of unclaimed gold
// Call update gold to send message from server


//    If player
//    	Update global grid to reflect swap
// 	Update grids of the players changed


//     Call update grid
     
   
}