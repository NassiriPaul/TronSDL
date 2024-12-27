#include <SDL2/SDL.h>
#include "types.h"

/**
 * Get the player's direction within a specified timeout (in tenths of a second).
 * @param direction Pointer to the current direction of the player.
 * @param timeout Timeout in tenths of a second to capture input.
 * @return Remaining time in tenths of a second after user input or timeout.
 */
int getInput(int* direction, unsigned long ms)
{
    SDL_Event event;
    unsigned long start, now, elapsed, remaining;

    /* Record the start time in milliseconds */
    start = SDL_GetTicks();

    while (1) {
        /* How long has elapsed? */
        now = SDL_GetTicks();
        elapsed = now - start;

        /* If we've hit or exceeded the timeout, clamp to timeout_ms and exit. */
        if (elapsed >= ms) {
            elapsed = ms;
            break;
        }

        /* Check for user input without blocking */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    *direction = UP;    break;
                    case SDLK_RIGHT: *direction = RIGHT; break;
                    case SDLK_DOWN:  *direction = DOWN;  break;
                    case SDLK_LEFT:  *direction = LEFT;  break;
                    case SDLK_t:  *direction = 5;  break;
                }
            }
            if (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE) *direction = 6;
            
        }

        /* Briefly pause to avoid spinning the CPU at 100%. */
        SDL_Delay(1);
    }

    /* Compute how many tenths of a second remain */
    remaining = (ms - elapsed);
    if (remaining < 0) {
        remaining = 0; /* Ensure no negative remaining time */
    }

    return (int)remaining;
}
