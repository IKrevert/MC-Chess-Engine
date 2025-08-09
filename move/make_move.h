#pragma once

#include "../defs.h"  


/** @brief Castling permission mask by board square (mailbox 120). */
extern const int CastlePerm[120];

/** @brief Make a move; returns non-zero if legal (king not left in check). */
extern int makeMove(S_BOARD *pos, int move);

/** @brief Undo the last made move. */
extern void takeMove(S_BOARD *pos);

/** @brief Make a null move (side to move toggles, no pieces moved). */
extern void makeNullMove(S_BOARD *pos);

/** @brief Undo the last null move. */
extern void takeNullMove(S_BOARD *pos);
