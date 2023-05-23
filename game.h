/* 
* game.h - header for module handling game and client methods
*
* TODO: add details here
*
* Team 9: Plankton, May 2023
*/

#ifndef __GAME_H
#define __GAME_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "libs/file.h"
#include "grid.h"
#include "support/message.h"

/**************** global types ****************/
typedef struct client client_t;
typedef struct game game_t;
typedef struct gold_location gold_location_t;


/**************** Functions ****************/

// TODO: add description for each function

client_t* new_player(game_t* game, const addr_t client, char* name);

void update_position(client_t* player, int x, int y);

client_t* new_spectator(game_t* game, const addr_t client);

void delete_client(client_t* client, game_t* game);

game_t* new_game(FILE* map_file, const int maxPlayers);

void end_game();

void update_gold(game_t* game, client_t* player, int x_pos, int y_pos, int goldMaxPiles);

void load_gold(game_t* game, const int goldTotal, const int goldMinPiles, const int goldMaxPiles);

void add_gold_pile(game_t* game, int gold_amt, int piles);

int* nugget_count_array(const int goldMinPiles, const int goldMaxPiles, int goldTotal);


#endif // __GAME_H