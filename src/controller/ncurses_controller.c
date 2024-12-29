#include <time.h>
#include <ncurses.h>
#include "types.h"
#include <sys/time.h> /* gettimeofday, struct timeval*/

int getInput(int* input, unsigned long ms)
{
    struct timeval start, stop; /* timestamps to get time elapsed and time remaining */
    unsigned long elapsed; /* elapsed time in milliseconds */
    unsigned long remaining;
    int ch;

    gettimeofday(&start, NULL);

    halfdelay((int)(ms / 100));/* say to ncurses to wait ms milliseconds to get an input. halfdelay take tenth of seconds that's why it is divide by 100 */
    noecho();
    
    ch = getch();

    /* Interpret the input */
    switch (ch) {
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
        case 116:  /*Touche t*/
            *direction = 5;  
            break;  
    }
    
    /* Now record the end time */
    gettimeofday(&stop, NULL);

    /* Convert difference to milliseconds */
    elapsed = (unsigned long)((stop.tv_sec - start.tv_sec) * 1000UL) + (unsigned long)((stop.tv_usec - start.tv_usec) / 1000UL);

    /*  constrain the answer */
    if (elapsed > ms) {
        elapsed = ms;
    }

    remaining = ms - elapsed;

    return (int)remaining;
}
