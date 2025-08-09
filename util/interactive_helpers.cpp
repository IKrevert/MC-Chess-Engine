#include "interactive_helpers.h"
#include <cstdio>
#include <cstring>
#include <cctype>
#include "../board/board_data.h"
#include "../move/move_generator.h"
#include "../move/make_move.h"
#include "notation.h"

int ParseSquare2(const char* s) {
    if (!s) return -1;
    while (*s == ' ') ++s;
    if (s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8') return -1;
    const int file = s[0] - 'a';
    const int rank = s[1] - '1';
    return FR2SQ(file, rank);
}

int ParsePromotionPiece(char c, int side) {
    if (!c) return EMPTY;
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    switch (c) {
        case 'q': return side == WHITE ? wQ : bQ;
        case 'r': return side == WHITE ? wR : bR;
        case 'b': return side == WHITE ? wB : bB;
        case 'n': return side == WHITE ? wN : bN;
        default:  return EMPTY;
    }
}

int LegalMovesFrom(S_BOARD* pos, int from,
                   int outMoves[], char labels[][8], int maxOut) {
    S_MOVELIST list[1];
    generateAllMoves(pos, list);

    int count = 0;
    for (int i = 0; i < list->count && count < maxOut; ++i) {
        const int m = list->moves[i].move;
        if (FROMSQ(m) != from) continue;

        if (makeMove(pos, m) == TRUE) {
            takeMove(pos);

            const int to   = TOSQ(m);
            const int prom = PROMOTED(m);
            if (prom == EMPTY) {
                std::snprintf(labels[count], 8, "%s", squareToString(to));
            } else {
                char pc = 'Q';
                switch (prom) {
                    case wQ: case bQ: pc = 'Q'; break;
                    case wR: case bR: pc = 'R'; break;
                    case wB: case bB: pc = 'B'; break;
                    case wN: case bN: pc = 'N'; break;
                }
                std::snprintf(labels[count], 8, "%s=%c", squareToString(to), pc);
            }
            outMoves[count++] = m;
        }
    }
    return count;
}

int MatchDestinationMove(S_BOARD* pos, int from,
                         const char* destStr, const int legalMoves[], int nMoves) {
    if (!destStr) return NOMOVE;

    char dst[8] = {0};
    int di = 0;
    while (destStr[di] && destStr[di] != '\n' && di < 7) { dst[di] = destStr[di]; ++di; }
    dst[di] = 0;

    char* eq = std::strchr(dst, '=');
    char promChar = 0;
    if (eq) {
        if (eq[1]) promChar = eq[1];
        *eq = '\0';
    } else if (di >= 3) {
        const char tail = dst[2];
        if (tail=='Q'||tail=='q'||tail=='R'||tail=='r'||tail=='B'||tail=='b'||tail=='N'||tail=='n') {
            promChar = tail;
            dst[2] = '\0';
        }
    }

    const int to = ParseSquare2(dst);
    if (to == -1) return NOMOVE;

    int cand[8];
    int candN = 0;
    for (int i = 0; i < nMoves && candN < 8; ++i) {
        const int m = legalMoves[i];
        if (FROMSQ(m) == from && TOSQ(m) == to) {
            cand[candN++] = m;
        }
    }
    if (candN == 0) return NOMOVE;
    if (candN == 1) {
        const int m = cand[0];
        if (PROMOTED(m) != EMPTY && promChar == 0) {
            for (int i = 0; i < candN; ++i) {
                const int pm = cand[i];
                const int pr = PROMOTED(pm);
                if (pr == (pos->side == WHITE ? wQ : bQ)) return pm;
            }
        }
        return m;
    }

    if (promChar) {
        const int want = ParsePromotionPiece(promChar, pos->side);
        if (want != EMPTY) {
            for (int i = 0; i < candN; ++i) {
                if (PROMOTED(cand[i]) == want) return cand[i];
            }
        }
    }

    for (int i = 0; i < candN; ++i) {
        const int pr = PROMOTED(cand[i]);
        if (pr == (pos->side == WHITE ? wQ : bQ)) return cand[i];
    }
    return cand[0];
}
