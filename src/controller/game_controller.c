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
 * @brief the main loop of the game himself
 *  
 * @param n_lines how many rows the map is made of
 * @param n_lines how many columns the map is made of
 *
 *@ return the score of the player
 */
static int playGame(int n_lines, int n_columns) {
    int collisionIndicator; /* collisionIndicator show what type of collision happen, no collision head to route or headbutt */
    int isOver; /* isOver indicates if the game is over, if 0 not over, if 1 bot won, if 2 player won */
    int remainingTimePlayer; /* How many ms left before the player will move by himself */
    int scorePlayer, scoreBot;
    int turboMovesLeft; /* In turbo mode, he can move 2 times in a row then the bot move. He can do it turboMovesLeft times.*/
    int input; /* Will receive the input from the controller to what the player wanna do */
    unsigned long remaining; /* How many ms he have left before the player move in the direction he have */
    Grid* grid;
    int i; /* How many time he will move before the bot will move */
    
    scorePlayer = 0, scoreBot = 0;
    grid = initGrid(n_lines, n_columns);
    if (viewMenu(n_lines, n_columns)) scoreBot=3;
    while (scorePlayer != 3 && scoreBot != 3) {
        newRun(grid,n_lines, n_columns); /* Init a new map for the round */
        isOver = 0;
        turboMovesLeft = 0; /* do not start in turbo mode*/
        viewStart(grid, scorePlayer, scoreBot);
	if (grid->player->direction==5){scoreBot =3; continue;}
        while(!isOver) {
            
            input = grid->player->direction;
            remainingTimePlayer = 100;
            while (remainingTimePlayer>0){ /* While the player have the time to change direction, we let him change it*/
                remainingTimePlayer = getInput(&input, remainingTimePlayer);
            }
            if (input==6) {scoreBot =3; break;} /* the player try to live so we act like he forfeit and end the game */
            if (input == 5 && turboMovesLeft == 0 && grid->player->turbos>0){ /* turn on the turbo mode if he isn't on and he have turbo left*/
                grid->player->turbos--;
                turboMovesLeft+=6;
                updateViewTurbos(grid->n_lines, grid->player->turbos);
            } else  if (input != 5) changeDirectionPlayer(grid, input);


            for (i = turboMovesLeft>0 ? 3 : 1; i>0;i--) { /* He will go in the same direction for i times before the bot can move or he can chose a new direction */
                collisionIndicator = moveRider (grid, grid->player, &grid->playerRoute);
                if (collisionIndicator) isOver = 1;
                viewUpdate(grid);
            }
	    if (turboMovesLeft>0) turboMovesLeft--;
            
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
