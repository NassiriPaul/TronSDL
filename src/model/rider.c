#include "../../include/model.h"
#include <stdlib.h>
#include <stdio.h>
Rider* initRider (int x, int y) {
	Rider* rider = (Rider*) malloc (sizeof(Rider));
	rider->pos_x = x;
	rider->pos_y = y;
	rider->turbos = 0;
	return rider;
}

void freeRider (Rider* r){
	free(r);
}