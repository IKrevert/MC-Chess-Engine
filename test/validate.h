// validate.h
#pragma once

#include "../defs.h"

/**
 * @brief Checks if a given square index refers to a valid on-board square.
 * @param sq The 120-based square index.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int SqOnBoard(const int sq);

/**
 * @brief Checks if the given side identifier is valid.
 * @param side Side value to check (WHITE or BLACK).
 * @return Non-zero if valid, 0 otherwise.
 */
extern int SideValid(const int side);

/**
 * @brief Checks if a file or rank index is valid (0–7).
 * @param fr The file or rank index.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int FileRankValid(const int fr);

/**
 * @brief Checks if the given piece code is either empty or a valid piece.
 * @param pce Piece code to check.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int pieceValueidEmpty(const int pce);

/**
 * @brief Checks if the given piece code is a valid piece (non-empty).
 * @param pce Piece code to check.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int pieceValueid(const int pce);

/**
 * @brief Checks if a given square index is within the 120-based range.
 * @param sq The square index to check.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int SqIs120(const int sq);

/**
 * @brief Checks if the given piece code is empty, valid, or OFFBOARD.
 * @param pce Piece code to check.
 * @return Non-zero if valid, 0 otherwise.
 */
extern int PceValidEmptyOffbrd(const int pce);

/**
 * @brief Validates that a move list is within bounds and references valid moves.
 * @param list Pointer to the move list.
 * @param pos Pointer to the board state.
 * @return Non-zero if the move list is valid, 0 otherwise.
 */
extern int MoveListOk(const S_MOVELIST* list, const S_BOARD* pos);

/**
 * @brief Mirrors the board position and evaluates it (used for testing).
 * @param pos Pointer to the board state.
 */
extern void MirrorEvalTest(S_BOARD* pos);

/**
 * @brief Runs a debug analysis test (used for search engine testing).
 * @param pos Pointer to the board state.
 * @param info Pointer to search information.
 */
extern void DebugAnalysisTest(S_BOARD* pos, S_SEARCHINFO* info);
