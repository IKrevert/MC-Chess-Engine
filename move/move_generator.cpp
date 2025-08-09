#include <iostream>
#include "move_generator.h"
#include "make_move.h"
#include "../util/attack.h"
#include "../util/setup.h"

const int LoopSlidePce[8]     = { wB, wR, wQ, 0, bB, bR, bQ, 0 };
const int LoopNonSlidePce[6]  = { wN, wK, 0, bN, bK, 0 };
const int LoopSlideIndex[2]   = { 0, 4 };
const int LoopNonSlideIndex[2]= { 0, 3 };

const int pieceDirection[13][8] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -8, -19, -21, -12,  8, 19, 21, 12 },
    { -9, -11,  11,   9,  0,  0,  0,  0 },
    { -1, -10,   1,  10,  0,  0,  0,  0 },
    { -1, -10,   1,  10, -9, -11, 11,  9 },
    { -1, -10,   1,  10, -9, -11, 11,  9 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -8, -19, -21, -12,  8, 19, 21, 12 },
    { -9, -11,  11,   9,  0,  0,  0,  0 },
    { -1, -10,   1,  10,  0,  0,  0,  0 },
    { -1, -10,   1,  10, -9, -11, 11,  9 },
    { -1, -10,   1,  10, -9, -11, 11,  9 }
};

const int numberDirections[13] = { 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8 };

const int VictimScore[13] = { 0,100,200,300,400,500,600,100,200,300,400,500,600 };
static int MvvLvaScores[13][13];

// ---- helpers (local linkage, C++-style params/locals) ----
static void AddQuietMove(const S_BOARD* board, int move, S_MOVELIST* moveList) {
    ASSERT(SqOnBoard(FROMSQ(move)));
    ASSERT(SqOnBoard(TOSQ(move)));
    ASSERT(isBoardStateValid(board));
    ASSERT(board->ply >= 0 && board->ply < MAXDEPTH);

    auto& slot = moveList->moves[moveList->count];
    slot.move = move;

    if (board->searchKillers[0][board->ply] == move) {
        slot.score = 900000;
    } else if (board->searchKillers[1][board->ply] == move) {
        slot.score = 800000;
    } else {
        slot.score = board->searchHistory[board->pieces[FROMSQ(move)]][TOSQ(move)];
    }
    moveList->count++;
}

static void AddCaptureMove(const S_BOARD* board, int move, S_MOVELIST* moveList) {
    ASSERT(SqOnBoard(FROMSQ(move)));
    ASSERT(SqOnBoard(TOSQ(move)));
    ASSERT(pieceValueid(CAPTURED(move)));
    ASSERT(isBoardStateValid(board));

    auto& slot = moveList->moves[moveList->count];
    slot.move  = move;
    slot.score = MvvLvaScores[CAPTURED(move)][board->pieces[FROMSQ(move)]] + 1000000;
    moveList->count++;
}

static void AddEnPassantMove(const S_BOARD* board, int move, S_MOVELIST* moveList) {
    ASSERT(SqOnBoard(FROMSQ(move)));
    ASSERT(SqOnBoard(TOSQ(move)));
    ASSERT(isBoardStateValid(board));
    ASSERT((rankIndex120[TOSQ(move)] == RANK_6 && board->side == WHITE) ||
           (rankIndex120[TOSQ(move)] == RANK_3 && board->side == BLACK));

    auto& slot = moveList->moves[moveList->count];
    slot.move  = move;
    slot.score = 105 + 1000000;
    moveList->count++;
}

static void AddWhitePawnCapMove(const S_BOARD* board, int fromSq, int toSq, int captured, S_MOVELIST* moveList) {
    ASSERT(pieceValueidEmpty(captured));
    ASSERT(SqOnBoard(fromSq));
    ASSERT(SqOnBoard(toSq));
    ASSERT(isBoardStateValid(board));

    if (rankIndex120[fromSq] == RANK_7) {
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, wQ, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, wR, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, wB, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, wN, 0), moveList);
    } else {
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, EMPTY, 0), moveList);
    }
}

