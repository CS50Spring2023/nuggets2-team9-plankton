# CS50 Nuggets
## Implementation Spec
### Team 9: Plankton, 23S, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. As a team of 3, we designed and implemented the server program and used a predesigned client for testing.
Outside of server, our design also includes the `grid` module, which handles the client and global grids, and the `game` module, which handles initializing, updating, and ending the game.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

Andra: design doc, implementation doc, visibility

Jackson: design doc, server, game module, integration & testing

Sanjana: grid module, visibility, integration & testing

## Data structures

We defined three custom data structures within `structs.h`, which are utilized by the main module, `server`, as well as by the helper modules `grid` and `game`. Since our grid is represented as an array of strings (or a string), we decided a against creating a grid structure to avoid unnecessary complexity.

* `gold_location_t` holds coordiantes and gold count for bundles of gold. The `game` module uses it to handle loading gold piles at the beginning of the game and updating them throughout the game.

* `client_t` holds all necessary information about each client who joins the game, including their address, position, personalized grid, and amount of gold, if they are a player. This structure is heavily utilized by all three modules.

* `game_t` holds all high level information about the game, including the global grid, a list of all the clients, and the amount of gold remaining in the grid. This structure is heavily utilized by all three modules.


## Server (main module)

### Definition of function prototypes

> For each function in server, here is a brief description and its function prototype.

Handles messages to `PLAY`, `SPECTATE`, or a specific `KEY` sent by a client. The function calls `inform_newClient` to allow new players to join the game if the maximum amount of player has not been reached, to ensure the most recent spectator is observing the game, and calls `handle_movement` for any key press.
```c
bool handleMessage(void* arg, const addr_t from, const char* message);
```

Sends a message to all players and the spectator to update their local displays by calling `send_displayMsg`.
```c
void update_displays(game_t* game);
```

Called by handleMessage when a client sends a `PLAY` or `SPECTATE` request. Sends client a gold message by calling `send_goldMsg` and a display message by calling `send_displayMsg`.
```c
void inform_newClient(client_t* client, game_t* game);
```

Called in `inform_newClient` and `handle_message`. Informs players of the amount of gold they pick up, the amount in their purses, and both players and spectators of the amount left in the game.
```c
void send_goldMsg(game_t* game, client_t* client, int goldPickedUp);
```

Called by `update_displays` to update an individual client's display.
```c
void send_displayMsg(game_t* game, client_t* client);
```

Extracts each player's name when they first join the game, storing the string up to a maximum of 50 characters.
```c
char* extract_playerName(const char* message, addr_t clientAddr);
```

Called by `handleMessage` to handle each request, or message, of a client playing the game.
```c
char* extractRequest(const char* input);
```

Called by `handleMessage` to handle the movement of a player whenever they press `Q` for quitting or a key for moving within the grid. When moving, the function updates a player's position, visibility, and handles the cases when a player steps on gold or another player.
```c
void handle_movement(client_t* player, char key, game_t* game);
```

Updates the previous spot occupies by a player when they move to the next spot.
```c
static void update_previous_spot(client_t* player, game_t* game, char grid_val);
```

Sends message to client about to quit the game. Called if a client hits `Q` on their keyboard, if the game ends by virtue of all gold being collected, if a new client tries to join the game when the maximum number of players has been reached, or if a new spectator joins the game.
```c
void send_quitMsg(addr_t clientAddr, int quitCode, bool isSpectator);
```

### Detailed pseudo code

#### `handleMessage`:

    extract client request
    if request is "PLAY"
        if still room for players
            create a new player 
            send messages (grid, gold, display)
        else
            send quit message
    else if request is "SPECTATE"
        if there is already another spectator
            send them a quit message
        create a new spectator
        send messages (grid, gold, display)
    else if request is "KEY"
        handle the player's move


#### `update_displays`:
    
    if there is a spectator
        send message to update their display to show the global grid
    loop through players
        send message to update their local grids


#### `inform_newClient`:

    send grid (location) message
    call function to send gold message
    call function to send display message


#### `send_goldMsg`:

    send gold message: how much was picked up, how much player owns, how much is left


#### `send_displayMsg`:

    if client is a spectator
        convert global map array of strings to array
    else if client is a player
        convert local map array of strings to array
    send display message to client


#### `extract_playerName`:

    initialize an array for the client's name
    for each character in a client's message
        parse message until start of player name
            if name length less than maximum length
                if the character is printable and not blank
                    add it to the clinet's name array
                if it's blank
                    add "_" to the client's name array
    

#### `extractRequest`:

    if input is empty
        return null
    declare and allocate memory for request string
    otherwise loop through each character until the first space
        add it to request string
    return request string


