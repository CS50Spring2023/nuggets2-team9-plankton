# CS50 Nuggets
## Implementation Spec
### Team 9: Plankton, 23S, 2023

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. As a team of 3, we designed and implemented the server program and used a predesigned client for testing.
Outside of server, our design also includes the `grid` module, which handles the client and global grids, and the `game` module, which handles initializing, updating, and ending the game.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

> Update your plan for distributing the project work among your 3 team members.
> Who writes the client program, the server program, each module?
> Who is responsible for various aspects of testing, for documentation, etc?

Andra:

Jackson:

Sanjana:


## Server (main module)

### Data structures

All data structures are defined within the [Game](##Game) module section.

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

Sends message to client about to quit the game. Called if a client hits `Q` on their keyboard, if a new client tries to join the game when the maximum number of players has been reached, or if a new spectator joins the game.
```c
void send_quitMsg(addr_t clientAddr, int quitCode, bool isSpectator);
```

### Detailed pseudo code

> Pseudocode for the most important functions in Server.

#### `EXAMPLE: sparseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()


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

---


## Grid module

### Data structures

All data structures are defined within the [Game](##Game) module section.

### Definition of function prototypes


### `isVisible` 
Checks the visibility of each point on the grid from a player's location, (pr, pc). For each wall/corner point (wr, wc), the function loops over the grid rows strictly between pr and wr, and over the columns strictly betwen pc and wc. We split this function in cases covering N-S, E-W of the player and all the areas in between. If these points are grid elements (integers) and don't include a boundary (wall or corner) as their value, then the player's grid gets updated to the value of the global grid. If these points are in between grid elements, we check the grid points above/below and to the left/right of the points. If at least one of each doesn't include a boundary, then the player's grid gets updated to the value of the global grid. Otherwise, the point and all points following it (in the direction examined) remain invisible to the player.


### Detailed pseudo code

---

## Game module


### Data structures
```c 

```
```c 

```
```c 

```

### Definition of function prototypes


### Detailed pseudo code


---

## Testing plan

### unit testing

> How will you test each unit (module) before integrating them with a main program (client or server)?

### integration testing

> How will you test the complete main programs: the server, and for teams of 4, the client?

---

## Limitations

> Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.
