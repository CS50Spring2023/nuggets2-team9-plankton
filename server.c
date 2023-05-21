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
#include <ctype.h>


int
main(const int argc, char* argv[])
{
    message_init(stderr);
    message_loop(arg, timeout, handleTimeout, handleStdin, handleMessage);
    message_done();

    return(0);
}

void
handle_movement(client_t* player, char key, game_t* game)
{

    int newPos_x = player->x;
    int newPos_y = player->y;


    switch (key) {
        // update new Pos based on the case
        case 'h': ... code for letter=='B'; break;
        case 'l': ... code for letter=='C'; break;
        case 'j': ... code for letter=='A'; break;
        case 'k': ... code for letter=='B'; break;
        case 'y': ... code for letter=='C'; break;
        case 'u': ... code for letter=='A'; break;
        case 'b': ... code for letter=='B'; break;
        case 'n': ... code for letter=='C'; break;
        default:  ... code for letter not matching any case above.

    }

    char grid_val = get_grid_value(newPos_x, newPos_y);
    if (grid_val == '+' || grid_val == '-' || grid_val == '|' || grid_val == ' '){
        return;
    }
    else if (grid_val == '.' || grid_val == '#'){

    }
    else if (isalpha(grid_val)){
        //
    }
    else if (grid_val == '*'){
        update_gold(game, player, newPos_x, newPos_y, goldMaxPiles);
        // send notify message
        
    }


     
   
}

// make spectator quit
