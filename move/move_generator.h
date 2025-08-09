// movegen.h
#pragma once

#include "../defs.h"  // S_BOARD, S_MOVELIST, enums, etc.

/** @brief Macro to encode a move into an integer. */
#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl) )

/** @brief Macro to check if a square index is off the board. */
#define SQOFFBOARD(sq) (fileIndex120[(sq)] == OFFBOARD)

/** @brief Sliding piece loop order for move generation. */
extern const int LoopSlidePce[8];

/** @brief Non-sliding piece loop order for move generation. */
extern const int LoopNonSlidePce[6];

/** @brief Indices into LoopSlidePce for white and black. */
extern const int LoopSlideIndex[2];

/** @brief Indices into LoopNonSlidePce for white and black. */
extern const int LoopNonSlideIndex[2];

/** @brief Direction offsets for each piece type. */
extern const int pieceDirection[13][8];

/** @brief Number of valid directions for each piece type. */
extern const int numberDirections[13];

/** @brief Victim scoring table for MVV-LVA. */
extern const int VictimScore[13];

/** @brief Most Valuable Victim / Least Valuable Attacker scores. */
extern int MvvLvaScores[13][13];

/**
 * @brief Initialize the MVV-LVA capture scoring table.
 * @note Call once at engine startup before generating moves.
 */
extern void InitMvvLva(void);

/**
 * @brief Generate all pseudo-legal moves for the side to move.
 * @param pos  Pointer to the current board position.
 * @param list Pointer to the move list to be filled.
 */
extern void generateAllMoves(const S_BOARD *pos, S_MOVELIST *list);

/**
 * @brief Generate all pseudo-legal capture moves (including en passant).
 * @param pos  Pointer to the current board position.
 * @param list Pointer to the move list to be filled.
 */
extern void generateAllCaptures(const S_BOARD *pos, S_MOVELIST *list);

/**
 * @brief Check if a given move exists in the current position.
 * @param pos  Pointer to the current board position.
 * @param move Encoded move integer.
 * @return Non-zero if the move exists; zero otherwise.
 */
extern int moveExists(S_BOARD *pos, int move);
