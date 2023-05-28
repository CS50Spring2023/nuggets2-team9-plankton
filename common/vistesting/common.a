/* unit test for visibiliy
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../libs/file.h"
#include "../libs/mem.h"

#include "../support/log.h"
#include "../support/message.h"

#include "structs.h"
#include "game.h"
#include "grid.h"

// puts player at spec coordinate, prints the overall grid, then the grid that is visible to them

int main()
{
    // load in sample game
    char* mapFilename = "../maps/hole.txt";
    FILE* map_file;
    map_file = fopen(mapFilename, "r");

    game_t* game = new_game(map_file, 26);

    grid_toStr(game->grid, NULL, game->rows, game->columns);

    addr_t from = message_noAddr();

    client_t* player = new_player(game, from, "sanjana");

    get_player_visible(game, player);

  printf("\n\n");

  for (int i = 0; i < 100; i++) {
    printf("@");
  }

  printf("\n\n\n");

  fclose(map_file);
  grid_toStr(game->grid, NULL, game->rows, game->columns);
  grid_delete(game->grid, game->rows);
  return 0; // exit status
}