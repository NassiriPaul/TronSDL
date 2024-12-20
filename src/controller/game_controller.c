#include "../../include/controller.h"
#include "../../include/model.h"
#include "../../include/view.h"
#include "../../include/types.h"
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
static int msleep(long sec, long msec)
{
    struct timespec req, rem;

    // Pause de 2.5 secondes
    req.tv_sec = sec;        // 2 secondes
    req.tv_nsec = msec; // 500 millisecondes (500 millions de nanosecondes)

    if (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR) {
            printf("Le sommeil a été interrompu.\n");
            printf("Temps restant: %ld secondes et %ld nanosecondes\n", rem.tv_sec, rem.tv_nsec);
        } else {
            perror("Erreur dans nanosleep");
        }
        return 1;
    }
    return 0;
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
    while(end == 0) {
        collision = moveRider (grid, PLAYER);
        if (collision) end = 1;

        collision = moveRider (grid, BOT);
        if (collision) end = 2;

        viewUpdate(grid);
    }
    
}

void endGame(Grid* grid) {
    freeGrid(grid);
    viewCleanup();
}