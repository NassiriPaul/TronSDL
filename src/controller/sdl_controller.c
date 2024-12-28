#include <SDL2/SDL.h>
#include "types.h"

int getInput(int* input, unsigned long ms)
{
    SDL_Event event; /* to store the user input , then will be cast in input*/
    unsigned long start, now, elapsed, remaining;

    start = SDL_GetTicks();

    while (1) {
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
                    case SDLK_UP:    *input = UP;    break;
                    case SDLK_RIGHT: *input = RIGHT; break;
                    case SDLK_DOWN:  *input = DOWN;  break;
                    case SDLK_LEFT:  *input = LEFT;  break;
                    case SDLK_t:  *input = 5;  break;
                }
            }
            if (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE) *input = 6;
            
        }

        /* Briefly pause to avoid spinning the CPU at 100%. */
        SDL_Delay(1);
    }

    /* constraint the time */
    remaining = (ms - elapsed);
    if (remaining < 0) {
        remaining = 0; /* Ensure no negative remaining time */
    }

    return (int)remaining;
}
