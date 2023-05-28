/* unit test for visibiliy
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file.h"
#include "mem.h"
#include "message.h"
#include "log.h"

#include "structs.h"
#include "game.h"
#include "grid.h"


// puts player at spec coordinate, prints the overall grid, then the grid that is visible to them

int main()
{
    // load in sample game
    char* mapFilename = "hole.txt";
    FILE* map_file;
    map_file = fopen(mapFilename, "r");

    game_t* game = new_game(map_file, 26);

    addr_t from = message_noAddr();

    client_t* player = new_player(game, from, "sanjana");

    printf("\n");

    // hardcode "player" at row 2, column 5
    change_spot(game, 2, 5, '@');

    // hardcode "gold" at row 2, column 10
    change_spot(game, 2, 10, '*');

    // hardcode "another player" at row 13, column 14
    change_spot(game, 13, 14, 'B');

    // hardcode "gold" at row 22, column 5-- THIS CAUSES A SEG FAULT?
    // change_spot(game, 22, 5, '*');

    // hardcode "gold" at row 14, column 6
    change_spot(game, 14, 6, '*');

        // hardcode "gold" at row 16, column 9
    change_spot(game, 16, 9, '*');

    grid_toStr(game->grid, NULL, game->rows, game->columns);
    printf(grid_toStr(game->grid, NULL, game->rows, game->columns));

    // // testing get_grid_value: 2,5 is a .
    // char x = get_grid_value(game, 2, 5);
    // printf("%c",x);
    // printf("\n");
    // // 8,8 is a #
    // char y = get_grid_value(game, 7, 15);
    // printf("%c",y);

    printf("\n");
    printf("testing is_visible using different coordinates \n");

    printf("if the player is at row 2, column 5, they should be able to see the gold at row 2 column 10\n");

// game, pc, pr, sc, sr: REMEMBER COLUMN BEFORE ROWS
  bool vis = is_visible(game, 5, 2, 10, 2);
  printf("%d\n", vis);

  printf("if the player is at row 1, column 5, they should not be able to see row 13 column 14 \n");
// // game, pc, pr, sc, sr
  bool vis2 = is_visible(game, 5, 1, 14, 13);
  printf("%d\n", vis2);

  printf("if the player is at row 13, column 14, they should be able to see row 14 column 6 \n");
  // // game, pc, pr, sc, sr
  bool vis3 = is_visible(game, 14, 13, 6, 14);
  printf("%d\n", vis3);

  printf("if the player is at row 13, column 14, they should not be able to see row 16 column 9 \n");
  // // game, pc, pr, sc, sr
  bool vis4 = is_visible(game, 14, 13, 9, 16);
  printf("%d\n", vis4);

  // seg faults
  // get_player_visible(game, player);

  printf("\n\n");

  fclose(map_file);
  grid_toStr(game->grid, NULL, game->rows, game->columns);
  grid_delete(game->grid, game->rows);
  return 0; // exit status
}