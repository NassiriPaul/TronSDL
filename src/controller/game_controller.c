#include "../../include/controller.h"
#include "../../include/model.h"
#include "../../include/view.h"
#include "../../include/types.h"
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <sys/select.h> /* select()       */


void sleep_ms(int milliseconds)
{
    usleep(milliseconds * 1000);
}

static void changeDirectionPlayer(Grid* grid, DIRECTIONS direction) {
    if ((direction == UP || direction == DOWN) && grid->player->direction != UP && grid->player->direction != DOWN) grid->player->direction = direction;
    if ((direction == LEFT || direction == RIGHT) && grid->player->direction != LEFT && grid->player->direction != RIGHT) grid->player->direction = direction;
}

static void changeDirectionBot(Grid* grid) {
    DIRECTIONS originalDirection;
	int presumed_next_pos_x,  presumed_next_pos_y, collisionIndicator;
    int collision[4];

    originalDirection = grid->bot->direction;

    grid->bot->direction = UP;
    setPresumedNextDirection(grid->bot, &presumed_next_pos_x, &presumed_next_pos_y);
    collision[UP] = checkCollision (grid, presumed_next_pos_x, presumed_next_pos_y);

    grid->bot->direction = RIGHT;
    setPresumedNextDirection(grid->bot, &presumed_next_pos_x, &presumed_next_pos_y);
    collision[RIGHT] = checkCollision (grid, presumed_next_pos_x, presumed_next_pos_y);

    grid->bot->direction = DOWN;
    setPresumedNextDirection(grid->bot, &presumed_next_pos_x, &presumed_next_pos_y);
    collision[DOWN] = checkCollision (grid, presumed_next_pos_x, presumed_next_pos_y);

    grid->bot->direction = LEFT;
    setPresumedNextDirection(grid->bot, &presumed_next_pos_x, &presumed_next_pos_y);
    collision[LEFT] = checkCollision (grid, presumed_next_pos_x, presumed_next_pos_y);

    switch (originalDirection) {
        case UP:
            if (collision[LEFT] && collision[RIGHT]) {grid->bot->direction = originalDirection;return;}
            if (!collision[UP] && rand()%10<=8) {grid->bot->direction = originalDirection;return;}
            if (collision[LEFT] && !collision[RIGHT]) {grid->bot->direction = RIGHT;return;}
            if (!collision[LEFT] && collision[RIGHT]) {grid->bot->direction = LEFT;return;}
            if (grid->n_columns-grid->bot->pos_x>grid->n_columns/2) {grid->bot->direction = RIGHT;return;}
            grid->bot->direction = LEFT;
            return;
        case DOWN:
            if (collision[LEFT] && collision[RIGHT]) {grid->bot->direction = originalDirection;return;}
            if (!collision[DOWN] && rand()%10<=8) {grid->bot->direction = originalDirection;return;}
            if (collision[LEFT] && !collision[RIGHT]) {grid->bot->direction = RIGHT;return;}
            if (!collision[LEFT] && collision[RIGHT]) {grid->bot->direction = LEFT;return;}
            if (grid->n_columns-grid->bot->pos_x>grid->n_columns/2) {grid->bot->direction = RIGHT;return;}
            grid->bot->direction = LEFT;
            return;
        case LEFT:
            if (collision[UP] && collision[DOWN]) {grid->bot->direction = originalDirection;return;}
            if (!collision[LEFT] && rand()%10<=8) {grid->bot->direction = originalDirection;return;}
            if (collision[UP] && !collision[DOWN]) {grid->bot->direction = DOWN;return;}
            if (!collision[UP] && collision[DOWN]) {grid->bot->direction = UP;return;}
            if (grid->n_lines-grid->bot->pos_y>grid->n_lines/2) {grid->bot->direction = DOWN;return;}
            grid->bot->direction = UP;
            return;
        case RIGHT:
            if (collision[UP] && collision[DOWN]) {grid->bot->direction = originalDirection;return;}
            if (!collision[RIGHT] && rand()%10<=8) {grid->bot->direction = originalDirection;return;}
            if (collision[UP] && !collision[DOWN]) {grid->bot->direction = DOWN;return;}
            if (!collision[UP] && collision[DOWN]) {grid->bot->direction = UP;return;}
            if (grid->n_lines-grid->bot->pos_y>grid->n_lines/2) {grid->bot->direction = DOWN;return;}
            grid->bot->direction = UP;
            return;
    }
}
static int playGame(int n_lines, int n_columns) {
    int collisionIndicator, isOver, remainingTimePlayer, remainingTimeBot, scorePlayer, scoreBot;
    DIRECTIONS input;
    clock_t start, end;
    unsigned long elapsed, remaining;
    Grid* grid;
    scorePlayer = 0, scoreBot = 0;
    while (scorePlayer != 3 && scoreBot != 3) {
        grid = initGrid(n_lines, n_columns);
        isOver = 0;
        viewStart(grid);
        updateViewScore(n_lines, scorePlayer, scoreBot);
        while(!isOver) {

            input = grid->player->direction;
            remainingTimePlayer = 100;
            while (remainingTimePlayer>0){ /* While the player have the time to change direction, we let him change it*/
                remainingTimePlayer = getDirection(&input, remainingTimePlayer);
            }
            changeDirectionPlayer(grid, input);
            collisionIndicator = moveRider (grid, grid->player, &grid->playerRoute);
            if (collisionIndicator) isOver = 1;
            viewUpdate(grid);
            
            

            collisionIndicator = moveRider (grid, grid->bot, &grid->botRoute);
            if (collisionIndicator) isOver = 2;
            changeDirectionBot(grid);
            viewUpdate(grid);
        }
        freeGrid(grid);
        if (isOver==1) scoreBot++;
        if (isOver==2) scorePlayer++;

    }
    return scorePlayer;
}

int startGame(int n_lines, int n_columns) {
    int scorePlayer;
    viewInit();
    scorePlayer = playGame(n_lines, n_columns);
    viewCleanup();
    return scorePlayer==3 ? 1 : 0;
}
