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
#include "game.h"
#include "grid.h"
#include <ctype.h>

static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles


int
main(const int argc, char* argv[])
{

    // parse args: first argument should be the pathname for a map file, the second is an optional seed for the random-number generator, which must be a positive int if provided
    
    // make sure there are no more than 2 arguments
    if (argc > 3){
        fprintf(stderr, "Too many arguments were provided. Call using the format ./server map.txt [seed]\n");
	exit(1);
    }
    
    // parse the command line, open the file
    char* mapFilename = argv[1];
    FILE* map_file;
    map_file = fopen(mapFilename, "r");

    // defensive check: file could be opened
    if (map_file == NULL){
        fprintf(stderr, "file could not be opened" );
	exit(1);
    }

    // TODO IF NOT HANDLED ELSEWHERE (SR): If the optional seed is provided, the server shall pass it to srand(seed). 
    // If no seed is provided, the server shall use srand(getpid()) to produce random behavior.
        // int seed = argv[2];


    // create a new game first
    game_t* game = new_game(map_file, MaxPlayers);
    load_gold(game, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles);

    // start up message module
    message_init(stderr);
    message_loop(game, timeout, handleTimeout, NULL, handleMessage);
    message_done();

    // close the file
    fclose(map_file);

    return(0);
}

// lots of error proofing needed
bool
handleMessage(void* arg, const addr_t from, const char* message)
{
    game_t* game = arg;
    char* request = extractRequest(message);

    if (strcmp(request, "PLAY") == 0){
        if (game->playersJoined < MaxPlayers){

            // put this into helper
            char* name = extract_playerName(message); // DO THIS FUNCTION
            client_t* player = new_player(game, from, name);
            mem_free(name);

            char* response = mem_malloc_assert(5, "Error allocating memory in handleMessage.\n");
            sprintf(response, "OK %c", player->id);

            message_send(from, response);
            mem_free(response); // is this losing the string for whoever receives message?????

            // send new client messages: grid, gold, display
            inform_newClient(client, game);

        }
        else {
            send_quitMsg(from, 2, false);
        }
    }
    else if (strcmp(request, "SPECTATE") == 0){
        client_t* spectator = new_spectator(game, from);

        // send new client messages: grid, gold, display
        inform_newClient(spectator, game);

    }
    else if (strcmp(request, "KEY") == 0){
        client_t* player = find_client(from, game);
        handle_movement(player, request[4], game);
        
    }

    mem_free(request);

    return false;




// the server immediately sends a GRID, GOLD and DISPLAY message to all new clients

}


void
inform_newClient(client_t* client, game_t* game)
{
    // send grid message
    char* gridMsg = mem_malloc_assert(13, "Error allocating memory in inform_newClient.\n"); // allows for max 3 digit row, columns numbers
    sprintf(gridMsg, "GRID %d %d", game->rows, game->columns);
    message_send(client->clientAddr, gridMsg);
    mem_free(gridMsg);

    // send gold message
    send_goldMsg(game, client, 0);

    // send display message
    send_displayMsg(game, client);

}

void
send_goldMsg(game_t* game, client_t* client, int goldPickedUp)
{
    // send gold message
    char* goldMsg = mem_malloc_assert(17, "Error allocating memory in inform_newClient.\n"); // allows for max 3 digit gold count numbers
    sprintf(gridMsg, "GOLD %d %d %d", goldPickedUp, client->gold, game->goldRemaining); //initial gold message always 0 just picked up
    message_send(client->clientAddr, goldMsg);
    mem_free(goldMsg);

}

void
send_displayMsg(game_t* game, client_t* client){
    int msgSize;
    char* map;
    char* display;

    if (client->isSpectator){
        map = grid_toStr(game->grid, NULL, game->rows, game->columns);
    }
    else{ // assmumes that grid is already up to date
        map = grid_toStr(game->grid, client->grid, game->rows, game->columns);
    }

    msgSize = 10 + strlen(map);
    display = mem_malloc_assert(msgSize, "Error allocating memory in sendDisplayMsg.\n");
    sprintf(display, "DISPLAY\n%s", map);
    mem_free(map);

    message_send(client->clientAddr, display);
    mem_free(display);

}

