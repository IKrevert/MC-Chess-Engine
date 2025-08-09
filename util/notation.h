// notation.h
#pragma once

#include "../defs.h"  // For S_BOARD, S_MOVELIST, NOMOVE, etc.

/**
 * @brief Convert a board square index to algebraic notation.
 * @param sq Board square index (0–119 in mailbox 120 format).
 * @return Pointer to a static string containing the square (e.g., "e4").
 * @note This uses a static buffer and is not thread-safe.
 */
extern char* squareToString(int sq);

/**
 * @brief Convert an encoded move to coordinate notation.
 * @param move Encoded move integer.
 * @return Pointer to a static string containing the move (e.g., "e2e4", "e7e8q").
 * @note This uses a static buffer and is not thread-safe.
 */
extern char* moveToString(int move);

/**
 * @brief Parse a move from coordinate notation.
 * @param ptrChar Pointer to a string containing the move (e.g., "e2e4", "e7e8q").
 * @param pos Pointer to the current board state.
 * @return Encoded move integer, or NOMOVE if invalid.
 */
extern int parseMoveString(char* ptrChar, S_BOARD* pos);

/**
 * @brief Print all moves in a move list to stdout.
 * @param list Pointer to the move list to print.
 */
extern void printMoveList(const S_MOVELIST* list);
