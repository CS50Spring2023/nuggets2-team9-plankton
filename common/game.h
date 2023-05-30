/* 
* game.h - header for module handling game and client methods
* includes functions to modify the overall status of the game, as well as locate entities within the game
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


/**************** Functions ****************/


/* new_player
 * Inputs:
 *     - game: pointer to the game object
 *     - client: address of the client
 *     - name: name of the player
 * Outputs:
 *     - Returns the newly created player client.
 */
client_t* new_player(game_t* game, addr_t client, char* name);

/* update_position
 * Inputs:
 *     - player: pointer to the player client
 *     - r: row coordinate
 *     - c: column coordinate
 * Outputs:
 *     - None.
 */
void update_position(client_t* player, int r, int c);

/* find_client
 * Inputs:
 *     - clientAddr: address of the client
 *     - game: pointer to the game object
 * Outputs:
 *     - Returns the client if found, NULL otherwise.
 */
client_t* find_client(addr_t clientAddr, game_t* game);

/* find_player
 * Inputs:
 *     - id: player ID
 *     - game: pointer to the game object
 * Outputs:
 *     - Returns the player client if found, NULL otherwise.
 */
client_t* find_player(char id, game_t* game);

/* new_spectator
 * Inputs:
 *     - game: pointer to the game object
 *     - client: address of the client
 * Outputs:
 *     - Returns the newly created spectator client.
 */
client_t* new_spectator(game_t* game, const addr_t client);

/* delete_client
 * Inputs:
 *     - client: pointer to the client to be deleted
 *     - game: pointer to the game object
 * Outputs:
 *     - None.
 */
void delete_client(client_t* client, game_t* game);

/* new_game
 * Inputs:
 *     - map_file: pointer to the file containing the game map
 *     - maxPlayers: maximum number of players allowed in the game
 * Outputs:
 *     - Returns the newly created game object.
 */
game_t* new_game(FILE* map_file, const int maxPlayers);

/* end_game
 * Inputs:
 *     - game: pointer to the game object
 *     - maxGoldPiles: maximum number of gold piles in the game
 * Outputs:
 *     - None.
 */
void end_game(game_t* game, int maxGoldPiles);

/* update_gold
 * Inputs:
 *     - game: pointer to the game object
 *     - player: pointer to the player client
 *     - r: row coordinate
 *     - c: column coordinate
 *     - goldMaxPiles: maximum number of gold piles in the game
 * Outputs:
 *     - Returns the amount of gold obtained by the player.
 */
int update_gold(game_t* game, client_t* player, int r, int c, int goldMaxPiles);

/* load_gold
 * Inputs:
 *     - game: pointer to the game object
 *     - goldTotal: total amount of gold in the game
 *     - goldMinPiles: minimum number of gold piles in the game
 *     - goldMaxPiles: maximum number of gold piles in the game
 * Outputs:
 *     - None.
 */
void load_gold(game_t* game, const int goldTotal, const int goldMinPiles, const int goldMaxPiles);

/* add_gold_pile
 * Inputs:
 *     - game: pointer to the game object
 *     - gold_amt: amount of gold in the new pile
 *     - piles: number of gold piles
 * Outputs:
 *     - None.
 */
void add_gold_pile(game_t* game, int gold_amt, int piles);

/* nugget_count_array
 * Inputs:
 *     - goldMinPiles: minimum number of gold piles in the game
 *     - goldMaxPiles: maximum number of gold piles in the game
 *     - goldTotal: total amount of gold in the game
 * Outputs:
 *     - Returns an array containing the number of gold nuggets in each pile.
 */
int* nugget_count_array(const int goldMinPiles, const int goldMaxPiles, int goldTotal);


#endif // __GAME_H_