#### `handle_movement`:

    if key is 'q'
        send message to quit
        update grid to delete client 
    if key is one of the movement keys (h, l, j, k, y, u, b, n)
        if trying to move on wall spot
            do nothing
        else if moving to empty grid spot (".") or in a tunnel ("#")
            update previous spot player was on to reflect the change
            update global grid to reflect player at new location
            update the player's record of their position
        else if moving to other player's location
            switch the positions of the two players
            update each player's records on the spot they stand on 
            update the global grid to reflect change
        else if moving to a spot with gold
            update client who picked up gold
            update other clients about gold count
            update previous spot player was on to reflect the change
            update global grid to reflect player at new location
            update the player's record of their position
    update displays of affected players


#### `update_previous_spot`:

    if previous spot was a tunnel
        update it to "#"
    if previous spot was a grid spot
        update it to "."


#### `send_quitMsg`:

    if client is a spectator
        record the quit reason: the game endeded (the client pressed "q" or the gold was collected) or another spectator joined
    if client is a player
        record the quit reason: the game endeded (the client pressed "q" or the gold was collected) or the player maximum number has been reached
    send quit message with the quit reason

---


## Grid module

The functions in this helper module are utilized by the server.

### Data structures

All data structures are defined within the [Game](#game-module) module section.

### Definition of function prototypes

Reads a file into an array of strings, with each string in the array representing a row of the game map.
```c
char** load_grid(FILE* fp, int* rows, int* columns)
```

Loads an empty player grid of equal rows and columns as the global grid.
```c
char** load_player_grid(game_t* game)
```

Converts an array of strings to a string.
```c
char* grid_toStr(char** global_grid, char** player_grid, int rows, int columns)
```

Assigns a random spot for an object (used to randomly place players and gold).
```c
void assign_random_spot(char** grid, int rows, int columns, char thing, int* spot_r, int* spot_c)
```

Returns the symbol at given coordinates of a grid.
```c
char get_grid_value(game_t* game, int r, int c)
```

Assigns a new symbol at given coordinates of a grid.
```c
void change_spot(game_t* game, int r, int c, char symbol)
```

Checks whether a location is open for a player to move in.
```c
static bool isOpen(game_t* game, const int c, const int r)
```

Determines if a value is an integer.
```c
bool static is_integer(float num);
```

Computes whether a point on the grid is visible from a player's current location.
```c
bool is_visible(game_t* game, const int playerColumn, const int playerRow, const int column, const int row) 
```

Loops over each point in the grid and calls `is_visible`, updating the player's grid accordingly. Returns true if the player's grid was modified.
```c
bool get_player_visible(game_t* game, client_t* player)
```

Frees memory allocated to a grid.
```c
void grid_delete(char** grid, int rows)
```

### Detailed pseudo code

#### `load_grid`:

    check if file is valid
    while there are rows in the map file
        add row containing a string of the file to grid 2d character array


#### `load_player_grid`:

    initialize an empty row as a string containing "\0" at an index equal to the last global grid cloumn + 1
    for each row in the global grid
        add an empty row into a player's local grid


#### `grid_toStr`:

    initialize a string for the converted map
    for each row the grid
        for each column in the grid
            the map string at index (current_row * (columns + 1) + current_column) is assigned the value of grid[current_row][current_column]
            if the player's grid is not null
                the map string gets assigned the value of player's grid[current_row][current_column] (at the same index as above)
        add "\0"
    add "\0"
    return the string


#### `assign_random_spot`:

    while not placed
        pick random row, column values 
        if location is a grid point "."
            place object (player or gold) at that location grid 


#### `get_grid_value`:

    return grid symbol at given row and column


#### `change_spot`:

    assign symbol to grid location at given row and column


#### `isOpen`:

    if symbol at given grid location is viable for player movement (".", "#", "*", or a letter)
        return true
    return false


#### `is_integer`:

    return true if a floating point variable is equal to iself cast as an integer


#### `is_visible`:

    calculate difference in x and y between the player and the spot
    if no change in x (player on same column as spot)  
        move incrementally towards the spot (either up or down, depending on player location)
    if no change in y (player on same row as spot)
        move incrementally towards the spot (either left or right, depending on player location)
    if change in both x and y
        calculate the slope
        calculate movement bounds
        for each column between player and spot
            if the point is on the grid (and integer)
                if it is not a valid grid point (".", "#", "*", or a letter)
                    return false
            else
                if the grid points above and below the point are not valid (".", "#", "*", or a letter)
                    return false 
        for each row between player and spot
            if the point is on the grid (and integer)
                if it is not a valid grid point (".", "#", "*", or a letter)
                    return false
            else
                if the grid points above and below the point are not valid (".", "#", "*", or a letter)
                    return false 
    return true


#### `get_player_visible`:

    initialize a variable, modified, as false to check if player's visibility changed
    for each row in the player's grid
        for each column in the player's grid
            if player at point location
                update player grid
                update modified
            if point is visible
                update player grid
                update modified
            if ...
    return modified


#### `grid_delete`:

    for each row in a grid
        free its memory
    free grid memory


---

## Game module

The functions in this helper module are utilized by the server.

### Definition of function prototypes

Initializes a new player whenever a player client successfully joins the game.
```c
client_t* new_player(game_t* game, addr_t client, char* name)
```

Updates a player's position by passing new row and column coordinates into the player structure.
```c
void update_position(client_t* player, int r, int c)
```

Returns the client with a given address.
```c
client_t* find_client(addr_t clientAddr, game_t* game)
```

Returns the player with a given address.
```c
client_t* find_player(char id, game_t* game)
```

Initializes a new spectator whenever a spectating client successfully joins the game.
```c
client_t* new_spectator(game_t* game, const addr_t client)
```

Deletes a client by freeing all memory allocated to them.
```c
void delete_client(client_t* client, game_t* game)
```

Creates a new game, loading the map file, initializing game statistics to 0, and initializing a NULL list of clients.
```c
game_t* new_game(FILE* map_file, const int maxPlayers)
```

Ends the game, freeing all memory allocated to the global grid and to each client.
```c
void end_game(game_t* game, int maxGoldPiles)
```

Updates amount of gold in a player's purse, the overall amount of gold remaining in the game, and grid representations.
```c
int update_gold(game_t* game, client_t* player, int r, int c, int goldMaxPiles)
```

Places gold into random bundles at random locations at the beginning of the game, by calling `add_gold_pile`.
```c
void load_gold(game_t* game, const int goldTotal, const int goldMinPiles, const int goldMaxPiles)
```

Places a gold bundle at o a random location.
```c
void add_gold_pile(game_t* game, int gold_amt, int piles)
```

Creates an array of gold bundles with randomly generated nugget counts.
```c
int* nugget_count_array(const int goldMinPiles, const int goldMaxPiles, int goldTotal)
```

### Detailed pseudo code

#### `new_player`:

    create a new client_t player, utilizing the client_t struct with player features
    assign them to a random spot on the grid
    return player


#### `update_position`:

    player's row becomes given row
    player's column becomes given column


#### `find_client`:

    for each client who joined
        if their address matches a the address passed as a parameter
            return that client
    return NULL


#### `find_player`:

    for each client who joined
        if their player id matches a the player id passed as a parameter
            return that player
    return NULL


#### `new_spectator`:

    create a new client_t spectator, utilizing the client_t struct with specator features
    mark that the game has one spectator in the client_struct
    return spectator

#### `delete_client`:

    if the client has a name
        free allocated memory
    if the client has a grid
        free allocated memory
    free memory allocated to client in client array
    free client memory

#### `new_game`:

    create a new game_t game
    for each of the possible clients
        initialize their value in the clients array to NULL
    initialize remaining gold to 0
    initialize number of players who joined to 0
    initialize spectator active switch to false
    load global grid
    return game


#### `end_game`:

    for each client in the game
        free allocated memory
    if there is a global grid
        free allocated memory
    for each gold pile location
        free allocated memory
    free game memory


#### `update_gold`:

    for each pile of gold
        if end of gold piles
            exit 1
        if gold pile at location passed into function
            decrement gold remaining
            increment gold in player's purse
            return the location of the gold pile
    return -1


#### `load_gold`:

    initialize gold amount
    for each gold pile (out of max piles)
        nugget count array value to gold amount
        add gold pile to random spot on grid


#### `add_gold_pile`:

    initialize a gold_location_t gold spot
    assign it to a random location on the grid
    assign it a gold amount (passed into function)
    add gold amount to the overall remaining amount of gold
    add the gold spot to the game's locations


#### `nugget_count_array`:

    while true
        while piles less than max piles and total gold addes less than total gold possible
            assign a random amount to gold amount
            if more gold assigned than allowable
                gold amount becomes total gold minus the total gold added
            assign gold amount to array of piles
            add gold amount to total gold
            increment piles
        if more piles or equal than min piles
            if less piles than max piles minus one
                add -1 to piles array
            return array


---

## Testing plan

### unit testing

We will first ensure that all modules compile separately. We will create a unit test for the grid module, testing loading in a map file and the functions in the grid module. We will also create a unit test for the server to ensure that it can send and receive messages to and from clients. Since the game module's function is to initialize, update, and end the game, we will test it in integration with the server.

### integration testing

As a team of three, we will test the integration of the server with our custom helper modules. We will test that game properly initializes, updates, and ends the game when the respective messages are received by the server. We will also test grid integration by observing whether a client's and spectator's (global) grids update as expected for specific player moves, in terms of visibility, symbols, and game statistics present on the grid displays. We will also test the functioning of the game on a custom grid we created, `plankton.txt`.

---

## Limitations

> Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.

*

