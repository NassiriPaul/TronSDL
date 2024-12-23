#include <stdlib.h>
#include <stdio.h>

#include "../../include/model.h"
#include "../../include/types.h"

/*Minimum 3 de longeur et 1 de largeur sinon return NULL*/ 
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
	Dot *current;
	printf("Bot route : ");
	if (!grid->botRoute)
    {
        printf("No Route");
    }

    current = grid->botRoute;
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

void setPresumedNextDirection(Rider* rider, int* next_x, int* next_y){
	/*Set presumed next positions*/
	switch (rider->direction)
	{
		case UP:
			*next_y = rider->pos_y-1;
			*next_x = rider->pos_x;
			break;
		case RIGHT:
			*next_y = rider->pos_y;
			*next_x = rider->pos_x+1;
			break;
		case DOWN:
			*next_y = rider->pos_y+1;
			*next_x = rider->pos_x;
			break;
		case LEFT:
			*next_y = rider->pos_y;
			*next_x = rider->pos_x-1;
			break;
	}
}


static int checkCollisionWithTrail(Route route, int pos_x, int pos_y) {
	Dot* dot;
	dot = route;
	while (dot) {
		if (dot->pos_x == pos_x && dot->pos_y == pos_y) return 1;
		dot = dot->nextDot;
	}
	return 0;
}

int checkCollision (Grid* grid, int presumed_next_pos_x, int presumed_next_pos_y) {
	if (!grid) {perror ("No grid allocated yet or NULL");return 1;}
	if(( presumed_next_pos_x <= 0 || presumed_next_pos_y <= 0)) return 2;
	if(( presumed_next_pos_x >= grid->n_columns-1 || presumed_next_pos_y >= grid->n_lines-1)) return 2;
	if(checkCollisionWithTrail(grid->playerRoute, presumed_next_pos_x, presumed_next_pos_y)) return 2;
	if(checkCollisionWithTrail(grid->botRoute, presumed_next_pos_x, presumed_next_pos_y)) return 2;

	if (presumed_next_pos_x == grid->player->pos_x && presumed_next_pos_y == grid->player->pos_y) return 3;
	if (presumed_next_pos_x == grid->bot->pos_x && presumed_next_pos_y == grid->bot->pos_y) return 3;

	return 0;
}

int moveRider (Grid* grid,  Rider* rider, Route* route) {
	DIRECTIONS direction;
	int presumed_next_pos_x;
	int presumed_next_pos_y;
	int indicatorCollision;

	if (!grid) {
		perror ("No grid allocated yet or NULL in MoveRider");
		return 1;
	}
	if (!rider) {
		perror ("The moving rider in the grid is not allocated in MoveRider");
		return 1;
	}
	direction = rider->direction;

	if (!rider) {perror ("The moving rider in the grid is not allocated");return 1;}


	setPresumedNextDirection(rider, &presumed_next_pos_x, &presumed_next_pos_y);
	indicatorCollision = checkCollision (grid, presumed_next_pos_x, presumed_next_pos_y);
	if (indicatorCollision != 0) return indicatorCollision;

	*route = updateRoute (rider, *route);
	
	switch (rider->direction)
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
	/*printf("(%d, %d)\n", rider->pos_x, rider->pos_y);*/

	return 0;
}