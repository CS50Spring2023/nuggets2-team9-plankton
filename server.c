//
//
//
//


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "libs/file.h"
#include "support/log.h"
#include "support/message.h"


int
main(const int argc, char* argv[])
{
    message_init(stderr);
    message_loop(arg, timeout, handleTimeout, handleStdin, handleMessage);
    message_done();

    return(0);
}

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

// make spectator quit
