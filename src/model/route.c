#include "../../include/model.h"
#include <stdlib.h>
#include <stdio.h>
void freeRoute (Route r){
	Dot *current = r;
    Dot *nextDot;
    
    while (current) {
        nextDot = current->nextDot; // Sauvegarde le prochain élément
        free(current);               // Libère l'élément actuel
        current = nextDot;           // Passe au suivant
    }
}

Route updateRoute (Rider* rider, Route route) {
    // Crée un nouveau dot
    Dot *newDot = (Dot *)malloc(sizeof(Dot));
    if (!newDot) {
        // Vérifie si l'allocation mémoire a échoué
        perror("Error allocating new dot\n");
        return route;
    }
    
    // Initialiser les valeurs du nouveau Dot
    newDot->pos_x = rider->pos_x;
    newDot->pos_y = rider->pos_y;
    newDot->nextDot = route; // Le nouveau Dot pointe vers le premier élément
    
    // Le nouveau Dot devient maintenant le premier de la liste
    return newDot;
}
