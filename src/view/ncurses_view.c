#include "../../include/view.h"
#include <curses.h>
#include <stdlib.h>
#include "view.h"

void viewInit() {
    // Initialisation de ncurses
    initscr();            // Démarre le mode ncurses
    cbreak();             // Mode cbreak : on ne bufferise pas toute la ligne
    noecho();             // N'affiche pas les caractères tapés
    curs_set(0);          // Cache le curseur
    keypad(stdscr, TRUE); // Permet l'utilisation des touches spéciales (flèches, etc.)
}

void viewStart(Grid *grid){

    clear();

    int n_lines = grid->n_lines;
    int n_columns = grid->n_columns;

    for (int i = 0; i < n_columns; i++) {
        mvaddch(0, i, '-');
        mvaddch(n_lines - 1, i, '-');
    }
    for (int j = 0; j < n_lines; j++) {
        mvaddch(j, 0, '|');
        mvaddch(j, n_columns - 1, '|');
    }

    if (grid->player) mvaddch(grid->player->pos_y, grid->player->pos_x, 'P');
    if (grid->bot) mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'B');

    // Rafraîchit l'écran pour montrer les modifications
    refresh();
}

void viewUpdate(Grid *grid) {
    Dot *d = grid->playerRoute;
    if (d) mvaddch(d->pos_y, d->pos_x, 'o');

    d = grid->botRoute;
    if (d) mvaddch(d->pos_y, d->pos_x, 'x'); 

    mvaddch(grid->player->pos_y, grid->player->pos_x, 'P');
    mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'B');

    refresh();
}

void viewCleanup() {
    endwin();
}