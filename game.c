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
    client_t** clients;
    int goldRemaining;
    int playersJoined;
    bool spectatorActive;
    int rows;
    int columns;
} game_t;



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
delete_client()
{

}

game_t*
new_game()
{

}

void
end_game()
{

}

void
update_gold()
{

}
