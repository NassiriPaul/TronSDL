#pragma once

typedef struct{
    int x;
    int y;
} Pos;

Pos *initPos(int x, int y); /*pas sur de son utiliter (si on malloc rider par exemple, on malloc aussi position en meme temps)


