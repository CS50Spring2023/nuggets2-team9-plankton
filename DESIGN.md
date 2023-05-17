# CS50 Nuggets
## Design Spec
### Team 9: Plankton, Spring, 2023

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. Since our team has 3 people, we will not be describing or completing the client proggram
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user: the server receives input through messages from the client.

### Inputs and outputs

> Briefly describe the inputs (map file) and outputs (to terminal).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.
Inputs:

	* map file: an array of strings representing the layout of the grid players can play on;
	the map is converted to a 2D array for manipulation and back to a string for passing messages between client and server

Outputs:

	* TODO


### Functional decomposition into modules

> List and description of the main modules that comprise our server, other than the main module:
`server.c` - main module
`grid.c` - module for handling the global and player grids
`game.c` - module for handling player structures and movement

### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	load grid 
	load gold
	call message_loop(), to await clients
		load a player for each client
		for each message after a player has been initialized
			handle the message by calling handleMove
	call gameOver() to inform all clients the game has ended
	clean up


> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures

> Describe each major data structure in this program: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).

* TODO: Any SERVER level structs???????


## Grid Module

### The Grid Module will be a collection of helper functions handling grid manipulation to be utilized in the server, it will not have a unique main or algorithmic flow.

### Functional decomposition

* load_grid:
	Create an array of strings, each string is one row of the map. Return the grid.
* grid_toStr:
	Create string version of grid map, containing rows*columns characters and new line characters at the end of every line
* assign_random_spot
	Assign players and gold to random locations (row, column) on the grid
* update_player_grid
	Update the player's grid array 
* isVisible:
	For a given player, update player_grid to contain points which are currently visible to them

#### Helper functions
	 
* visCol:
	Check columns between player column and boundary column locations for visibility
* visRow
	Check rows between player row and boundary row locations for visibility
* pcONwc
	Handle case when the player's column is equal to the boundary's column
	
### Pseudo code for each Function

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

#### update_player_grid()


#### isVisible()
	for wall point (wr, wc) in grid array
		for each row exclusively between pr and wr
			compute column coordinates
			if column is an integer (aka a point on grid)
				if grid[row][column] contains a boundary element ("|", "-", "x")
					this and all following row and column points remain inivisible
				else 
					update player_grid[row][column] to value of global grid[row][column]
			else (not a grid point)
				if column element behind or in front is a boundary element
					this and all following row and column points remain invisible
				else
					update player_grid[row][column] to value of global grid[row][column]
		for each column exclusively between pc and wc
			compute row
			repeat row logic above for columns

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.

Both global and player-specific grids are manipulated as arrays of strings. When passing messages between server and clients, grids are converted to strings.


## Game Module

### The Game Module will be a collection of helper functions handling movement and player structure manipulation to be utilized in the server, it will not have a unique main or algorithmic flow.

### Functional decomposition

* handleMovement:
	Handle the movement prompted by a valid key, k. Find the coordinate a player is attempting to move to, and determine if they're allowed to move. If so, call function to move player. If there's gold, handle that accordingly (update player struct and global game status). If there's another player there, handle that accordingly. Then, update the game accordingly.

* updateGame: NOT SURE IF WE WANT TO KEEP THIS HERE!!!
	Has helper functions updateGold, updateVisibility, and updateSeen


### Pseudo code for each Function

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.

* Player Struct points to the following elements:
	* player's ID (a letter between A-Z based on when player joined the game) [char array]
	* player's real name [char array]
	* player's amount of gold [integer value]
	* player's map [array of strings]
	
---
		

