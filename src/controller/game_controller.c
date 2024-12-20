#include "../../include/controller.h"
#include "../../include/model.h"
#include "../../include/view.h"
#include "../../include/types.h"
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int msleep(long sec, long msec)
{
    struct timespec req, rem;

    // Pause de 2.5 secondes
    req.tv_sec = sec;        // 2 secondes
    req.tv_nsec = msec; // 500 millisecondes (500 millions de nanosecondes)

    nanosleep(&req, &rem);
    return 0;
}

struct timespec addDelay(struct timespec original, long sec, long nsec){
    struct timespec delayed;

    delayed.tv_nsec = original.tv_nsec + nsec;
    delayed.tv_sec = original.tv_sec + sec + delayed.tv_nsec/1000000000;
    delayed.tv_nsec %= 1000000000;

    return delayed;
}

int difference(struct timespec time1, struct timespec time2){
    if (time1.tv_sec < time2.tv_sec) return 0;
    if (time1.tv_sec > time2.tv_sec) return 1;
    if (time1.tv_nsec < time2.tv_nsec) return 0;

    return 1;
}

void startGame(int n_lines, int n_columns) {
    Grid* grid = initGrid(n_lines, n_columns);
    viewInit();
    playGame(grid);
    endGame(grid);
}

void playGame(Grid* grid) {
    viewStart(grid);
    int collision = 0;
    int end = 0;
    struct timespec next_move;
    struct timespec time_ingame;
    timespec_get(&time_ingame, TIME_UTC);
    next_move = addDelay(time_ingame, 0, 500000000L);
    while(end==0) {
        timespec_get(&time_ingame, TIME_UTC);
        if (!difference(next_move, time_ingame)) {
            collision = moveRider (grid, PLAYER);
            if (collision) end = 1;

            collision = moveRider (grid, BOT);
            if (collision) end = 2;

            viewUpdate(grid);
            next_move = addDelay(time_ingame, 0L, 500000000L);
        }
    }
    
}

void endGame(Grid* grid) {
    freeGrid(grid);
    viewCleanup();
}
