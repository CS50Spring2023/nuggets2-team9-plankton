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

### Functional decomposition into modules

> List and briefly describe any modules that comprise your server, other than the main module.

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

---

## Grid Module

### The Grid Module will be a collection of helper functions to be utilized in the server, it will not have a unique main or algorithmic flow.

### Functional decomposition

* loadVisible:
	For a given player, load what points are currently visible to them
* loadGold:
	During the initialization of the game, randomly load the gold into valid coordinates (accessible to players)
* addPlayer:
	When a player enters the game, randomly add them into a valid spot (accessible to players, doesn't currently contain gold)
* handleMessage:
	Handle the message received from the client accordingly. If it is "Q", quit the game. If it is any other key, k, find the coordinate a player is attempting to move to, and determine if they're allowed to move. If so, call function to move player. If there's gold, handle that accordingly (update player struct and global game status). If there's another player there, handle that accordingly. Then, update the game accordingly.
* updateGame:
	Has helper functions updateGold, updateVisibility, and updateSeen
* isVisibile:
	Checks the visibility of each point on the grid from a player's location, (pr, pc). For each wall/corner point (wr, wc), the function loops over the grid rows strictly between pr and wr, and over the columns strictly betwen pc and wc. We split this function in cases covering N-S, E-W of the player and all the areas in between. If these points are grid elements (integers) and don't include a boundary (wall or corner) as their value, then the player's grid gets updated to the value of the global grid. If these points are in between grid elements, we check the grid points above/below and to the left/right of the points. If at least one of each doesn't include a boundary, then the player's grid gets updated to the value of the global grid. Otherwise, the point and all points following it (in the direction examined) remain invisible to the player. 
	
### Pseudo code for each Function

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

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

#### handle movement
	Check whats is in the spot we want to move to
	If boundary
      Do nothing
	If gold
	  increment player’s purse gold count
      change grid representation at location to “.”
      decrement count of unclaimed gold
      Call update gold to send message from serve
    If player
   	  Update global grid to reflect swap
	  Update grids of the players changed

    Call update grid, which updates all player grids to reflect new global grid

			

### Major data structures

**** add is spectator boolean
* Player Struct that contains the following
	* What coordinates the player has "seen" [represented as an array of tuples]
	* What coordinates the player currently can see [represented as an array of tuples]
	* How much gold the player has [represented as an int]

* Game Struct that contains the follow
	* array of player structs
	* the total amount of gold remaining

Change this to just be an array not a struct:

* Grid Struct that contains the following
	* NR x NC array that contains numeric values for each coordinate, denoting what is located there
	* -1 = this gridspot can never be accessed by the player (ex: empty space between rooms)
	* . = this gridspot is empty, it can be accessed by the player, but contains nothing
	* \* = this gridspot contains gold
	* x = this gridspot is a corner
	* p = this gridspot contains a player
	* | = this gridspot contains a vertical wall, it cannot be accessed
	* \- = this gridspot contains a horizontal wall, it cannot be accessed

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
