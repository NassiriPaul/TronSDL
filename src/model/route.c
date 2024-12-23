#include "../../include/model.h"
#include <stdlib.h>
#include <stdio.h>
void freeRoute (Route r){
	Dot *current = r;
    Dot *nextDot;
    
    while (current) {
        nextDot = current->nextDot; 
        free(current);
        current = nextDot;
    }
}

Route updateRoute (Rider* rider, Route route) {
    Dot *newDot = (Dot *)malloc(sizeof(Dot));
    if (!newDot) {
        perror("Error allocating new dot\n");
        return route;
    }
    newDot->pos_x = rider->pos_x;
	newDot->pos_y = rider->pos_y;
    newDot->nextDot = route;
    
    return newDot;
}