static void AddWhitePawnMove(const S_BOARD* board, int fromSq, int toSq, S_MOVELIST* moveList) {
    ASSERT(SqOnBoard(fromSq));
    ASSERT(SqOnBoard(toSq));
    ASSERT(isBoardStateValid(board));

    if (rankIndex120[fromSq] == RANK_7) {
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, wQ, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, wR, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, wB, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, wN, 0), moveList);
    } else {
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, EMPTY, 0), moveList);
    }
}

static void AddBlackPawnCapMove(const S_BOARD* board, int fromSq, int toSq, int captured, S_MOVELIST* moveList) {
    ASSERT(pieceValueidEmpty(captured));
    ASSERT(SqOnBoard(fromSq));
    ASSERT(SqOnBoard(toSq));
    ASSERT(isBoardStateValid(board));

    if (rankIndex120[fromSq] == RANK_2) {
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, bQ, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, bR, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, bB, 0), moveList);
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, bN, 0), moveList);
    } else {
        AddCaptureMove(board, MOVE(fromSq, toSq, captured, EMPTY, 0), moveList);
    }
}

static void AddBlackPawnMove(const S_BOARD* board, int fromSq, int toSq, S_MOVELIST* moveList) {
    ASSERT(SqOnBoard(fromSq));
    ASSERT(SqOnBoard(toSq));
    ASSERT(isBoardStateValid(board));

    if (rankIndex120[fromSq] == RANK_2) {
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, bQ, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, bR, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, bB, 0), moveList);
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, bN, 0), moveList);
    } else {
        AddQuietMove(board, MOVE(fromSq, toSq, EMPTY, EMPTY, 0), moveList);
    }
}

// ---- public API (same behavior, C++ locals/loops/names) ----
void InitMvvLva() {
    for (int attacker = wP; attacker <= bK; ++attacker) {
        for (int victim = wP; victim <= bK; ++victim) {
            MvvLvaScores[victim][attacker] = VictimScore[victim] + 6 - (VictimScore[attacker] / 100);
        }
    }
}

int moveExists(S_BOARD* board, const int move) {
    S_MOVELIST moveList[1];
    generateAllMoves(board, moveList);

    for (int i = 0; i < moveList->count; ++i) {
        if (!makeMove(board, moveList->moves[i].move)) {
            continue;
        }
        takeMove(board);
        if (moveList->moves[i].move == move) {
            return TRUE;
        }
    }
    return FALSE;
}

