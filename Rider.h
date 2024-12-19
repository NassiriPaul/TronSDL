#pragma once

//a voir si on mets sa direction

typedef struct {
	Pos *pos;
	int turbos;
    int scores;
} Rider;

Rider *initRider(int x, int y);
void freeRider(Rider *r);
void printPosRider(Rider *r);

