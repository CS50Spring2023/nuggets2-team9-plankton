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
        // update new Pos based on the key inputted
        case 'h': newPos_x--; break;
        case 'l': newPos_x++; break;
        case 'j': newPos_y++; break;
        case 'k': newPos_y--; break;
        case 'y': newPos_x--; newPos_y--; break;
        case 'u': newPos_x++; newPos_y--; break;
        case 'b': newPos_x--; newPos_y++; break;
        case 'n': newPos_x++; newPos_y++; break;
        default: exit(1);

    }

    char grid_val = get_grid_value(newPos_x, newPos_y);

    if (grid_val == '+' || grid_val == '-' || grid_val == '|' || grid_val == ' '){
        return;
    }
    else if (grid_val == '.' || grid_val == '#'){
        //change the global grid on the spot they came from back to what it was
        if (player->onTunnel){
            change_spot(game, player->x, player->y, '#');
        }
        else{
            change_spot(game, player->x, player->y, '.');
        }

        player->onTunnel = (grid_val == '#');
        
        // change the global grid on the spot they are now on to be their letter
        change_spot(game, newPos_x, newPos_y, player->id);

        // update the player's position in the player struct
        update_position(player, newPos_x, newPos_y);
        
    }
    else if (isalpha(grid_val)){
        // find the player there using a game function
        player_t* other_player = find_player(grid_val);

        // switch the positions of the two players
        update_position(other_player, player->x, player->y);
        update_position(player, newPos_x, newPos_y);

        // update the player's records on the spot they stand on 
        bool player_OnTunnel = player->onTunnel;
        player->onTunnel = other_player->onTunnel;
        other_player->onTunnel = player_OnTunnel;

        // update the global grid to reflect change
        change_spot(game, player->x, player->y, player->id);
        change_spot(game, other_player->x, other_player->y, other_player->id);


    }
    else if (grid_val == '*'){
        update_gold(game, player, newPos_x, newPos_y, goldMaxPiles);
        // send gold notify message
        //change the global grid on the spot they came from back to what it was
        // change the global grid on the spot they are now on to be their letter
        update_position(player, newPos_x, newPos_y);
    }
   
}

// make spectator quit

// gold notification