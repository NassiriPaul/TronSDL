#include <stdlib.h>
#include <stdio.h>

#include "../../include/model.h"

// Minimum 3 de longeur et 1 de largeur sinon return NULL
Grid* initGrid (int n_lines, int n_columns) {
	Grid* grid = (Grid*) malloc (sizeof(Grid));

	grid->n_lines = n_lines;
	grid->n_columns = n_columns;

	grid->player = initRider((n_columns*2)/3, n_lines/2);
	grid->bot = initRider(n_columns/3, n_lines/2);

	grid->playerRoute = (Route) malloc (sizeof(Dot));
	grid->botRoute = (Route) malloc (sizeof(Dot));
	grid->playerRoute = grid->botRoute = NULL;

	return grid;
}

void printGrid (Grid* grid){
	printf("Bot route : ");
	if (!grid->botRoute)
    {
        printf("No Route");
    }
    Dot *current = grid->botRoute;
    while (current != NULL)
    {
        printf("(%d, %d) <- ", current->pos_x, current->pos_y);
        current = current->nextDot;
    }
	printf("\n");
	printf("Player route : ");
	if (!grid->playerRoute)
    {
        printf("No Route");
    }
    current = grid->playerRoute;
    while (current != NULL)
    {
        printf("(%d, %d) <- ", current->pos_x, current->pos_y);
        current = current->nextDot;
    }
    printf("NULL\n");
	printf("Player position : (%d, %d)\n", grid->player->pos_x, grid->player->pos_y);
	printf("Bot position : (%d, %d)\n", grid->bot->pos_x, grid->bot->pos_y);
}

void freeGrid(Grid* grid) {
	freeRider (grid->player);
	freeRider (grid->bot);
	freeRoute (grid->playerRoute);
	freeRoute (grid->botRoute);
	free (grid);
}

int checkCollision (Grid* grid, DIRECTIONS direction, RIDERS movingRider) {
	if (!grid) {
		perror ("No grid allocated yet or NULL");
		return 1;
	}

	Rider* rider;
	Rider* otherRider;
	int presumed_next_pos_x;
	int presumed_next_pos_y;
	

	if (movingRider == PLAYER) {
		rider = grid->player;
		otherRider = grid->bot;
	} else {
		rider = grid->bot;
		otherRider = grid->bot;
	}

	if (!rider) {
		perror ("The moving rider in the grid is not allocated");
		return 1;
	}

	if (!otherRider) {
		perror ("The other moving rider in the grid is not allocated");
		return 1;
	}

	// Set presumed next positions
	switch (direction)
	{
		case UP:
			presumed_next_pos_y = rider->pos_y-1;
			presumed_next_pos_x = rider->pos_x;
			break;
		case RIGHT:
			presumed_next_pos_y = rider->pos_y;
			presumed_next_pos_x = rider->pos_x+1;
			break;
		case DOWN:
			presumed_next_pos_y = rider->pos_y+1;
			presumed_next_pos_x = rider->pos_x;
			break;
		case LEFT:
			presumed_next_pos_y = rider->pos_y;
			presumed_next_pos_x = rider->pos_x-1;
			break;
		default:
			printf("here");
	}

	// check collision with the grid wall
	// Reduire en deux if avec un MODULO
	if ( presumed_next_pos_y < 0 ) {
		printf("Rider went into the top wall in (%d, %d)\n", rider->pos_x, rider->pos_y);
		return 2;
	}
	if ( presumed_next_pos_y >= grid->n_lines ) {
		printf("Rider went into the bottom wall in (%d, %d)\n", rider->pos_x, rider->pos_y);
		return 2;
	}
	if ( presumed_next_pos_x < 0 ) {
		printf("Rider went into the left wall in (%d, %d)\n", rider->pos_x, rider->pos_y);
		return 2;
	}
	if ( presumed_next_pos_x >= grid->n_columns ) {
		printf("Rider went into the right wall in (%d, %d)\n", rider->pos_x, rider->pos_y);
		return 2;
	}

	// check both routes
	Dot* dot;
	dot = grid->botRoute;
	while (dot)
	{
		if (dot->pos_x == presumed_next_pos_x && dot->pos_y == presumed_next_pos_y) {
			printf("Collide with bot route\n");
			return 2;
		}
		dot = dot->nextDot;
	}

	dot = grid->playerRoute;
	while (dot)
	{
		if (dot->pos_x == presumed_next_pos_x && dot->pos_y == presumed_next_pos_y) {
			printf("Collide with player route\n");
			return 2;
		}
		dot = dot->nextDot;
	}

	// Check if the moving rider hit the other rider
	if (presumed_next_pos_x == otherRider->pos_x && presumed_next_pos_y == otherRider->pos_y) return 3;

	return 0;
}

int move (Grid* grid, DIRECTIONS direction, RIDERS movingRider) {
	if (!grid) {
		perror ("No grid allocated yet or NULL");
		return 1;
	}
	Rider* rider;

	int indicatorCollision = checkCollision (grid, direction, movingRider);
	if (indicatorCollision != 0) return indicatorCollision;

	if (movingRider == PLAYER) {
		rider = grid->player;
		grid->playerRoute = updateRoute (rider, grid->playerRoute);
	}
	else {
		rider = grid->bot;
		grid->botRoute = updateRoute (rider, grid->botRoute);
	}
	printf("Rider was (%d, %d) and is now ", rider->pos_x, rider->pos_y);
	switch (direction)
	{
		case UP:
			rider->pos_y -= 1;
			break;
		case RIGHT:
			rider->pos_x += 1;
			break;
		case DOWN:
			rider->pos_y += 1;
			break;
		case LEFT:
			rider->pos_x -= 1;
			break;
	}
	printf("(%d, %d)\n", rider->pos_x, rider->pos_y);

	return 0;
}

