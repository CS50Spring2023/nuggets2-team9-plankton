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
    char** grid;
    
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

    game->clients[playersJoined + 1];
    (game->playersJoined)++;
    
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
update_gold()
{

}

void
load_gold(game_t* game, const int goldTotal)
{
    int gold_x;
    int gold_y;

    // go thru and assign gold randomnly somehow
    while (game->goldRemaining < goldTotal){
        assign_random_spot(game->grid, game->rows; game->columns, '*', &gold_x, &gold_y);
    }
    

}

