//
//
//
//


#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdlib.h>
#include <stdbool.h>
#include "../support/message.h"



// typedef struct gold_location gold_location_t;
typedef struct gold_location {
    int r;
    int c;
    int nuggetCount;
} gold_location_t;

// typedef struct client client_t;
typedef struct client {
     addr_t clientAddr;
    bool isSpectator;
    char id;
    char* real_name;
    int gold;
    int r;
    int c;
    char** grid;
    bool onTunnel;
    int clientsArr_Idx;
    
} client_t;

// typedef struct game game_t;
typedef struct game {
    char** grid;
    client_t** clients;
    int goldRemaining;
    int playersJoined;
    bool spectatorActive;
    int rows;
    int columns;
    gold_location_t** locations;

} game_t;

#endif