#pragma once
#include <cstddef>
#include "../defs.h"
#include "../board/board.h"

/**
 * @brief Parse "e2" style coordinate into a 120-based square index.
 * @return FR2SQ(file,rank) on success; -1 on bad input.
 */
int ParseSquare2(const char* s);

/**
 * @brief Map promotion char ('q','r','b','n') to a piece code for the given side.
 *        Case-insensitive. Returns EMPTY if none/unknown.
 */
int ParsePromotionPiece(char c, int side);

/**
 * @brief Build list of legal moves that originate from `from`.
 * @param pos Board state.
 * @param from 120-based from-square.
 * @param outMoves Output array of encoded moves.
 * @param labels Output labels (e.g., "e4", "e8=Q"), each [8].
 * @param maxOut Capacity of outMoves/labels.
 * @return Count of legal moves written (<= maxOut).
 */
int LegalMovesFrom(S_BOARD* pos, int from,
                   int outMoves[], char labels[][8], int maxOut);

/**
 * @brief Resolve a destination string ("e4", "e8=Q", "e8q") to a move.
 * @return Encoded move or NOMOVE if none matches.
 */
int MatchDestinationMove(S_BOARD* pos, int from,
                         const char* destStr, const int legalMoves[], int nMoves);
