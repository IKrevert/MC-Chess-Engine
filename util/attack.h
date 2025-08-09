#pragma once
#include "../defs.h"

/**
 * @brief Knight movement directions in 120-square board representation.
 */
extern const int knightDirection[8];

/**
 * @brief Rook movement directions in 120-square board representation.
 */
extern const int rookDirection[4];

/**
 * @brief Bishop movement directions in 120-square board representation.
 */
extern const int bishopDirection[4];

/**
 * @brief King movement directions in 120-square board representation.
 */
extern const int kingDirection[8];

/**
 * @brief Determines if a given square is attacked by a given side.
 * 
 * @param sq Square index (120-based representation).
 * @param side Side to check for attacks (WHITE or BLACK).
 * @param pos Pointer to the current board state.
 * @return true if the square is attacked by the given side, false otherwise.
 */
extern bool isSquareAttacked(int sq, int side, const S_BOARD* pos);
