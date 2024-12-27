#pragma once
#include "model.h"

void viewInit();
void viewStart(Grid *grid, int scorePlayer, int scoreBot);
void viewUpdate(Grid *grid);
void updateViewTurbos(int n_lines, int turbos);
void updateViewScore(int n_lines, int scorePlayer, int scoreBot);
void viewCleanup();
