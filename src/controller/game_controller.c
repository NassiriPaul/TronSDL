#include "../../include/controller.h"
#include "../../include/model.h"
#include "../../include/view.h"
#include "../../include/types.h"
#include <stdlib.h> /* Use for randomness */
#include <time.h>

/**
 * @brief suspend for ms milliseconds
 *  for simplicity of use, we convert every time related functions we use in milliseconds
 * @param ms number of milliseconds the program have to stop
 */
static void sleep_ms(int ms)
{
    usleep(ms * 1000); /* Convert milliseconds in microseconds */
}

/**
 * @brief change the direction of the player if possible
 *  a player cannot change for the same direction or the opposite
 * @param grid to get access to the actual position of the player
 * @param direction the direction the player want to have
 */
static void changeDirectionPlayer(Grid* grid, DIRECTIONS direction) {
    if ((direction == UP || direction == DOWN) && grid->player->direction != UP && grid->player->direction != DOWN) grid->player->direction = direction;
    if ((direction == LEFT || direction == RIGHT) && grid->player->direction != LEFT && grid->player->direction != RIGHT) grid->player->direction = direction;
}

/**
 * @brief The strategy to chose the next direction of the bot
 *  
 * @param grid to get access on information in the map
 */
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
            if (!collision[UP] && rand()%20<=18) {grid->bot->direction = originalDirection;return;}
            if (collision[LEFT] && !collision[RIGHT]) {grid->bot->direction = RIGHT;return;}
            if (!collision[LEFT] && collision[RIGHT]) {grid->bot->direction = LEFT;return;}
            if (rand()%2==0) {grid->bot->direction = RIGHT;return;}
            grid->bot->direction = LEFT;
            return;
        case DOWN:
            if (collision[LEFT] && collision[RIGHT]) {grid->bot->direction = originalDirection;return;}
            if (!collision[DOWN] && rand()%20<=18) {grid->bot->direction = originalDirection;return;}
            if (collision[LEFT] && !collision[RIGHT]) {grid->bot->direction = RIGHT;return;}
            if (!collision[LEFT] && collision[RIGHT]) {grid->bot->direction = LEFT;return;}
            if (rand()%2==0)  {grid->bot->direction = RIGHT;return;}
            grid->bot->direction = LEFT;
            return;
        case LEFT:
            if (collision[UP] && collision[DOWN]) {grid->bot->direction = originalDirection;return;}
            if (!collision[LEFT] && rand()%20<=18) {grid->bot->direction = originalDirection;return;}
            if (collision[UP] && !collision[DOWN]) {grid->bot->direction = DOWN;return;}
            if (!collision[UP] && collision[DOWN]) {grid->bot->direction = UP;return;}
            if (rand()%2==0)  {grid->bot->direction = DOWN;return;}
            grid->bot->direction = UP;
            return;
        case RIGHT:
            if (collision[UP] && collision[DOWN]) {grid->bot->direction = originalDirection;return;}
            if (!collision[RIGHT] && rand()%20<=18) {grid->bot->direction = originalDirection;return;}
            if (collision[UP] && !collision[DOWN]) {grid->bot->direction = DOWN;return;}
            if (!collision[UP] && collision[DOWN]) {grid->bot->direction = UP;return;}
            if (rand()%2==0) {grid->bot->direction = DOWN;return;}
            grid->bot->direction = UP;
            return;
    }
}

/**
 * @brief The strategy to chose the next direction of the bot
 *  
 * @param grid to get access on information in the map
 */
static int playGame(int n_lines, int n_columns) {
    int collisionIndicator, isOver, remainingTimePlayer, remainingTimeBot, scorePlayer, scoreBot, turboMovesLeft;
    int input;
    clock_t start, end;
    unsigned long elapsed, remaining;
    Grid* grid;
    int i;
    
    scorePlayer = 0, scoreBot = 0;
    grid = initGrid(n_lines, n_columns);
    viewMenu(n_lines, n_columns);
    /*freeGrid(grid);*/
    while (scorePlayer != 3 && scoreBot != 3) {
        newRun(grid,n_lines, n_columns);
        /*grid = initGrid(n_lines, n_columns);*/
        isOver = 0;
        turboMovesLeft = 0;

        if (grid->player->direction==5){scoreBot =3; continue;}
        viewStart(grid, scorePlayer, scoreBot);
        while(!isOver) {
            
            input = grid->player->direction;
            remainingTimePlayer = 100;
            while (remainingTimePlayer>0){ /* While the player have the time to change direction, we let him change it*/
                remainingTimePlayer = getInput(&input, remainingTimePlayer);
            }
            if (input==6) {scoreBot =3; break;}
            if (input == 5 && turboMovesLeft == 0 && grid->player->turbos>0){
                grid->player->turbos--;
                turboMovesLeft+=8;
                updateViewTurbos(grid->n_lines, grid->player->turbos);
            } else  if (input != 5) changeDirectionPlayer(grid, input);


            for (i = turboMovesLeft>0 ? 2 : 1; i>0;i--) {
                collisionIndicator = moveRider (grid, grid->player, &grid->playerRoute);
                if (collisionIndicator) isOver = 1;
                viewUpdate(grid);
                if (turboMovesLeft>0) turboMovesLeft--;
            }
            
            collisionIndicator = moveRider (grid, grid->bot, &grid->botRoute);
            if (collisionIndicator) isOver = 2;
            changeDirectionBot(grid);
            viewUpdate(grid);

        }
        if (isOver==1) scoreBot++;
        if (isOver==2) scorePlayer++;
        freeRun(grid);
    }
    free(grid);
    return scorePlayer;
}

int startGame(int n_lines, int n_columns) {
    int scorePlayer;
    viewInit();
	
    scorePlayer = playGame(n_lines, n_columns);
    viewCleanup();
    return scorePlayer==3 ? 1 : 0;
}
