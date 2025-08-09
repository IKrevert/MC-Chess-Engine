#include <cstdio>
#include "defs.h"
#include "../move/move_generator.h"
#include "notation.h"
#include "setup.h" 

char* squareToString(const int square) {
    static char sqStr[3];

    const int fileIdx = fileIndex120[square];
    const int rankIdx = rankIndex120[square];

    std::sprintf(sqStr, "%c%c", ('a' + fileIdx), ('1' + rankIdx));
    return sqStr;
}

char* moveToString(const int move) {
    static char mvStr[6];

    const int fromFile = fileIndex120[FROMSQ(move)];
    const int fromRank = rankIndex120[FROMSQ(move)];
    const int toFile   = fileIndex120[TOSQ(move)];
    const int toRank   = rankIndex120[TOSQ(move)];

    const int promoted = PROMOTED(move);

    if (promoted) {
        char promoChar = 'q';
        if (IsKn(promoted)) {
            promoChar = 'n';
        } else if (IsRQ(promoted) && !IsBQ(promoted)) {
            promoChar = 'r';
        } else if (!IsRQ(promoted) && IsBQ(promoted)) {
            promoChar = 'b';
        }
        std::sprintf(mvStr, "%c%c%c%c%c",
                     ('a' + fromFile), ('1' + fromRank),
                     ('a' + toFile),   ('1' + toRank),
                     promoChar);
    } else {
        std::sprintf(mvStr, "%c%c%c%c",
                     ('a' + fromFile), ('1' + fromRank),
                     ('a' + toFile),   ('1' + toRank));
    }
    return mvStr;
}

int parseMoveString(char* text, S_BOARD* board) {
    ASSERT(isBoardStateValid(board));

    // quick format checks
    if (text[1] > '8' || text[1] < '1') return NOMOVE;
    if (text[3] > '8' || text[3] < '1') return NOMOVE;
    if (text[0] > 'h' || text[0] < 'a') return NOMOVE;
    if (text[2] > 'h' || text[2] < 'a') return NOMOVE;

    const int fromSq = FR2SQ(text[0] - 'a', text[1] - '1');
    const int toSq   = FR2SQ(text[2] - 'a', text[3] - '1');

    ASSERT(SqOnBoard(fromSq) && SqOnBoard(toSq));

    S_MOVELIST moveList[1];
    generateAllMoves(board, moveList);

    for (int i = 0; i < moveList->count; ++i) {
        const int moveEnc = moveList->moves[i].move;
        if (FROMSQ(moveEnc) == fromSq && TOSQ(moveEnc) == toSq) {
            const int promoPiece = PROMOTED(moveEnc);
            if (promoPiece != EMPTY) {
                // require matching promotion letter in text[4]
                if (IsRQ(promoPiece) && !IsBQ(promoPiece) && text[4] == 'r') return moveEnc;
                if (!IsRQ(promoPiece) && IsBQ(promoPiece) && text[4] == 'b') return moveEnc;
                if (IsRQ(promoPiece) &&  IsBQ(promoPiece) && text[4] == 'q') return moveEnc;
                if (IsKn(promoPiece)                         && text[4] == 'n') return moveEnc;
                continue;
            }
            return moveEnc;
        }
    }
    return NOMOVE;
}

void printMoveList(const S_MOVELIST* moveList) {
    std::printf("MoveList:\n");
    for (int i = 0; i < moveList->count; ++i) {
        const int moveEnc = moveList->moves[i].move;
        const int score   = moveList->moves[i].score;
        std::printf("Move:%d > %s (score:%d)\n", i + 1, moveToString(moveEnc), score);
    }
    std::printf("MoveList Total %d Moves:\n\n", moveList->count);
}
