#pragma once

typedef struct {
	int n_lines;
	int n_columns;

	Rider* player;
	Rider* bot;

	Route* playerRoute;
	Route* botRoute;

} Grid;

Grid *initGrid(int n_lines, int n_columns);
void printGrid(Grid *grid);
void freeGrid(Grid *grid);
int checkCollision (Grid* grid, DIRECTIONS direction, RIDERS movingRider);