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

    grid_toStr(game->grid, NULL, game->rows, game->columns);
    // printf(grid_toStr(game->grid, NULL, game->rows, game->columns));

    addr_t from = message_noAddr();

    client_t* player = new_player(game, from, "sanjana");

     printf("\n");

    // testing get_grid_value: 2,5 is a .
    char x = get_grid_value(game, 2, 5);
    printf("%c",x);
    printf("\n");
    // 8,8 is a #
    char y = get_grid_value(game, 7, 15);
    printf("%c",y);

    // get_player_visible(game, player);

  printf("\n\n");

  fclose(map_file);
  grid_toStr(game->grid, NULL, game->rows, game->columns);
  grid_delete(game->grid, game->rows);
  return 0; // exit status
}