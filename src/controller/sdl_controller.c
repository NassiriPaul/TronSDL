#include <SDL2/SDL.h>
#include "types.h"


int getDirection(DIRECTIONS *direction, unsigned long ms)
{
    SDL_Event e;
    unsigned long start, now, elapsed, remaining;

    /* Record the start time in milliseconds */
    start = SDL_GetTicks();

    while (1) {
        /* How long has elapsed? */
        now = SDL_GetTicks();
        elapsed = now - start;

        /* If we've hit or exceeded the deadline, clamp to ms and exit. */
        if (elapsed >= ms) {
            elapsed = ms;
            break;
        }

        /* Check for user input without blocking */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    *direction = UP;    break;
                    case SDLK_RIGHT: *direction = RIGHT; break;
                    case SDLK_DOWN:  *direction = DOWN;  break;
                    case SDLK_LEFT:  *direction = LEFT;  break;
                }
            }
        }


        /* Briefly pause to avoid spinning the CPU at 100%. */
        SDL_Delay(1);
    }

    /* Compute how many ms remain */
    remaining = ms - elapsed;
    return (int)remaining;
}
