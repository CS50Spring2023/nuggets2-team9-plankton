/*
game.c 
TODO: add details here

Team 9: Plankton, April 2023
*/



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

    game->clients[playersJoined + 1];
    (game->playersJoined)++;
    
    // assign player to a random spot
    assign_random_spot(game->grid, game->rows, game->columns, player);
    
}

// add update position function
void
update_position(player_t* player, int x, int y)
{
    player->x = x;
    player->y = y;
}

void
new_spectator(game_t* game, const addr_t client)
{
    if (game->spectatorActive){
        spectator_quit((game->clients)[0]);
        client_delete((game->clients)[0]);
        game->spectatorActive = false;
    }

    client_t* spectator = mem_malloc_assert(sizeof(client_t), "Error allocating memory in new_spectator.\n");
    spectator->isSpectator = true;
    spectator->clientAddr = client;
    spectator->id = '\0';
    spectator->grid = NULL;
    spectator->gold = 0;
    spectator->real_name = NULL;

    (game->clients)[0] = spectator;
    game->spectatorActive = true;

}

void
delete_client(client_t* client)
{
    if (player->real_name != NULL){
        mem_free(player->real_name);
    }
    if (player->grid != NULL){
        // loop thru and free strings, don't know how grid will be represented yet
    }

}

game_t*
new_game(FILE* map_file, const int maxPlayers)
{
    game_t* new_game = mem_malloc_assert(sizeof(game_t), "Error allocating memory in new_game.\n");
    new_game->clients =  mem_malloc_assert((maxPlayers + 1) * sizeof(client_t), "Error allocating memory in new_game.\n");
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

void
update_gold(game_t* game, client_t* player, int x_pos, int y_pos, int goldMaxPiles)
{
    for (int i = 0; i < goldMaxPiles; i++){
        gold_location_t* location = (game->locations)[i];
        if (location->nuggetCount < 0){
            exit(1);
            // error
        }

        if (location->x == x_pos && location->y == y_pos){
            game->goldRemaining -= location->nuggetCount;
            player->gold -= location->nuggetCount;
            // call grid function to allow for grid to be changed

            return;
        }

    }
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

