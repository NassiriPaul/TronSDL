#include <stdlib.h>
#include <stdio.h>

#include <Position.h>
#include <Grid.h>
#include <Rider.h>
#include <Route.h>

/*
A IMPLEMENTER UN STRUCT POSITION
*/

typedef enum {UP, RIGHT, DOWN, LEFT} DIRECTIONS;
typedef enum {BOT, PLAYER} RIDERS;

/** Rider.h

typedef struct {
	int pos_x;
	int pos_y;
	int turbos;
} Rider;

*/

//Méthode pour rider

Rider* initRider (int x, int y) {
	Rider* rider = (Rider*) malloc (sizeof(Rider));
	rider->pos={x,y};
	rider->turbos = 0;
	return rider;
}

void freeRider (Rider* rider){
	free(rider);
}

void printPosRider(Rider *rider){
    printf("Position rider:(%d,%d)\n", rider->pos.x, rider->pos.y);
}

/** Route.h

typedef struct dot {
	int pos_x;
	int pos_y;
	struct dot *nextDot;
} Dot;

typedef Dot* Route;

*/

// A REVOIR
void freeRoute (Route* r){
	Dot *current = r->premier;
    Dot *nextDot;
    
    while (current) {
        nextDot = current->nextDot; // Sauvegarde le prochain élément
        free(current);               // Libère l'élément actuel
        current = nextDot;           // Passe au suivant
    }
    
    r->premier = NULL;
	free(r);// Une fois la liste vide, on met le premier à NULL
}


/** Grid.h

typedef struct {
	int n_lines;
	int n_columns;

	Rider* player;
	Rider* bot;

	Route* playerRoute;
	Route* botRoute;

} Grid;

*/


// Minimum 3 de longeur et 1 de largeur sinon return NULL
Grid* initGrid (int n_lines, int n_columns) {
	Grid* grid = (Grid*) malloc (sizeof(Grid));

	grid->n_lines = n_lines;
	grid->n_columns = n_columns;

	grid->player = initRider((n_columns*2)/3, n_lines/2);
	grid->bot = initRider(n_columns/3, n_lines/2);

	grid->playerRoute = (Route*) malloc (sizeof(Route));
	grid->botRoute = (Route*) malloc (sizeof(Route));
	grid->playerRoute->premier = grid->botRoute->premier = NULL;

	return grid;
}

void printGrid (Grid* grid){
	// check both routes
	Dot* dot;
	dot = grid->botRoute->premier; //a changer du au changement de route
	printf("Bot route : ");
	if (!dot) printf("No route\n");
	else printf("\n");
	while (dot)
	{
		printf("(%d, %d)\n", dot->pos_x, dot->pos_x);
		dot = dot->nextDot;
	}
	printf("Player route : ");
	
	dot = grid->playerRoute->premier;
	if (!dot) printf("No route\n");
	else printf("\n");
	while (dot)
	{
		printf("(%d, %d)\n", dot->pos_x, dot->pos_x);
		dot = dot->nextDot;
	}
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

/*
Check if a ridercan move in the grid toward a direction
Return possibility:
	0 means that the rider can be moved
	1 means an error occured
	2 means the rider will hit the wall of the grid or a route
	3 means the rider will hit the other rider
*/
int checkCollision (Grid* grid, DIRECTIONS direction, RIDERS movingRider) {
	if (!grid) {
		perror ("No grid allocated yet or NULL");
		return 1;
	}

	Rider* rider;
	Rider* otherRider;
	Pos *presumed_next_pos;
	

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
            presumed_next_pos = {rider->pos.x,rider->pos.y-1}
			break;
		case RIGHT:
            presumed_next_pos = {rider->pos.x+1,rider->pos.y}
			break;
		case DOWN:
            presumed_next_pos = {rider->pos.x,rider->pos.y+1}
			break;
		case LEFT:
            presumed_next_pos = {rider->pos.x-1,rider->pos.y}
			break;
		default:
			printf("here");
	}

	// check collision with the grid wall
	if ( presumed_next_pos.y % grid->n_lines < 0 ) {
		printf("Rider went into the -- wall in (%d, %d)\n", rider->pos.x, rider->pos.y);
		return 2;
	}
	if ( presumed_next_pos.x % grid->n_lines < 0 ) {
		printf("Rider went into the | wall in (%d, %d)\n", rider->pos.x, rider->pos.y);
		return 2;
	}


	// check both routes
	Dot* dot;
	dot = grid->botRoute->premier;
	while (dot)
	{
		if (dot->pos_x == presumed_next_pos.x && dot->pos_y == presumed_next_pos.y) {
			printf("Collide with bot route\n");
			return 2;
		}
		dot = dot->nextDot;
	}

	dot = grid->playerRoute->premier;
	while (dot)
	{
		if (dot->pos_x == presumed_next_pos.x && dot->pos_y == presumed_next_pos.y) {
			printf("Collide with player route\n");
			return 2;
		}
		dot = dot->nextDot;
	}

	// Check if the moving rider hit the other rider
	if (presumed_next_pos_x == otherRider->pos_x && presumed_next_pos_y == otherRider->pos_y) return 3;

	return 0;
}

void updateRoute (Rider* rider,Route* route) {
    // Crée un nouveau dot
    Dot *newDot = (Dot *)malloc(sizeof(Dot));
    if (!newDot) {
        // Vérifie si l'allocation mémoire a échoué
        perror("Error allocating new dot\n");
        return;
    }
    
    // Initialiser les valeurs du nouveau Dot
    newDot->pos_x = rider->pos_x;
	newDot->pos_y = rider->pos_y;
    newDot->nextDot = route->premier; // Le nouveau Dot pointe vers le premier élément
    
    // Le nouveau Dot devient maintenant le premier de la liste
    route->premier = newDot;
}
/*
Move a rider in the grid toward a direction
Return possibility:
	0 means that the rider has been moved successfully
	1 means an error occured
	2 means the rider hit the wall of the grid or a route
	3 means the rider hit the other rider
*/
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
		updateRoute (rider, grid->playerRoute);
	}
	else {
		rider = grid->bot;
		updateRoute (rider, grid->botRoute);
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

int main(){
	int n = 10, m = 10;
	Grid* grid = initGrid (n, m);

	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	move (grid, UP, PLAYER);
	printGrid (grid);
	freeGrid (grid);
	return 0;
}