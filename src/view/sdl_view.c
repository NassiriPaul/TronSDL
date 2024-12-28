/*
   sdl_view.c

   Minimal SDL-based view that parallels your ncurses_view.c logic.
   You can adapt cell size, colors, border drawing, etc.
*/

#include <SDL2/SDL.h>
#include <stdio.h>       /* for printf in updateViewScore() */
#include "../../include/view.h"  /* for Grid, Dot, etc. */
#include <SDL2/SDL_ttf.h>

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font;
static int CELL_SIZE;

/* Initialize SDL */
void viewInit()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
        /* handle error, or exit */
        return;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    }

    window = SDL_CreateWindow("Tron SDL",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    /* Clear screen to black initially */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


int viewMenu(int n_lines, int n_columns){
    const char *lines[7];
    char buffer[64];
    int i;
    SDL_Color white;
    SDL_Color cyan;
    SDL_Color red;
    TTF_Font *font;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect destRect;
    SDL_Rect clearRect; 
    SDL_Event event;

    /* Choose a font color (white) */
    white.r = 255;
    white.g = 255;
    white.b = 255;
    white.a = 255;

    cyan.r = 43;
    cyan.g = 250;
    cyan.b = 250;
    cyan.a = 255;

    red.r = 255;
    red.g = 0;
    red.b = 0;
    red.a = 255;

    /* Load the font */
    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
        return 1;
    }

    /* lignes to print */
    lines[0] = "Welcome to the Game Arena !";
    lines[1] = "Directional keys :";
    lines[2] = "UP : up arrow";
    lines[3] ="RIGHT : right arrow";
    lines[4] = "LEFT : left arrow";
    lines[5] = "DOWN : down arrow";
    lines[6] = "Press any key to continue";

    /* Print the text on the SDL surface */
    /* for all elements in the array */
    for (i = 0; i < 7; i++) {
        snprintf(buffer, sizeof(buffer), "%s", lines[i]);
        if (i == 0) textSurface = TTF_RenderText_Blended(font, buffer, cyan);
        else if (i == 6) textSurface = TTF_RenderText_Blended(font, buffer, red);
        else textSurface = TTF_RenderText_Blended(font, buffer, white);
        
        if (!textSurface) {
            fprintf(stderr, "TTF_RenderText_Blended error: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            return 1;
        }

        /* Convert the surface in texture */
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            fprintf(stderr, "SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            TTF_CloseFont(font);
            return 1;
        }

        /* Define the text position */
        destRect.x = 50;
        if (i == 0) destRect.y = 30;
        else if (i == 6) destRect.y = 30 + 8.5 * 50;
        else destRect.y = 30 + (1+i) * 50; 
        destRect.w = textSurface->w;
        destRect.h = textSurface->h;


        /* Copy the texture in the renderer  */
        SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

        /* Destroy the texture when printed */
        SDL_DestroyTexture(textTexture);

        
    }

    /* Mettre à jour le rendu */
    SDL_RenderPresent(renderer);
    
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYUP) return 0;
        if (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE) return 1;
    }

    return 0;

}




/* Draws the initial grid border and draws the player/bot in their starting positions */
void viewStart(Grid *grid, int scorePlayer, int scoreBot)
{
    SDL_Event event;
    SDL_Rect rect;
    CELL_SIZE = 800/grid->n_columns;
    SDL_SetWindowSize(window, grid->n_columns*CELL_SIZE, (grid->n_lines*CELL_SIZE+24*4));

    /* Clear to black */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Draw a white border around the playing area */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /* Top line */
    SDL_RenderDrawLine(renderer,
        0, 0,
        grid->n_columns * CELL_SIZE, 0
    );
    /* Bottom line */
    SDL_RenderDrawLine(renderer,
        0, grid->n_lines * CELL_SIZE,
        grid->n_columns * CELL_SIZE, grid->n_lines * CELL_SIZE
    );
    /* Left line */
    SDL_RenderDrawLine(renderer,
        0, 0,
        0, grid->n_lines * CELL_SIZE
    );
    /* Right line */
    SDL_RenderDrawLine(renderer,
        grid->n_columns * CELL_SIZE, 0,
        grid->n_columns * CELL_SIZE, grid->n_lines * CELL_SIZE
    );

    /* Draw player as a green square */
    if (grid->player) {
        rect.x = grid->player->pos_x * CELL_SIZE;
        rect.y = grid->player->pos_y * CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  /* green */
        SDL_RenderFillRect(renderer, &rect);
    }

    /* Draw bot as a red square */
    if (grid->bot) {
        SDL_Rect rect;
        rect.x = grid->bot->pos_x * CELL_SIZE;
        rect.y = grid->bot->pos_y * CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  /* red */
        SDL_RenderFillRect(renderer, &rect);
    }
    updateViewTurbos(grid->n_lines, grid->player->turbos);
    updateViewScore(grid->n_lines, scorePlayer, scoreBot);
    SDL_RenderPresent(renderer);
    
    
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYUP) {
            if (SDLK_UP == event.key.keysym.sym) {grid->player->direction = UP;    break;}
            if (SDLK_RIGHT == event.key.keysym.sym) {grid->player->direction = RIGHT;    break;}
            if (SDLK_LEFT == event.key.keysym.sym) {grid->player->direction = LEFT;    break;}
            if (SDLK_DOWN == event.key.keysym.sym) {grid->player->direction = DOWN;    break;}
        }
        if (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE) grid->player->direction = 5;
    }
}

