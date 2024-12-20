//#include....
#include <type.h>
#include <curses.h>

DIRECTION getDirection(){
    noecho();
    key=getch();
    swicth (key){
        case "z"|:
        case KEY_UP:
            return UP;
        case "d":
        case KEY_RIGHT:
            return RIGHT;
        case "s":
        case KEY_DOWN:
            return DOWN;
        case "q":
        case KEY_LEFT:
            return LEFT;
    }
}