/* 
* game.h - header for module handling game and client methods
*
* TODO: add details here
*
* Team 9: Plankton, May 2023
*/

#ifndef __GAME_H_
#define __GAME_H_

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
#include "grid.h"


// typedef struct gold_location gold_location_t;
// typedef struct client client_t;
// typedef struct game game_t;

/**************** Functions ****************/

// TODO: add description for each function

client_t* new_player(game_t* game, addr_t client, char* name);
void update_position(client_t* player, int r, int c);
client_t* find_client(addr_t clientAddr, game_t* game);
client_t* find_player(char id, game_t* game);
client_t* new_spectator(game_t* game, const addr_t client);
void delete_client(client_t* client, game_t* game);
game_t* new_game(FILE* map_file, const int maxPlayers);
void end_game(game_t* game, int maxGoldPiles);
int update_gold(game_t* game, client_t* player, int r_pos, int c_pos, int goldMaxPiles);
void load_gold(game_t* game, const int goldTotal, const int goldMinPiles, const int goldMaxPiles);
void add_gold_pile(game_t* game, int gold_amt, int piles);
int* nugget_count_array(const int goldMinPiles, const int goldMaxPiles, int goldTotal);

#endif // __GAME_H_