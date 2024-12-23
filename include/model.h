#pragma once
#include "types.h"

typedef struct {
	int pos_x;
	int pos_y;
	int turbos;
	DIRECTIONS direction;
} Rider;

Rider *initRider(int x, int y);
void freeRider(Rider *r);

typedef struct dot {
	int pos_x;
    int pos_y;
	struct dot *nextDot;
} Dot;
typedef Dot* Route;

Route updateRoute(Rider *rider, Route route);
void freeRoute (Route r);

/*GRID*/ 
typedef struct {
	int n_lines;
	int n_columns;

	Rider* player;
	Rider* bot;

	Route playerRoute;
	Route botRoute;

} Grid;

Grid *initGrid(int n_lines, int n_columns);
void printGrid(Grid *grid);
void freeGrid(Grid *grid);
void setPresumedNextDirection(Rider* rider, int* next_x, int* next_y);
/*
Check if a ridercan move in the grid toward a direction
Return possibility:
	0 means that the rider can be moved
	1 means an error occured
	2 means the rider will hit the wall of the grid or a route
	3 means the rider will hit the other rider
*/
int checkCollision (Grid* grid, int presumed_next_position_x, int presumed_next_position_y);
/*
Move a rider in the grid toward a direction
Return possibility:
	0 means that the rider has been moved successfully
	1 means an error occured
	2 means the rider hit the wall of the grid or a route
	3 means the rider hit the other rider
*/
int moveRider (Grid* grid,  Rider* rider, Route* route);