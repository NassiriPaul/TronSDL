#include "../../include/view.h"
#include <curses.h>
#include <stdlib.h>
#include "view.h"

void viewInit() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    keypad(stdscr, TRUE);
}

void viewStart(Grid *grid, int scorePlayer, int scoreBot){
    int i;
    int j;
    int n_lines;
    int n_columns;
    DIRECTIONS firstDirection;

    n_lines = grid->n_lines;
    n_columns = grid->n_columns;

    clear();

    for (i = 0; i < n_columns; i++) {
        mvaddch(0, i, '-');
        mvaddch(n_lines - 1, i, '-');
    }
    for (j = 0; j < n_lines; j++) {
        mvaddch(j, 0, '|');
        mvaddch(j, n_columns - 1, '|');
    }

    if (grid->player) mvaddch(grid->player->pos_y, grid->player->pos_x, 'O');
    if (grid->bot) mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'O');

    
    updateViewScore(grid->n_lines, scorePlayer, scoreBot);

    refresh();
    while (1) {
        firstDirection = getch();
        switch (firstDirection)
        {
        case KEY_UP: grid->player->direction = UP;return;
        case KEY_RIGHT: grid->player->direction = RIGHT;return;
        case KEY_DOWN: grid->player->direction = DOWN;return;
        case KEY_LEFT: grid->player->direction = LEFT;return;
        default:
            break;
        }
    }
    
}

void viewUpdate(Grid *grid) {
    Dot *d;
    
    d = grid->playerRoute;
    if (d){
        attron(COLOR_PAIR(3)); 
        mvaddch(d->pos_y, d->pos_x, 'X');
        attroff(COLOR_PAIR(3)); 
    }

    d = grid->botRoute;
    if (d){
        attron(COLOR_PAIR(4)); 
        mvaddch(d->pos_y, d->pos_x, 'X');
        attroff(COLOR_PAIR(4)); 

    }

    attron(COLOR_PAIR(1));
    mvaddch(grid->player->pos_y, grid->player->pos_x, 'O');
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvaddch(grid->bot->pos_y, grid->bot->pos_x, 'O');
    attroff(COLOR_PAIR(2));

    refresh();
}

void updateViewScore(int n_lines, int scorePlayer, int scoreBot) {
    noecho();
    mvprintw(n_lines+1, 0, "Player : %d", scorePlayer);
    mvprintw(n_lines+2, 0, "Bot : %d", scoreBot);
    refresh();
}

void viewCleanup() {
    clear();
    endwin();
}