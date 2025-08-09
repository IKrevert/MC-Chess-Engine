#include <cstdio>
#include "../defs.h"
#include "../board/board.h"
#include "../move/move_generator.h"
#include "../move/make_move.h"
#include "../util/notation.h"
#include "../util/misc.h"   // for GetTimeMs()

long leafNodes = 0;

void Perft(int depth, S_BOARD* pos) {
    ASSERT(isBoardStateValid(pos));

    if (depth == 0) {
        ++leafNodes;
        return;
    }

    S_MOVELIST list[1];
    generateAllMoves(pos, list);

    for (int i = 0; i < list->count; ++i) {
        const int move = list->moves[i].move;
        if (!makeMove(pos, move)) {
            continue;
        }
        Perft(depth - 1, pos);
        takeMove(pos);
    }
}

void PerftTest(int depth, S_BOARD* pos) {
    ASSERT(isBoardStateValid(pos));

    printBoardState(pos);
    std::printf("\nStarting Test To Depth:%d\n", depth);

    leafNodes = 0;
    const int startMs = GetTimeMs();

    S_MOVELIST list[1];
    generateAllMoves(pos, list);

    for (int i = 0; i < list->count; ++i) {
        const int move = list->moves[i].move;
        if (!makeMove(pos, move)) {
            continue;
        }
        const long before = leafNodes;
        Perft(depth - 1, pos);
        takeMove(pos);
        const long nodesForMove = leafNodes - before;
        std::printf("move %d : %s : %ld\n", i + 1, moveToString(move), nodesForMove);
    }

    std::printf("\nTest Complete : %ld nodes visited in %dms\n",
                leafNodes, GetTimeMs() - startMs);
}
