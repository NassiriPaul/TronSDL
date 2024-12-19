#pragma once

typedef enum {UP, RIGHT, DOWN, LEFT} DIRECTIONS;
typedef enum {BOT, PLAYER} RIDERS;

typedef struct{
    int x;
    int y;
} Pos;

Pos* addPos (int x, int y); //pas sur de son utiliter (si on malloc rider par exemple, on malloc aussi position en meme temps)