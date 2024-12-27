#include "../../include/model.h"
#include "../../include/types.h"
#include <stdlib.h>

Rider* initRider (int x, int y) {
	Rider* rider = (Rider*) malloc (sizeof(Rider));
	rider->pos_x = x;
	rider->pos_y = y;
	rider->turbos = 3;
	rider->direction = UP;
	return rider;
}

void freeRider (Rider* r){
	free(r);
}
