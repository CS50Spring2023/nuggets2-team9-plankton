/*
game.c 
TODO: add details here

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


// a single player's struct
typedef struct client {
    const addr_t clientAddr;
    bool isSpectator;
    char id;
    char* real_name;
    int gold;
    int x;
    int y;
    char** grid;
    bool onTunnel;
    int clientsArr_Idx;
    
} client_t;


// game struct
typedef struct game {
    char** grid;
    client_t* clients;
    int goldRemaining;
    int playersJoined;
    bool spectatorActive;
    int rows;
    int columns;
    gold_location_t* locations;

} game_t;

typedef struct gold_location {
    int x;
    int y;
    int nuggetCount;
} gold_location_t;



client_t*
new_player(game_t* game, const addr_t client, char* name)
{
    client_t* player = mem_malloc_assert(sizeof(client_t), "Error allocating memory in new_player.\n");
    char* alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    player->clientAddr = client;
    player->isSpectator = false;
    player->id = alpha[game->playersJoined];
    player->real_name = mem_malloc_assert(strlen(name) + 1, "Error allocating memory in new_player.\n");
    strcpy(player->real_name, name);
    player->gold = 0;
    player->grid = mem_malloc_assert(game->rows * sizeof(char*), "Error allocating memory in new_player.\n");
    player->onTunnel = false;

    game->clients[playersJoined + 1] = player;
    player->clientsArr_Idx = playersJoined + 1;
    (game->playersJoined)++;
    
    // assign player to a random spot
    assign_random_spot(game->grid, game->rows, game->columns, player);
    
    // update visibility here
    
}

// add update position function
void
update_position(client_t* player, int x, int y)
{
    player->x = x;
    player->y = y;
}

// add find client
client_t*
find_client(const addr_t clientAddr, game_t* game)
{
    for (int i = 0; i < game->playersJoined + 1; i++){
        if ((game->clients)[i] != NULL){
            // can addresses be compared like this?
            if (((game->clients)[i])->clientAddr == clientAddr){
                return (game->clients)[i];
                break;
            }   
        } 
    }

    return NULL;

}

// only for finding a player
client_t*
find_player(char id, game_t* game)
{
    // find the player with the passed ID
    for (int i = 1; i < game->playersJoined + 1; i++){
        if ((game->clients)[i] != NULL){
            if (((game->clients)[i])->id == id){
                return (game->clients)[i];
                break;
            }
        }
    }
    return NULL;
}


client_t*
new_spectator(game_t* game, const addr_t client)
{
    if (game->spectatorActive){
        send_quitMsg(game->clients[0], 0, true);
        delete_client((game->clients)[0], game);
        game->spectatorActive = false;
    }

    client_t* spectator = mem_malloc_assert(sizeof(client_t), "Error allocating memory in new_spectator.\n");
    spectator->isSpectator = true;
    spectator->clientAddr = client;
    spectator->id = '$';
    spectator->grid = NULL;
    spectator->gold = 0;
    spectator->real_name = NULL;
    spectator->onTunnel = false;
    spectator->clientsArr_Idx = 0;

    (game->clients)[0] = spectator;
    game->spectatorActive = true;

    return spectator;

}


// not done
void
delete_client(client_t* client, game_t* game)
{
    if (player->real_name != NULL){
        mem_free(player->real_name);
    }
    
    // grid delete function call

    (game->clients)[client->clientsArr_Idx] = NULL;

    mem_free(client);

}

game_t*
new_game(FILE* map_file, const int maxPlayers)
{
    game_t* new_game = mem_malloc_assert(sizeof(game_t), "Error allocating memory in new_game.\n");
    new_game->clients =  mem_malloc_assert((maxPlayers + 1) * sizeof(client_t), "Error allocating memory in new_game.\n");

    // initialize array of client to be all NULL
    for (int i = 0; i < maxPlayers + 1; i++){
        new_game->clients = NULL;
    }

    new_game->goldRemaining = 0;
    new_game->playersJoined = 0;
    new_game->spectatorActive = false;
    new_game->grid = load_grid(map_file, &(new_game->rows), &(new_game->columns));


}

void
end_game()
{
    // free all memory

}

int
update_gold(game_t* game, client_t* player, int x_pos, int y_pos, int goldMaxPiles)
{
    for (int i = 0; i < goldMaxPiles; i++){
        gold_location_t* location = (game->locations)[i];
        if (location->nuggetCount < 0){
            exit(1);
            // error, reached the end of the gold piles without finding pile
        }

        if (location->x == x_pos && location->y == y_pos){
            game->goldRemaining -= location->nuggetCount;
            player->gold -= location->nuggetCount;

            // call grid function to allow for grid to be changed
            

            return location->nuggetCount;
        }

    }

    return -1;
}

void
load_gold(game_t* game, const int goldTotal, const int goldMinPiles, const int goldMaxPiles)
{
    int gold_amt;

    game->locations =  mem_malloc_assert((goldMaxPiles) * sizeof(gold_location_t), "Error allocating memory in load_gold.\n");

    int* nugget_counts = nugget_count_array(goldMinPiles, goldMaxPiles, goldTotal);

    for (int i = 0; i < goldMaxPiles; i++){
        gold_amt = nugget_counts[i]

        if (gold_amt < 0){
            break;
        }

        add_gold_pile(game, gold_amt, i);
    }

    mem_free(nugget_counts);
    
}

void 
add_gold_pile(game_t* game, int gold_amt, int piles)
{
    int gold_x;
    int gold_y;

    gold_location_t* gold_spot = mem_malloc_assert(sizeof(gold_location_t));
    assign_random_spot(game->grid, game->rows; game->columns, '*', &gold_x, &gold_y);
    gold_spot->x = gold_x;
    gold_spot->y = gold_y;
    gold_spot->nuggetCount = gold_amt;

    game->goldRemaining += gold_amt;
    (game->locations)[piles] = gold_spot;

}

int*
nugget_count_array(const int goldMinPiles, const int goldMaxPiles, int goldTotal)
{
    const int lower_bound = 5;
    const int upper_bound = 30;

    while (true){
        int piles = 0;
        int* arr = mem_malloc_assert(goldMaxPiles * sizeof(int));
        int gold_amt;
        int total_gold_added = 0;

        while (piles < goldMaxPiles && total_gold_added < goldTotal){
            gold_amt = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;

            if (gold_amt + total_gold_added > goldTotal || piles = goldMaxPiles - 1){
                gold_amt = goldTotal - total_gold_added;
            }
            
            arr[piles] = gold_amt;
            total_gold_added += gold_amt;

            piles++;
        }

        if(piles >= goldMinPiles){
            if (piles < goldMaxPiles - 1){
                arr[piles + 1] = -1;
            }
            return arr;
        }

        mem_free(arr);
    }

}

