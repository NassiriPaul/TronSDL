#include <stdlib.h>
#include <stdio.h>
#include "../include/controller.h"

int main(){
	int n, m, res;
	n = 20, m = 50;
	res = startGame(n, m);
	
    res==1 ? printf("PLAYER HAVE WIN") : printf("PLAYER HAVE LOST");
	return 0;
}