void generateAllMoves(const S_BOARD* board, S_MOVELIST* moveList) {
    ASSERT(isBoardStateValid(board));
    moveList->count = 0;

    const int side = board->side;

    if (side == WHITE) {
        for (int pi = 0; pi < board->pceNum[wP]; ++pi) {
            const int fromSq = board->pList[wP][pi];
            ASSERT(SqOnBoard(fromSq));

            if (board->pieces[fromSq + 10] == EMPTY) {
                AddWhitePawnMove(board, fromSq, fromSq + 10, moveList);
                if (rankIndex120[fromSq] == RANK_2 && board->pieces[fromSq + 20] == EMPTY) {
                    AddQuietMove(board, MOVE(fromSq, fromSq + 20, EMPTY, EMPTY, MFLAGPS), moveList);
                }
            }
            if (!SQOFFBOARD(fromSq + 9) && pieceColor[board->pieces[fromSq + 9]] == BLACK) {
                AddWhitePawnCapMove(board, fromSq, fromSq + 9, board->pieces[fromSq + 9], moveList);
            }
            if (!SQOFFBOARD(fromSq + 11) && pieceColor[board->pieces[fromSq + 11]] == BLACK) {
                AddWhitePawnCapMove(board, fromSq, fromSq + 11, board->pieces[fromSq + 11], moveList);
            }

            if (board->enPas != NO_SQ) {
                if (fromSq + 9 == board->enPas)  AddEnPassantMove(board, MOVE(fromSq, fromSq + 9,  EMPTY, EMPTY, MFLAGEP), moveList);
                if (fromSq + 11 == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq + 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }

        if (board->castlePerm & WKCA) {
            if (board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY) {
                if (!isSquareAttacked(E1, BLACK, board) && !isSquareAttacked(F1, BLACK, board)) {
                    AddQuietMove(board, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), moveList);
                }
            }
        }
        if (board->castlePerm & WQCA) {
            if (board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[B1] == EMPTY) {
                if (!isSquareAttacked(E1, BLACK, board) && !isSquareAttacked(D1, BLACK, board)) {
                    AddQuietMove(board, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), moveList);
                }
            }
        }
    } else {
        for (int pi = 0; pi < board->pceNum[bP]; ++pi) {
            const int fromSq = board->pList[bP][pi];
            ASSERT(SqOnBoard(fromSq));

            if (board->pieces[fromSq - 10] == EMPTY) {
                AddBlackPawnMove(board, fromSq, fromSq - 10, moveList);
                if (rankIndex120[fromSq] == RANK_7 && board->pieces[fromSq - 20] == EMPTY) {
                    AddQuietMove(board, MOVE(fromSq, fromSq - 20, EMPTY, EMPTY, MFLAGPS), moveList);
                }
            }
            if (!SQOFFBOARD(fromSq - 9) && pieceColor[board->pieces[fromSq - 9]] == WHITE) {
                AddBlackPawnCapMove(board, fromSq, fromSq - 9, board->pieces[fromSq - 9], moveList);
            }
            if (!SQOFFBOARD(fromSq - 11) && pieceColor[board->pieces[fromSq - 11]] == WHITE) {
                AddBlackPawnCapMove(board, fromSq, fromSq - 11, board->pieces[fromSq - 11], moveList);
            }

            if (board->enPas != NO_SQ) {
                if (fromSq - 9 == board->enPas)  AddEnPassantMove(board, MOVE(fromSq, fromSq - 9,  EMPTY, EMPTY, MFLAGEP), moveList);
                if (fromSq - 11 == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq - 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }

        if (board->castlePerm & BKCA) {
            if (board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY) {
                if (!isSquareAttacked(E8, WHITE, board) && !isSquareAttacked(F8, WHITE, board)) {
                    AddQuietMove(board, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), moveList);
                }
            }
        }
        if (board->castlePerm & BQCA) {
            if (board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[B8] == EMPTY) {
                if (!isSquareAttacked(E8, WHITE, board) && !isSquareAttacked(D8, WHITE, board)) {
                    AddQuietMove(board, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), moveList);
                }
            }
        }
    }

    // sliding pieces
    for (int pIndex = LoopSlideIndex[side], piece = LoopSlidePce[pIndex++]; piece != 0; piece = LoopSlidePce[pIndex++]) {
        ASSERT(pieceValueid(piece));
        for (int pi = 0; pi < board->pceNum[piece]; ++pi) {
            const int fromSq = board->pList[piece][pi];
            ASSERT(SqOnBoard(fromSq));

            for (int d = 0; d < numberDirections[piece]; ++d) {
                const int delta = pieceDirection[piece][d];
                int targetSq = fromSq + delta;

                while (!SQOFFBOARD(targetSq)) {
                    if (board->pieces[targetSq] != EMPTY) {
                        if (pieceColor[board->pieces[targetSq]] == (side ^ 1)) {
                            AddCaptureMove(board, MOVE(fromSq, targetSq, board->pieces[targetSq], EMPTY, 0), moveList);
                        }
                        break;
                    }
                    AddQuietMove(board, MOVE(fromSq, targetSq, EMPTY, EMPTY, 0), moveList);
                    targetSq += delta;
                }
            }
        }
    }

    // non-sliding pieces
    for (int pIndex = LoopNonSlideIndex[side], piece = LoopNonSlidePce[pIndex++]; piece != 0; piece = LoopNonSlidePce[pIndex++]) {
        ASSERT(pieceValueid(piece));
        for (int pi = 0; pi < board->pceNum[piece]; ++pi) {
            const int fromSq = board->pList[piece][pi];
            ASSERT(SqOnBoard(fromSq));

            for (int d = 0; d < numberDirections[piece]; ++d) {
                const int targetSq = fromSq + pieceDirection[piece][d];
                if (SQOFFBOARD(targetSq)) continue;

                if (board->pieces[targetSq] != EMPTY) {
                    if (pieceColor[board->pieces[targetSq]] == (side ^ 1)) {
                        AddCaptureMove(board, MOVE(fromSq, targetSq, board->pieces[targetSq], EMPTY, 0), moveList);
                    }
                    continue;
                }
                AddQuietMove(board, MOVE(fromSq, targetSq, EMPTY, EMPTY, 0), moveList);
            }
        }
    }

    ASSERT(MoveListOk(moveList, board));
}

