// board.c

#include <iostream>

//local
#include "board.h"
#include "board_data.h"
#include "../util/bitboard.h"
#include "../util/setup.h"
#include "../util/hash_keys.h"

bool isPieceListValid(const S_BOARD* pos) {
    for (int piece = wP; piece <= bK; ++piece) {
        if (pos->pceNum[piece] < 0 || pos->pceNum[piece] >= 10) return FALSE;
    }

    if (pos->pceNum[wK] != 1 || pos->pceNum[bK] != 1) return FALSE;

    for (int piece = wP; piece <= bK; ++piece) {
        for (int count = 0; count < pos->pceNum[piece]; ++count) {
            const int square = pos->pList[piece][count];
            if (!SqOnBoard(square)) return FALSE;
        }
    }
	return true ;
}

bool isBoardStateValid(const S_BOARD* pos) {
    int tempPceNum[13]   = {0};
    int tempBigPce[2]    = {0};
    int tempMajPce[2]    = {0};
    int tempMinPce[2]    = {0};
    int tempMaterial[2]  = {0};

    U64 tempPawns[3] = {0ULL, 0ULL, 0ULL};
    tempPawns[WHITE] = pos->pawns[WHITE];
    tempPawns[BLACK] = pos->pawns[BLACK];
    tempPawns[BOTH]  = pos->pawns[BOTH];

    // check piece lists
    for (int piece = wP; piece <= bK; ++piece) {
        for (int idx = 0; idx < pos->pceNum[piece]; ++idx) {
            const int sq120 = pos->pList[piece][idx];
            ASSERT(pos->pieces[sq120] == piece);
        }
    }

    // check piece count and other counters
    for (int sq64 = 0; sq64 < 64; ++sq64) {
        const int sq120  = SQ120(sq64);
        const int piece  = pos->pieces[sq120];
        tempPceNum[piece]++;

        const int color = pieceColor[piece];
        if (isBigPiece[piece]   == TRUE) tempBigPce[color]++;
        if (isMinorPiece[piece] == TRUE) tempMinPce[color]++;
        if (isMajorPiece[piece] == TRUE) tempMajPce[color]++;

        tempMaterial[color] += pieceValue[piece];
    }

    for (int piece = wP; piece <= bK; ++piece) {
        ASSERT(tempPceNum[piece] == pos->pceNum[piece]);
    }

    // check bitboards count
    int popCount = CNT(tempPawns[WHITE]);
    ASSERT(popCount == pos->pceNum[wP]);
    popCount = CNT(tempPawns[BLACK]);
    ASSERT(popCount == pos->pceNum[bP]);
    popCount = CNT(tempPawns[BOTH]);
    ASSERT(popCount == (pos->pceNum[bP] + pos->pceNum[wP]));

    // check bitboards squares
    while (tempPawns[WHITE]) {
        const int sq64 = POP(&tempPawns[WHITE]);
        ASSERT(pos->pieces[SQ120(sq64)] == wP);
    }

    while (tempPawns[BLACK]) {
        const int sq64 = POP(&tempPawns[BLACK]);
        ASSERT(pos->pieces[SQ120(sq64)] == bP);
    }

    while (tempPawns[BOTH]) {
        const int sq64 = POP(&tempPawns[BOTH]);
        const int p    = pos->pieces[SQ120(sq64)];
        ASSERT(p == bP || p == wP);
    }

    ASSERT(tempMaterial[WHITE] == pos->material[WHITE] && tempMaterial[BLACK] == pos->material[BLACK]);
    ASSERT(tempMinPce[WHITE]  == pos->minPce[WHITE]   && tempMinPce[BLACK]   == pos->minPce[BLACK]);
    ASSERT(tempMajPce[WHITE]  == pos->majPce[WHITE]   && tempMajPce[BLACK]   == pos->majPce[BLACK]);
    ASSERT(tempBigPce[WHITE]  == pos->bigPce[WHITE]   && tempBigPce[BLACK]   == pos->bigPce[BLACK]);

    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(generatePositionKeys(pos) == pos->posKey);

    ASSERT(pos->enPas == NO_SQ ||
           (rankIndex120[pos->enPas] == RANK_6 && pos->side == WHITE) ||
           (rankIndex120[pos->enPas] == RANK_3 && pos->side == BLACK));

    ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
    ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    ASSERT(isPieceListValid(pos));
    return true;
}

void updateListsMaterial(S_BOARD* pos) {
    for (int index = 0; index < BRD_SQ_NUM; ++index) {
        const int square = index;
        const int piece  = pos->pieces[index];

        ASSERT(PceValidEmptyOffbrd(piece));
        if (piece != OFFBOARD && piece != EMPTY) {
            const int color = pieceColor[piece];
            ASSERT(SideValid(color));

            if (isBigPiece[piece]   == TRUE) pos->bigPce[color]++;
            if (isMinorPiece[piece] == TRUE) pos->minPce[color]++;
            if (isMajorPiece[piece] == TRUE) pos->majPce[color]++;

            pos->material[color] += pieceValue[piece];

            ASSERT(pos->pceNum[piece] < 10 && pos->pceNum[piece] >= 0);
            pos->pList[piece][pos->pceNum[piece]] = square;
            pos->pceNum[piece]++;

            if (piece == wK) pos->KingSq[WHITE] = square;
            if (piece == bK) pos->KingSq[BLACK] = square;

            if (piece == wP) {
                SETBIT(pos->pawns[WHITE], SQ64(square));
                SETBIT(pos->pawns[BOTH],  SQ64(square));
            } else if (piece == bP) {
                SETBIT(pos->pawns[BLACK], SQ64(square));
                SETBIT(pos->pawns[BOTH],  SQ64(square));
            }
        }
    }
}

