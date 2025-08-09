// main.cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "defs.h"
#include "board/board_data.h"
#include "board/board.h"
#include "move/move_generator.h"
#include "move/make_move.h"
#include "util/notation.h"
#include "util/setup.h"
#include "util/interactive_helpers.h"






#include "test/perft.h"


//PERF TEST 


int main() {
    initializeEngine();

    S_BOARD pos[1];
    loadFEN(START_FEN, pos);  // start position
    printBoardState(pos);

    int depth = 5; //depth of perf testing 
    PerftTest(depth, pos);

    return 0;
}




















/* 


MOVE GENERATOR WITH USER INPUT 



*/




// int main(int /*argc*/, char* /*argv*/[]) {
//     initializeEngine();

//     S_BOARD pos[1];

//     std::setbuf(stdin,  nullptr);
//     std::setbuf(stdout, nullptr);

//     // Load start position so "e2" etc. works immediately
//     if (loadFEN(START_FEN, pos) != 0) {
//         std::printf("Failed to parse START_FEN\n");
//     }

//     std::printf("Type a square (e.g., 'e2'), then a destination (e.g., 'e4').\n");
//     std::printf("Commands: board | new | fen <FEN> | undo | quit\n");

//     // Selection state
//     int  pending_from  = NO_SQ;
//     int  pending_moves[MAXPOSITIONMOVES];
//     char pending_labels[MAXPOSITIONMOVES][8];
//     int  pending_count = 0;

//     char line[512];

//     while (TRUE) {
//         std::memset(line, 0, sizeof(line));
//         std::fflush(stdout);

//         if (!std::fgets(line, static_cast<int>(sizeof(line)), stdin)) continue;
//         if (line[0] == '\n') continue;

//         // Commands
//         if (!std::strncmp(line, "quit", 4)) break;

//         if (!std::strncmp(line, "board", 5)) {
//             printBoardState(pos);
//             continue;
//         }

//         if (!std::strncmp(line, "new", 3)) {
//             loadFEN(START_FEN, pos);
//             pending_from = NO_SQ;
//             pending_count = 0;
//             printBoardState(pos);
//             continue;
//         }

//         if (!std::strncmp(line, "fen ", 4)) {
//             const char* fen = line + 4;
//             if (loadFEN(const_cast<char*>(fen), pos) != 0) std::printf("Bad FEN\n");
//             pending_from = NO_SQ;
//             pending_count = 0;
//             printBoardState(pos);
//             continue;
//         }

//         if (!std::strncmp(line, "undo", 4)) {
//             if (pos->hisPly > 0) {
//                 takeMove(pos);
//                 pending_from = NO_SQ;
//                 pending_count = 0;
//                 printBoardState(pos);
//             } else {
//                 std::printf("Nothing to undo\n");
//             }
//             continue;
//         }

//         // If no pending selection, treat input as a "from" square.
//         if (pending_from == NO_SQ) {
//             const int from = ParseSquare2(line);
//             if (from == -1) {
//                 std::printf("Unknown command or bad square. Try: e2 | board | new | fen <FEN> | undo | quit\n");
//                 continue;
//             }

//             const int pce = pos->pieces[from];
//             if (pce == EMPTY || pce == OFFBOARD) {
//                 std::printf("No piece on %s\n", squareToString(from));
//                 continue;
//             }
//             if (pieceColor[pce] != pos->side) {
//                 std::printf("It's %s to move; %s has a %c on %s\n",
//                             (pos->side == WHITE ? "White" : "Black"),
//                             (pieceColor[pce] == WHITE ? "White" : "Black"),
//                             pieceToCharacter[pce],
//                             squareToString(from));
//                 continue;
//             }

//             pending_count = LegalMovesFrom(pos, from, pending_moves, pending_labels, MAXPOSITIONMOVES);
//             if (pending_count == 0) {
//                 std::printf("No legal moves for %s\n", squareToString(from));
//                 continue;
//             }

//             pending_from = from;
//             std::printf("Legal moves for %s (%c): ", squareToString(from), pieceToCharacter[pce]);
//             for (int i = 0; i < pending_count; ++i) {
//                 std::printf("%s%s", pending_labels[i], (i + 1 == pending_count ? "" : " "));
//             }
//             std::printf("\n");
//             continue;
//         }

//         // We have a pending from-square; treat input as a destination.
//         const char* p = line;
//         while (*p == ' ') ++p;

//         const int chosen = MatchDestinationMove(pos, pending_from, p, pending_moves, pending_count);
//         if (chosen == NOMOVE) {
//             std::printf("'%.*s' is not a legal destination for %s. Try again.\n",
//                         static_cast<int>(std::strcspn(p, "\n")), p, squareToString(pending_from));
//             continue;
//         }

//         // Make the move
//         if (makeMove(pos, chosen) == TRUE) {
//             pending_from  = NO_SQ;
//             pending_count = 0;
//             printBoardState(pos);

//             std::printf("%s to move. Select a piece (e.g., '%s').\n",
//                         (pos->side == WHITE ? "White" : "Black"),
//                         (pos->side == WHITE ? "e2" : "e7"));
//         } else {
//             std::printf("Internal error: move became illegal. Try again.\n");
//         }
//     }

//     return 0;
// }
