#include <stdlib.h>
#include <stdio.h>
#include "../include/types.h"
#include "../include/model.h"

int main(){
	int n = 10, m = 10;
	Grid* grid = initGrid (n, m);

	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, RIGHT, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	printGrid (grid);
	freeGrid (grid);
	return 0;
}
