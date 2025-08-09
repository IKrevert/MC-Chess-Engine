#pragma once

#include "../defs.h"
#include "../test./validate.h"

/**
 * @brief Validate that piece list array is consistent with board state.
 * @param pos Pointer to board state.
 * @return TRUE if list is valid, FALSE otherwise.
 */
extern bool isPieceListValid(const S_BOARD *pos);

/**
 * @brief Performs a full internal consistency check of the board state.
 * @param pos Pointer to board state.
 * @return TRUE if board is valid, FALSE otherwise.
 */
extern bool isBoardStateValid(const S_BOARD *pos);

/**
 * @brief Update piece lists, material counts, and pawn bitboards from scratch.
 * @param pos Pointer to board state.
 */
extern void updateListsMaterial(S_BOARD *pos);

/**
 * @brief Parse a FEN string into the given board state.
 * @param fen FEN string.
 * @param pos Pointer to board state.
 * @return 0 on success, -1 on failure.
 */
extern int loadFEN(char *fen, S_BOARD *pos);

/**
 * @brief Reset board to empty state.
 * @param pos Pointer to board state.
 */
extern void resetBoardState(S_BOARD *pos);

/**
 * @brief Print the board to stdout in a human-readable format.
 * @param pos Pointer to board state.
 */
extern void printBoardState(const S_BOARD *pos);

/**
 * @brief Flip the board vertically, swapping colors and positions.
 * @param pos Pointer to board state.
 */
extern void mirrorBoardPosition(S_BOARD *pos);