char*
extract_playerName()
{
    
}

char* 
extractRequest(const char* input) 
{
    if (input == NULL || input[0] == '\0') {
        // Handle empty input string or null pointer
        return NULL;
    }

    int length = strlen(input);
    int wordEndIndex = -1;

    // Find the end index of the first word
    for (int i = 0; i < length; i++) {
        if (input[i] == ' ') {
            wordEndIndex = i;
            break;
        }
    }

    if (wordEndIndex == -1) {
        // If no space is found, the entire input is considered as the first word
        wordEndIndex = length;
    }

    // Allocate memory for the new string
    char* firstWord = mem_malloc_assert((wordEndIndex + 1) * sizeof(char), "Error allocating memory in extractFirstWord.\n");


    // Copy the first word into the new string
    strncpy(firstWord, input, wordEndIndex);
    firstWord[wordEndIndex] = '\0';  // Null-terminate the string

    return firstWord;
}



void
handle_movement(client_t* player, char key, game_t* game)
{
    if(key == 'q'){
        send_quitMsg(player, 1, player->isSpectator);
        
        if (!player->isSpectator){
            // reset spot
            if (player->onTunnel){
                change_spot(game, player->x, player->y, '#');
            }
            else{
                change_spot(game, player->x, player->y, '.');
            }
        }

        delete_client(player, game);
    }

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

    char grid_val = get_grid_value(game, newPos_x, newPos_y);

    if (grid_val == '+' || grid_val == '-' || grid_val == '|' || grid_val == ' '){
        return;
    }
    else if (grid_val == '.' || grid_val == '#'){
        // change the spot the player came from back
        update_previous_spot(player, game, grid_val);
        
        // change the global grid on the spot they are now on to be their letter
        change_spot(game, newPos_x, newPos_y, player->id);

        // update the player's position in the player struct
        update_position(player, newPos_x, newPos_y);
        
    }
    else if (isalpha(grid_val)){
        // find the player there using a game function
        player_t* other_player = find_player(grid_val, game);

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
        int nuggetsFound = update_gold(game, player, newPos_x, newPos_y, goldMaxPiles);
        
        // update the client that just picked up gold
        send_goldMsg(game, player, nuggetsFound);

        // update the other clients about the gold counts
        for (int i = 0; i < game->playersJoined + 1; i++){
            if (game->clients[i] != NULL && ((game->clients)[i])->id != player->id){
                send_goldMsg(game, (game->clients)[i], 0);
            }
        }

        // update the grids
                
        // change the spot the player came from back
        update_previous_spot(player, game, grid_val);
        
        // change the global grid on the spot they are now on to be their letter
        change_spot(game, newPos_x, newPos_y, player->id);

        // update the player's position in the player struct
        update_position(player, newPos_x, newPos_y);

    }
   
}

static void
update_previous_spot(player_t* player, game_t* game, char grid_val)
{
    //change the global grid on the spot they came from back to what it was
    if (player->onTunnel){
        change_spot(game, player->x, player->y, '#');
    }
    else{
        change_spot(game, player->x, player->y, '.');
    }
    player->onTunnel = (grid_val == '#');
}


void
send_quitMsg(addr_t clientAddr, int quitCode, bool isSpectator)
{
    char* quitMsg = mem_malloc_assert(55, "Error allocating memory in send_quitMsg.\n"); 
    char* quitReason = mem_malloc_assert(45, "Error allocating memory in send_quitMsg.\n");

    if (isSpectator){
        if (quitCode == 0){
            strcpy(quitReason, "You have been replaced by a new spectator.");
        }
        else{
            strcpy(quitReason, "Thanks for watching!");
        }
    }
    else {
        if (quitCode == 1){
            strcpy(quitReason, "Thanks for playing!");
        }
        if (quitCode == 2){
            strcpy(quitReason, "Game is full: no more players can join.");
        }
        if (quitCode == 3){
            strcpy(quitReason, "Sorry - you must provide player's name.");
        }

    }

    sprintf(quitMsg, "QUIT %s", quitReason);
    message_send(clientAddr, quitMsg);
    mem_free(quitMsg);

}
