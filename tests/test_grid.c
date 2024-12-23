 #include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/model.h"

int main(void)
{
    Grid *grid;
    int collision;
    int i;
    int sx[7];
    int sy[7];
    Dot *point;

    /* Test: Grid initialization */
    grid = initGrid(10, 10);
    assert(grid != NULL);
    assert(grid->n_lines == 10);
    assert(grid->n_columns == 10);
    assert(grid->player->pos_x == 6);
    assert(grid->player->pos_y == 5);
    assert(grid->player->direction == UP);
    freeGrid(grid);

    /* Test: collision with top wall */
    grid = initGrid(10, 10);
    collision = 0;
    for (i = 0; i < 4; i++) {
        collision = moveRider(grid, grid->player, &grid->playerRoute);
        assert(collision == 0);
        assert(grid->player->direction == UP);
        assert(grid->player->pos_x == 6);
        assert(grid->player->pos_y == 5 - (i + 1));
    }
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: collision with bottom wall */
    grid = initGrid(10, 10);
    collision = 0;
    grid->player->direction = DOWN;
    grid->player->pos_x = 6;
    grid->player->pos_y = 6;
    for (i = 0; i < 3; i++) {
        collision = moveRider(grid, grid->player, &grid->playerRoute);
        assert(collision == 0);
        assert(grid->player->pos_y == 6 + (i + 1));
    }
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: collision with right wall */
    grid = initGrid(10, 10);
    collision = 0;
    grid->player->direction = RIGHT;
    grid->player->pos_x = 6;
    grid->player->pos_y = 6;
    for (i = 0; i < 3; i++) {
        collision = moveRider(grid, grid->player, &grid->playerRoute);
        assert(collision == 0);
        assert(grid->player->pos_x == 6 + (i + 1));
    }
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: collision with left wall */
    grid = initGrid(10, 10);
    collision = 0;
    grid->player->direction = LEFT;
    grid->player->pos_x = 4;
    grid->player->pos_y = 4;
    for (i = 0; i < 3; i++) {
        collision = moveRider(grid, grid->player, &grid->playerRoute);
        assert(collision == 0);
        assert(grid->player->pos_x == 4 - (i + 1));
    }
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: player's route building */
    grid = initGrid(10, 10);
    collision = 0;
    grid->player->pos_x = 6;
    grid->player->pos_y = 6;
    grid->player->direction = UP;
    moveRider(grid, grid->player, &grid->playerRoute);

    grid->player->direction = RIGHT;
    moveRider(grid, grid->player, &grid->playerRoute);
    moveRider(grid, grid->player, &grid->playerRoute);

    grid->player->direction = DOWN;
    moveRider(grid, grid->player, &grid->playerRoute);
    moveRider(grid, grid->player, &grid->playerRoute);

    grid->player->direction = LEFT;
    moveRider(grid, grid->player, &grid->playerRoute);
    moveRider(grid, grid->player, &grid->playerRoute);

    sx[0] = 7;  sy[0] = 7;
    sx[1] = 8;  sy[1] = 7;
    sx[2] = 8;  sy[2] = 6;
    sx[3] = 8;  sy[3] = 5;
    sx[4] = 7;  sy[4] = 5;
    sx[5] = 6;  sy[5] = 5;
    sx[6] = 6;  sy[6] = 6;

    point = grid->playerRoute;
    for (i = 0; i !=7; i++) {
        assert(point != NULL);
        assert(sx[i] == point->pos_x);
        assert(sy[i] == point->pos_y);
        point = point->nextDot;
    }
    freeGrid(grid);

    /* Test: collision with own route */
    grid = initGrid(10, 10);
    grid->player->pos_x = 5;
    grid->player->pos_y = 5;
    grid->player->direction = UP;
    moveRider(grid, grid->player, &grid->playerRoute);

    grid->player->direction = DOWN;
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: collision with bot's route */
    grid = initGrid(10, 10);
    grid->bot->pos_x = 3;
    grid->bot->pos_y = 3;
    grid->bot->direction = RIGHT;
    moveRider(grid, grid->bot, &grid->botRoute);
    moveRider(grid, grid->bot, &grid->botRoute);

    grid->player->pos_x = 4;
    grid->player->pos_y = 4;
    grid->player->direction = UP;
    collision = moveRider(grid, grid->player, &grid->playerRoute);
    assert(collision == 2);
    freeGrid(grid);

    /* Test: collision with the other rider */
    grid = initGrid(10, 10);
    grid->player->pos_x = 6;
    grid->player->pos_y = 5;
    grid->player->direction = LEFT;

    grid->bot->pos_x = 4;
    grid->bot->pos_y = 5;
    grid->bot->direction = RIGHT;

    moveRider(grid, grid->player, &grid->playerRoute);
    collision = moveRider(grid, grid->bot, &grid->botRoute);
    assert(collision == 3);
    freeGrid(grid);

    printf("All tests passed!\n");
    return 0;
}
