#pragma once
#include "model.h"

/**
 * @brief Init the UI (ncurses or SDL)
 * Will do some stuff like removing the cursor, cleaning the screen , check if all graphical component is well installed
 */
void viewInit();

/**
 * @brief Show the menu ( before the first round )
 * Will show the keys and wait for a key being press
 * @return an integer for the portability with SDL
 */
int viewMenu(int n_lines, int n_columns);

/**
 * @brief Set the UI to what it should look like before a round start and wait for a direction to start the round
 *
 * @param grid Pointer to the Grid
 * @param scorePlayer Score of the player
 * @param scoreBot    Score of the bot
 */
void viewStart(Grid *grid, int scorePlayer, int scoreBot);

/**
 * @brief Update the screen to the movement that just happen
 *  will look in the grid the new position of player and bot and the new dot they add in their path
 * @param grid Pointer to the Grid
 */
void viewUpdate(Grid *grid);

/**
 * @brief Show how many turbos the player have left
 *
 * @param n_lines Number of lines in the grid. it will be shown after the n_lines
 * @param turbos Number of turbos remaining for a player.
 */
void updateViewTurbos(int n_lines, int turbos);

/**
 * @brief Show how many points the player and the bot have.
 *
 * @param n_lines Number of lines in the grid (used for positioning).
 * @param scorePlayer Current score of the player.
 * @param scoreBot Current score of the bot.
 */
void updateViewScore(int n_lines, int scorePlayer, int scoreBot);

/**
 * @brief Clean only the graphical stuff
 */
void viewCleanup();
