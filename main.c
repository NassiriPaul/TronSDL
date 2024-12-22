#include <stdlib.h>
#include <stdio.h>
#include "../include/controller.h"

int main(){
	int n = 20, m = 50;
	int jeu=startGame(n, m);
	if(jeu==1) printf("Joueur perds");
    if(jeu==2) printf("BOT perds");
	return 0;
}
