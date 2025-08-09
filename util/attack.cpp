#include <cstdio>
#include "defs.h"

const int knightDirection[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int rookDirection[4]   = { -1, -10, 1, 10 };
const int bishopDirection[4] = { -9, -11, 11, 9 };
const int kingDirection[8]   = { -1, -10, 1, 10, -9, -11, 11, 9 };

bool isSquareAttacked(const int square, const int side, const S_BOARD* pos) {

    ASSERT(SqOnBoard(square));
    ASSERT(SideValid(side));
    ASSERT(isBoardStateValid(pos));

    // pawns
    if (side == WHITE) {
        if (pos->pieces[square - 11] == wP || pos->pieces[square - 9] == wP) {
            return true;
        }
    } else {
        if (pos->pieces[square + 11] == bP || pos->pieces[square + 9] == bP) {
            return true;
        }
    }

    // knights
    for (int i = 0; i < 8; ++i) {
        int piece = pos->pieces[square + knightDirection[i]];
        ASSERT(PceValidEmptyOffbrd(piece));
        if (piece != OFFBOARD && IsKn(piece) && pieceColor[piece] == side) {
            return true;
        }
    }

    // rooks, queens
    for (int i = 0; i < 4; ++i) {
        int dir = rookDirection[i];
        int targetSq = square + dir;
        ASSERT(SqIs120(targetSq));
        int piece = pos->pieces[targetSq];
        ASSERT(PceValidEmptyOffbrd(piece));
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (IsRQ(piece) && pieceColor[piece] == side) {
                    return true;
                }
                break;
            }
            targetSq += dir;
            ASSERT(SqIs120(targetSq));
            piece = pos->pieces[targetSq];
        }
    }

    // bishops, queens
    for (int i = 0; i < 4; ++i) {
        int dir = bishopDirection[i];
        int targetSq = square + dir;
        ASSERT(SqIs120(targetSq));
        int piece = pos->pieces[targetSq];
        ASSERT(PceValidEmptyOffbrd(piece));
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (IsBQ(piece) && pieceColor[piece] == side) {
                    return true;
                }
                break;
            }
            targetSq += dir;
            ASSERT(SqIs120(targetSq));
            piece = pos->pieces[targetSq];
        }
    }

    // kings
    for (int i = 0; i < 8; ++i) {
        int piece = pos->pieces[square + kingDirection[i]];
        ASSERT(PceValidEmptyOffbrd(piece));
        if (piece != OFFBOARD && IsKi(piece) && pieceColor[piece] == side) {
            return true;
        }
    }

    return false;
}
