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

void sleep_ms(int milliseconds)
{
    usleep(milliseconds * 1000);
}

static void changeDirectionPlayer(Grid* grid, DIRECTIONS direction) {
    if ((direction == UP || direction == DOWN) && grid->player->direction != UP && grid->player->direction != DOWN) grid->player->direction = direction;
    if ((direction == LEFT || direction == RIGHT) && grid->player->direction != LEFT && grid->player->direction != RIGHT) grid->player->direction = direction;
}
/*
static void changeDirectionBot(Grid* grid) {
    
	int presumed_next_pos_x,  presumed_next_pos_y;
    int collision = 0;
    collision = checkCollision (grid, BOT);
    if (grid->bot->direction == UP || grid->bot->direction == DOWN){
        if(!collision) return;
        grid->bot->direction = LEFT;
        collision = checkCollision (grid, BOT);
        if(!collision) return;
        grid->bot->direction = RIGHT;
        return;
    }
    if (grid->bot->direction == LEFT || grid->bot->direction == RIGHT){
        if(!collision) return;
        grid->bot->direction = UP;
        collision = checkCollision (grid, BOT);
        if(!collision) return;
        grid->bot->direction = DOWN;
        return;
    } 
}*/


int startGame(int n_lines, int n_columns) {
    int jeu;
    Grid* grid;

    
    viewInit();
    
    grid = initGrid(n_lines, n_columns);
    jeu = playGame(grid);

    endGame(grid);

    return jeu;
}

int playGame(Grid* grid) {
    int collisionIndicator, isOver, remainingTimePlayer, remainingTimeBot;
    DIRECTIONS input;
    clock_t start, end;
     unsigned long elapsed, remaining;

    viewStart(grid);


    isOver = 0;
    while(!isOver) {

        input = grid->player->direction;
        remainingTimePlayer = 200;
        while (remainingTimePlayer>0){ /* While the player have the time to change direction, we let him change it*/
            remainingTimePlayer = getDirection(&input, remainingTimePlayer);
        }
        changeDirectionPlayer(grid, input);
        collisionIndicator = moveRider (grid, grid->player, &grid->playerRoute);
        if (collisionIndicator) isOver = 1;
        viewUpdate(grid);
        
        

        /*collision = moveRider (grid, BOT);
        if (collision) end = 2;
        changeDirectionBot(grid);*/
        viewUpdate(grid);
    }
    return isOver;
}

void endGame(Grid* grid) {
    freeGrid(grid);
    viewCleanup();
}