void generateAllCaptures(const S_BOARD* board, S_MOVELIST* moveList) {
    ASSERT(isBoardStateValid(board));
    moveList->count = 0;

    const int side = board->side;

    if (side == WHITE) {
        for (int pi = 0; pi < board->pceNum[wP]; ++pi) {
            const int fromSq = board->pList[wP][pi];
            ASSERT(SqOnBoard(fromSq));

            if (!SQOFFBOARD(fromSq + 9)  && pieceColor[board->pieces[fromSq + 9]]  == BLACK)
                AddWhitePawnCapMove(board, fromSq, fromSq + 9,  board->pieces[fromSq + 9],  moveList);
            if (!SQOFFBOARD(fromSq + 11) && pieceColor[board->pieces[fromSq + 11]] == BLACK)
                AddWhitePawnCapMove(board, fromSq, fromSq + 11, board->pieces[fromSq + 11], moveList);

            if (board->enPas != NO_SQ) {
                if (fromSq + 9  == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq + 9,  EMPTY, EMPTY, MFLAGEP), moveList);
                if (fromSq + 11 == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq + 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }
    } else {
        for (int pi = 0; pi < board->pceNum[bP]; ++pi) {
            const int fromSq = board->pList[bP][pi];
            ASSERT(SqOnBoard(fromSq));

            if (!SQOFFBOARD(fromSq - 9)  && pieceColor[board->pieces[fromSq - 9]]  == WHITE)
                AddBlackPawnCapMove(board, fromSq, fromSq - 9,  board->pieces[fromSq - 9],  moveList);
            if (!SQOFFBOARD(fromSq - 11) && pieceColor[board->pieces[fromSq - 11]] == WHITE)
                AddBlackPawnCapMove(board, fromSq, fromSq - 11, board->pieces[fromSq - 11], moveList);

            if (board->enPas != NO_SQ) {
                if (fromSq - 9  == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq - 9,  EMPTY, EMPTY, MFLAGEP), moveList);
                if (fromSq - 11 == board->enPas) AddEnPassantMove(board, MOVE(fromSq, fromSq - 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }
    }

    // sliding captures only
    for (int pIndex = LoopSlideIndex[side], piece = LoopSlidePce[pIndex++]; piece != 0; piece = LoopSlidePce[pIndex++]) {
        ASSERT(pieceValueid(piece));
        for (int pi = 0; pi < board->pceNum[piece]; ++pi) {
            const int fromSq = board->pList[piece][pi];
            ASSERT(SqOnBoard(fromSq));

            for (int d = 0; d < numberDirections[piece]; ++d) {
                const int delta = pieceDirection[piece][d];
                int targetSq = fromSq + delta;

                while (!SQOFFBOARD(targetSq)) {
                    if (board->pieces[targetSq] != EMPTY) {
                        if (pieceColor[board->pieces[targetSq]] == (side ^ 1)) {
                            AddCaptureMove(board, MOVE(fromSq, targetSq, board->pieces[targetSq], EMPTY, 0), moveList);
                        }
                        break;
                    }
                    targetSq += delta;
                }
            }
        }
    }

    // non-sliding captures only
    for (int pIndex = LoopNonSlideIndex[side], piece = LoopNonSlidePce[pIndex++]; piece != 0; piece = LoopNonSlidePce[pIndex++]) {
        ASSERT(pieceValueid(piece));
        for (int pi = 0; pi < board->pceNum[piece]; ++pi) {
            const int fromSq = board->pList[piece][pi];
            ASSERT(SqOnBoard(fromSq));

            for (int d = 0; d < numberDirections[piece]; ++d) {
                const int targetSq = fromSq + pieceDirection[piece][d];
                if (SQOFFBOARD(targetSq)) continue;

                if (board->pieces[targetSq] != EMPTY &&
                    pieceColor[board->pieces[targetSq]] == (side ^ 1)) {
                    AddCaptureMove(board, MOVE(fromSq, targetSq, board->pieces[targetSq], EMPTY, 0), moveList);
                }
            }
        }
    }

    ASSERT(MoveListOk(moveList, board));
}
