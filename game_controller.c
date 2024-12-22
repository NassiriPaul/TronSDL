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

static int msleep(long sec, long nanosec)
{
    struct timespec req, rem;

    req.tv_sec = sec;
    req.tv_nsec = nanosec;

    nanosleep(&req, &rem);
    return 0;
}

static struct timespec addDelay(struct timespec original, long sec, long nsec){
    struct timespec delayed;

    delayed.tv_nsec = original.tv_nsec + nsec;
    delayed.tv_sec = original.tv_sec + sec + delayed.tv_nsec/1000000000;
    delayed.tv_nsec %= 1000000000;

    return delayed;
}

static int difference(struct timespec time1, struct timespec time2){
    if (time1.tv_sec < time2.tv_sec) return 0;
    if (time1.tv_sec > time2.tv_sec) return 1;
    if (time1.tv_nsec < time2.tv_nsec) return 0;

    return 1;
}

static void changeDirectionPlayer(Grid* grid, DIRECTIONS direction) {
    if ((direction == UP || direction == DOWN) && grid->player->direction != UP && grid->player->direction != DOWN) grid->player->direction = direction;
    if ((direction == LEFT || direction == RIGHT) && grid->player->direction != LEFT && grid->player->direction != RIGHT) grid->player->direction = direction;
}

static void changeDirectionBot(Grid* grid) {
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
    
}

int startGame(int n_lines, int n_columns) {
    Grid* grid = initGrid(n_lines, n_columns);
    viewInit();
    int jeu=playGame(grid);
    endGame(grid);
    return jeu;
}

int playGame(Grid* grid) {
    viewStart(grid);

    int collision;
    int end = 0;

    struct timespec next_move;
    struct timespec time_ingame;

    timespec_get(&time_ingame, TIME_UTC);
    next_move = addDelay(time_ingame, 0L, 200000000L);
    DIRECTIONS input;
    while(!end) {
        getDirection(&input);
        changeDirectionPlayer(grid, input);
        timespec_get(&time_ingame, TIME_UTC);
        if (!difference(next_move, time_ingame)) {
            collision = moveRider (grid, PLAYER);
            if (collision) end = 1;

            collision = moveRider (grid, BOT);
            if (collision) end = 2;

            viewUpdate(grid);
            changeDirectionBot(grid);
            next_move = addDelay(time_ingame, 0L, 200000000L);
            
        }
    }
    return end;
}

void endGame(Grid* grid) {
    freeGrid(grid);
    viewCleanup();
}