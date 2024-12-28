#pragma once
#include "model.h"

/**
 * @brief Launch the game
 * 
 * @param n_lines number of lines in the grid
 * @param n_columns number of colunms in the grid
 *
 * @return the winner. 1 for the player and 0 for the bot
 */
int startGame(int n_lines, int n_columns);

/**
 * @brief Get the player's input within a specified timeout (in ms).
 *
 * inputs are casted as follow : 0 top, 1 right, 2 down, 3 left, 5 turbos and 6 quit
 * @param input A pointer to stock the input of the player ( turbo, quit or movement )
 * @param ms The player have ms milliseconds to send an input; else nothing new is stored in ms
 *
 * @return if the user sent an input before the timeout, we return the time he had left in ms
 */
int getInput(int* input, unsigned long ms);
