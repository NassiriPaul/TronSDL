#pragma once



typedef struct dot {
	Pos *pos;
	struct dot *nextDot;
} Dot;

typedef Dot* Route;

void updateRoute(Rider *rider, Route *route);

