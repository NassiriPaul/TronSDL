#include "../../include/view.h"
#include <curses.h>
#include <stdlib.h>

/* Initialize the view with ncurses settings */
void viewInit() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

/* Draw the grid boundaries and initial positions of entities */
void viewStart(Grid *grid) {
    int i, j;

    if (!grid) return;
    clear();
    for (i = 0; i < grid->n_columns; i++) 
        mvaddch(grid->n_lines, i, '-');

    for (j = 0; j < grid->n_lines; j++) 
        mvaddch(j, grid->n_columns, '|');

    if (grid->player) 
        mvaddch(grid->player->pos_y, grid->player->pos_x, 'P');
    if (grid->bot) 
        mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'B');

    refresh();
}

/* Update the positions of player and bot, and their routes */
void viewUpdate(Grid *grid) {
    Dot *d;

    if (!grid) return;
    d = grid->playerRoute;
    if (d) 
        mvaddch(d->pos_y, d->pos_x, 'o');
    d = grid->botRoute;
    if (d) 
        mvaddch(d->pos_y, d->pos_x, 'x');
    if (grid->player) 
        mvaddch(grid->player->pos_y, grid->player->pos_x, 'P');
    if (grid->bot) 
        mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'B');

    refresh();
}

/* Update the scores of the player and bot */
void updateViewScore(int n_lines, int scorePlayer, int scoreBot) {
    noecho();
    mvprintw(n_lines + 1, 0, "Player : %d", scorePlayer);
    mvprintw(n_lines + 2, 0, "Bot : %d", scoreBot);
    refresh();
}

/* Clean up the ncurses environment */
void viewCleanup() {
    clear();
    curs_set(1);
    endwin();
}
