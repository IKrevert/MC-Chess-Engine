#include <iostream>
#include <memory>
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
void printBoard(Board& board) {
    std::cout << "  a b c d e f g h\n";
    for (int i = 7; i >= 0; --i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            Spot* s = board.getBox(i, j);
            Piece* p = s->getPiece();
            if (!p) {
                std::cout << ". ";
            } else {
                // You can expand this with proper piece notation
                if (dynamic_cast<King*>(p)) std::cout << (p->isWhite() ? "K " : "k ");
                else if (dynamic_cast<Queen*>(p)) std::cout << (p->isWhite() ? "Q " : "q ");
                else if (dynamic_cast<Rook*>(p)) std::cout << (p->isWhite() ? "R " : "r ");
                else if (dynamic_cast<Bishop*>(p)) std::cout << (p->isWhite() ? "B " : "b ");
                else if (dynamic_cast<Knight*>(p)) std::cout << (p->isWhite() ? "N " : "n ");
                else if (dynamic_cast<Pawn*>(p)) std::cout << (p->isWhite() ? "P " : "p ");
            }
        }
        std::cout << " " << i + 1 << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

int main() {
    std::unique_ptr<Player> white = std::make_unique<HumanPlayer>(true);
    std::unique_ptr<Player> black = std::make_unique<HumanPlayer>(false);
    Game game(white.get(), black.get());

    std::string input;
    while (!game.isEnd()) {
        printBoard(game.getBoard());
        Player* current = game.getCurrentTurn();
        std::cout << (current->isWhiteSide() ? "White" : "Black") << "'s move (e.g., e2 e4): ";
        std::string from, to;
        std::cin >> from >> to;

        int sx = from[1] - '1';
        int sy = from[0] - 'a';
        int ex = to[1] - '1';
        int ey = to[0] - 'a';

        if (!game.playerMove(current, sx, sy, ex, ey)) {
            std::cout << "Invalid move, try again.\n";
        }

        if (game.getStatus() == GameStatus::WHITE_WIN)
            std::cout << "White wins!\n";
        else if (game.getStatus() == GameStatus::BLACK_WIN)
            std::cout << "Black wins!\n";
    }

    return 0;
}