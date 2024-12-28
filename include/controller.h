#pragma once
#include "model.h"

/**
 * @brief Launch the game
 * 
 * @param n_lines number of lines in the grid
 * @param n_columns number of colunms in the grid
 * @return the winner. 1 for the player and 0 for the bot
 */
int startGame(int n_lines, int n_columns);

/**
 * @brief Get the player's direction within a specified timeout (in tenths of a second).
 * 
 * @param direction Pointer to the current direction of the player.
 * @param timeout Timeout in tenths of a second to capture input.
 * @return Remaining time in tenths of a second after user input or timeout.
 */
int getInput(int* input, unsigned long ms);
