// validate.cpp
#include <cstdio>
#include "defs.h"
#include "util/setup.h"
#include "validate.h"

int MoveListOk(const S_MOVELIST* moves, const S_BOARD* board) {
    if (moves->count < 0 || moves->count >= MAXPOSITIONMOVES) return FALSE;

    for (int i = 0; i < moves->count; ++i) {
        const int m    = moves->moves[i].move;
        const int from = FROMSQ(m);
        const int to   = TOSQ(m);

        if (!SqOnBoard(from) || !SqOnBoard(to)) return FALSE;
        if (!pieceValueidEmpty(board->pieces[from])) return FALSE;
    }
    return TRUE;
}

int SqIs120(const int sq) {
    return (sq >= 0 && sq < BRD_SQ_NUM);
}

int PceValidEmptyOffbrd(const int pce) {
    return (pce == OFFBOARD) || (pce >= EMPTY && pce <= bK);
}

int SqOnBoard(const int sq) {
    // fileIndex120[sq] is OFFBOARD for the 120-mailbox border
    return (sq >= 0 && sq < BRD_SQ_NUM) && (fileIndex120[sq] != OFFBOARD);
}

int SideValid(const int side) {
    return (side == WHITE || side == BLACK);
}

int FileRankValid(const int fr) {
    return (fr >= 0 && fr <= 7);
}

int pieceValueidEmpty(const int pce) {
    return (pce >= EMPTY && pce <= bK);
}

int pieceValueid(const int pce) {
    return (pce >= wP && pce <= bK);
}

