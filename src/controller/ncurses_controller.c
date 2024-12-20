//#include....
#include <type.h>
#include <curses.h>

void getDirection(DIRECTIONS* direction){
    noecho();
    halfdelay(10); //attends 1 seconde
    int key=getch();
    switch (key){
        case 0:
            break;
        case KEY_UP:
            *direction = UP;
            break;
        case KEY_RIGHT:
            *direction = RIGHT;
            break;
        case KEY_DOWN:
            *direction = DOWN;
            break;
        case KEY_LEFT:
            *direction = LEFT;
            break;
        default:
            break;
    }
}
