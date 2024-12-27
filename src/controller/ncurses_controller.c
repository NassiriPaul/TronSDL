#include <time.h>       /* clock_gettime, struct timespec, etc. (POSIX) */
#include <ncurses.h>    /* noecho(), halfdelay(), getch(), etc.         */
#include "types.h"      /* For DIRECTIONS enum, etc.                    */
#include <sys/time.h>   /* gettimeofday, struct timeval                 */

int getInput(int* direction, unsigned long ms)
{
    struct timeval start, stop;
    unsigned long elapsed;     /* elapsed time in milliseconds */
    unsigned long remaining;
    int ch;

    gettimeofday(&start, NULL);

    /*
     * halfdelay() expects tenths of a second. 
     * For example, if ms=1000, ms/100=10 -> halfdelay(10) = 1 second max.
     */
    halfdelay((int)(ms / 100));
    noecho();

    /* Attempt to read one character within halfdelay(...) */
    ch = getch();  /* returns ERR if no key pressed within that time */

    /* Now record the end time */
    gettimeofday(&stop, NULL);

    /* Convert difference to milliseconds:
       (stop.tv_sec - start.tv_sec) -> seconds difference
       (stop.tv_usec - start.tv_usec) -> microseconds difference
    */
    elapsed = (unsigned long)((stop.tv_sec - start.tv_sec) * 1000UL)
            + (unsigned long)((stop.tv_usec - start.tv_usec) / 1000UL);

    /* If user took more than ms, clamp to ms */
    if (elapsed > ms) {
        elapsed = ms;
    }

    remaining = ms - elapsed;

    /* Interpret the key */
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
            return 0;
            break;
            
    }

    return (int)remaining;  /* Return leftover time in ms */
}
