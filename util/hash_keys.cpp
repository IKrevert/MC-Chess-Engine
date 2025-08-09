#include <cstdio>
#include "../defs.h"
#include "setup.h"

U64 generatePositionKeys(const S_BOARD* board) {
    U64 finalKey = 0ULL;

    // Pieces
    for (int sq = 0; sq < BRD_SQ_NUM; ++sq) {
        int piece = board->pieces[sq];
        if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            ASSERT(piece >= wP && piece <= bK);
            finalKey ^= zobristPieceKeys[piece][sq];
        }
    }

    // Side to move
    if (board->side == WHITE) {
        finalKey ^= zobristSideKey;
    }

    // En passant square
    if (board->enPas != NO_SQ) {
        ASSERT(board->enPas >= 0 && board->enPas < BRD_SQ_NUM);
        ASSERT(SqOnBoard(board->enPas));
        ASSERT(rankIndex120[board->enPas] == RANK_3 || rankIndex120[board->enPas] == RANK_6);
        finalKey ^= zobristPieceKeys[EMPTY][board->enPas];
    }

    // Castling rights
    ASSERT(board->castlePerm >= 0 && board->castlePerm <= 15);
    finalKey ^= zobristCastleKeys[board->castlePerm];

    return finalKey;
}