/* Draws the newest positions of player and bot, plus their routes. */
void viewUpdate(Grid *grid)
{
    Dot *d;

    /* Draw the player's latest route cell (if any) in a darker green */
    d = grid->playerRoute;
    if (d) {
        SDL_Rect r;
        r.x = d->pos_x * CELL_SIZE;
        r.y = d->pos_y * CELL_SIZE;
        r.w = CELL_SIZE;
        r.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 255);
        SDL_RenderFillRect(renderer, &r);
    }

    /* Draw the bot's latest route cell in a darker red */
    d = grid->botRoute;
    if (d) {
        SDL_Rect r;
        r.x = d->pos_x * CELL_SIZE;
        r.y = d->pos_y * CELL_SIZE;
        r.w = CELL_SIZE;
        r.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
        SDL_RenderFillRect(renderer, &r);
    }

    /* Player's new position in bright green */
    if (grid->player) {
        SDL_Rect rect;
        rect.x = grid->player->pos_x * CELL_SIZE;
        rect.y = grid->player->pos_y * CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    /* Bot's new position in bright red */
    if (grid->bot) {
        SDL_Rect rect;
        rect.x = grid->bot->pos_x * CELL_SIZE;
        rect.y = grid->bot->pos_y * CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.h = CELL_SIZE;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

void updateViewTurbos(int n_lines, int turbos) 
{
    char buffer[64];
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect destRect;
    SDL_Rect clearRect; 

    snprintf(buffer, sizeof(buffer), "Turbos : %d", turbos);

    /* Choisir une couleur de texte (blanc) */
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    /* Charger la police */
    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
        return;
    }
    
    /* Rendre le texte dans une surface SDL */
    textSurface = TTF_RenderText_Blended(font, buffer, color);
    if (!textSurface) {
        fprintf(stderr, "TTF_RenderText_Blended error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    /* Convertir la surface en texture */
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    /* Définir où afficher le texte */
    destRect.x = 10;
    destRect.y = n_lines * CELL_SIZE + 10;
    destRect.w = textSurface->w;
    destRect.h = textSurface->h;

    /* Définir un rectangle d'effacement plus grand que l'ancien texte */
    clearRect.x = destRect.x - 5; 
    clearRect.y = destRect.y - 5;
    clearRect.w = textSurface->w+50;
    clearRect.h = textSurface->h+20;

    /* Effacer la zone où le texte sera rendu */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &clearRect);

    /* Libérer la surface */
    SDL_FreeSurface(textSurface);

    /* Copier la texture dans le renderer */
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    /* Détruire la texture une fois dessinée */
    SDL_DestroyTexture(textTexture);

    /* Libérer la police */
    TTF_CloseFont(font);
}


void updateViewScore(int n_lines, int scorePlayer, int scoreBot)
{
    /* Create a text buffer like "Player: 5 | Bot: 2" */
    char buffer[64];
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect destRect;


    snprintf(buffer, sizeof(buffer), "Player: %d - Bot: %d", scorePlayer, scoreBot);

    /* Choose a text color (white) */
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    /* Render text to an SDL_Surface (using SDL_ttf) */
    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
    }
    
    textSurface = TTF_RenderText_Blended(font, buffer, color);
    if (!textSurface) {
        fprintf(stderr, "TTF_RenderText_Blended error: %s\n", TTF_GetError());
        return;
    }

    /* Convert that surface to an SDL_Texture */
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    /* Decide where on-screen to render. For example, just below the grid:
       x = 10, y = n_lines*CELL_SIZE + 10
    */
    
    destRect.x = 10;
    destRect.y = (n_lines+3) * CELL_SIZE;
    destRect.w = textSurface->w;  /* The width of the text */
    destRect.h = textSurface->h;  /* The height of the text */

    /* Clean up the surface, since we now have a texture */
    SDL_FreeSurface(textSurface);

    /* Copy (draw) the texture onto the renderer at destRect */
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    /* Destroy the texture once we're done drawing */
    SDL_DestroyTexture(textTexture);

    /* If you want it immediately visible, call SDL_RenderPresent(renderer);
       But usually you do that once at the end of your frame update.
    */
}
/* Cleanup SDL resources */
void viewCleanup()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}