int loadFEN(char* fen, S_BOARD* pos) {
    ASSERT(fen != NULL);
    ASSERT(pos != NULL);

    int rank     = RANK_8;
    int file     = FILE_A;
    int piece    = 0;
    int runCount = 0;

    resetBoardState(pos);

    while ((rank >= RANK_1) && *fen) {
        runCount = 1;
        switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
                piece    = EMPTY;
                runCount = *fen - '0';
                break;

            case '/':
            case ' ':
                --rank;
                file = FILE_A;
                ++fen;
                continue;

            default:
                printf("FEN error \n");
                return -1;
        }

        for (int i = 0; i < runCount; ++i) {
            const int sq64  = rank * 8 + file;
            const int sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
            ++file;
        }
        ++fen;
    }

    ASSERT(*fen == 'w' || *fen == 'b');
    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;

    for (int i = 0; i < 4; ++i) {
        if (*fen == ' ') break;
        switch (*fen) {
            case 'K': pos->castlePerm |= WKCA; break;
            case 'Q': pos->castlePerm |= WQCA; break;
            case 'k': pos->castlePerm |= BKCA; break;
            case 'q': pos->castlePerm |= BQCA; break;
            default: break;
        }
        ++fen;
    }
    ++fen;

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    if (*fen != '-') {
        const int fileIdx = fen[0] - 'a';
        const int rankIdx = fen[1] - '1';

        ASSERT(fileIdx >= FILE_A && fileIdx <= FILE_H);
        ASSERT(rankIdx >= RANK_1 && rankIdx <= RANK_8);

        pos->enPas = FR2SQ(fileIdx, rankIdx);
    }

    pos->posKey = generatePositionKeys(pos);
    updateListsMaterial(pos);
    return 0;
}

void resetBoardState(S_BOARD* pos) {
    for (int index = 0; index < BRD_SQ_NUM; ++index) {
        pos->pieces[index] = OFFBOARD;
    }
    for (int sq64 = 0; sq64 < 64; ++sq64) {
        pos->pieces[SQ120(sq64)] = EMPTY;
    }

    for (int side = 0; side < 2; ++side) {
        pos->bigPce[side]   = 0;
        pos->majPce[side]   = 0;
        pos->minPce[side]   = 0;
        pos->material[side] = 0;
    }

    for (int p = 0; p < 3; ++p) {
        pos->pawns[p] = 0ULL;
    }

    for (int piece = 0; piece < 13; ++piece) {
        pos->pceNum[piece] = 0;
    }

    pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;

    pos->side      = BOTH;
    pos->enPas     = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply    = 0;
    pos->hisPly = 0;

    pos->castlePerm = 0;
    pos->posKey     = 0ULL;
}

void printBoardState(const S_BOARD* pos) {
    printf("\nGame Board:\n\n");

    for (int rank = RANK_8; rank >= RANK_1; --rank) {
        printf("%d  ", rank + 1);
        for (int file = FILE_A; file <= FILE_H; ++file) {
            const int square = FR2SQ(file, rank);
            const int piece  = pos->pieces[square];
            printf("%3c", pieceToCharacter[piece]);
        }
        printf("\n");
    }

    printf("\n   ");
    for (int file = FILE_A; file <= FILE_H; ++file) {
        printf("%3c", 'a' + file);
    }
    printf("\n");

    printf("side:%c\n", SideChar[pos->side]);
    printf("enPas:%d\n", pos->enPas);
    printf("castle:%c%c%c%c\n",
           (pos->castlePerm & WKCA) ? 'K' : '-',
           (pos->castlePerm & WQCA) ? 'Q' : '-',
           (pos->castlePerm & BKCA) ? 'k' : '-',
           (pos->castlePerm & BQCA) ? 'q' : '-');
    printf("PosKey:%llX\n", pos->posKey);
}

void mirrorBoardPosition(S_BOARD* pos) {
    int  tempPiecesArray[64];
    int  mirroredSide = pos->side ^ 1;
    static const int swapPiece[13] = { EMPTY, bP, bN, bB, bR, bQ, bK, wP, wN, wB, wR, wQ, wK };
    int  mirroredCastlePerm = 0;
    int  mirroredEnPas      = NO_SQ;

    if (pos->castlePerm & WKCA) mirroredCastlePerm |= BKCA;
    if (pos->castlePerm & WQCA) mirroredCastlePerm |= BQCA;
    if (pos->castlePerm & BKCA) mirroredCastlePerm |= WKCA;
    if (pos->castlePerm & BQCA) mirroredCastlePerm |= WQCA;

    if (pos->enPas != NO_SQ) {
        mirroredEnPas = SQ120(Mirror64[SQ64(pos->enPas)]);
    }

    for (int sq = 0; sq < 64; ++sq) {
        tempPiecesArray[sq] = pos->pieces[SQ120(Mirror64[sq])];
    }

    resetBoardState(pos);

    for (int sq = 0; sq < 64; ++sq) {
        const int mapped = swapPiece[tempPiecesArray[sq]];
        pos->pieces[SQ120(sq)] = mapped;
    }

    pos->side      = mirroredSide;
    pos->castlePerm = mirroredCastlePerm;
    pos->enPas      = mirroredEnPas;
    pos->posKey     = generatePositionKeys(pos);

    updateListsMaterial(pos);
    ASSERT(isBoardStateValid(pos));
}
