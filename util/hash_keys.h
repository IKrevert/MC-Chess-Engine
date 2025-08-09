#pragma once

#include "../defs.h"
#include "setup.h"

/**
 * @brief Generates a Zobrist hash key for the given board position.
 * 
 * The key uniquely (with very high probability) represents:
 * - Pieces on all squares
 * - Side to move
 * - Castling rights
 * - En passant square (if any)
 * 
 * @param pos Pointer to the current board state.
 * @return 64-bit Zobrist position key.
 */
extern U64 generatePositionKeys(const S_BOARD* pos);

