#pragma once
#include "types.h"

/**
 * @struct Rider
 * @brief Represents a character playable by an human or by the bot
 */
typedef struct {
	int pos_x;
	int pos_y;
	int turbos;
	DIRECTIONS direction;
} Rider;

/**
 * @brief Rider initialization
 * 
 * @param x position in x axis 
 * @param y position in y axis
 * @return the Rider
 */
Rider *initRider(int x, int y);

/**
 * @brief Rider deallocation
 * @param r the rider
 */
void freeRider(Rider *r);

/**
 * @struct Dot
 * @brief Represent the trail behind players in linked list form
 */
typedef struct dot {
	int pos_x;
    int pos_y;
	struct dot *nextDot;
} Dot;
typedef Dot* Route;

/**
 * @brief Create a new node behind the rider
 * 
 * @param rider the rider
 * @param route the trail behind the rider
 */
Route updateRoute(Rider *rider, Route route);

/**
 * @brief Trail deallocation
 * 
 * @param r the trail
 */
void freeRoute (Route r);

/**
 * @struct Grid
 * @brief Represents a grid of n_lines X n_columns
 */
typedef struct {
	int n_lines;
	int n_columns;

	Rider* player;
	Rider* bot;

	Route playerRoute;
	Route botRoute;

} Grid;

/**
 * @brief Grid Initialization
 * @return the grid
 */
Grid *initGrid();

/**
 * @brief Grid deallocation
 * 
 * @param grid the grid
 */
void freeGrid(Grid *grid);

/**
 * @brief Set the next direction of the rider
 * 
 * @param rider the rider
 * @param next_x Pointer of the next position in the x axis
 * @param next_y Pointer of the next position in the y axis
 */
void setPresumedNextDirection(Rider* rider, int* next_x, int* next_y);

/**
 * @brief Check if a the rider can move in the grid toward a direction
 * 
 * @param grid the grid
 * @param presumed_next_position_x the next position in the x axis
 * @param presumed_next_position_y the next position in the y axis
 * 
 * @return 0 : the rider can be moved / 1 : an error occured / 2 : the rider will collide with the wall / 3 : the rider will collide with the other rider
 */
int checkCollision (Grid* grid, int presumed_next_position_x, int presumed_next_position_y);

/**
 * @brief Move a rider in the grid toward a direction
 * 
 * @param grid the grid
 * @param rider the rider
 * @param route the trail will be update after the move
 * @return 0 : the rider have move / 1 : an error occured / 2 : the rider have collide with the wall / 3 : the rider have collide with the other rider
 */
int moveRider (Grid* grid,  Rider* rider, Route* route);