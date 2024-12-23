#pragma once
#include "model.h"

void viewInit();
void viewStart(Grid *grid);
void viewUpdate(Grid *grid);
void updateViewScore(int n_lines, int scorePlayer, int scoreBot);
void viewCleanup();