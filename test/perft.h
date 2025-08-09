#pragma once

#include "../defs.h"
#include "../board/board.h"

/**
 * @brief Runs a perft search from a given board state.
 * 
 * @param depth The depth to search.
 * @param pos Pointer to the board state.
 */
extern void Perft(int depth, S_BOARD* pos);

/**
 * @brief Executes a perft test and prints move breakdown and timing.
 * 
 * @param depth The depth to search.
 * @param pos Pointer to the board state.
 */
extern void PerftTest(int depth, S_BOARD* pos);

/**
 * @brief Global counter for leaf nodes visited during perft.
 */
extern long leafNodes;
