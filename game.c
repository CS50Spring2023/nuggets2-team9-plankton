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
typedef struct player {
  char player[26];  // array of players from 0-25
  // each player slot points to: id, real_name, gold, and the player's personal grid array;

} player_t;



/**************** global player functions ****************/
player_t*
player_new(void){
    // create a new player; use every time a new client wants to join, if player limit unreached
}

player_t*
player_delete(player_t* player){  // do we also need: void (*itemdelete)(void *item)
    // delete player; when server receives QUIT message OR when all nuggests have been collected
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