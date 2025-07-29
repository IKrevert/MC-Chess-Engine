#include "Board.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include <stdexcept>

#include "Game.h"

extern Game* currentGame; 

std::pair<int, int> Board::getEnPassantTarget() const {
    return currentGame ? currentGame->enPassantTarget : std::make_pair(-1, -1);
}

Board::Board() {
    resetBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete boxes[i][j];
}

Spot* Board::getBox(int x, int y) {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        throw std::out_of_range("Index out of bound");
    }
    return boxes[x][y];
}


bool Board::isSquareUnderAttack(int x, int y, bool byWhite) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* p = boxes[i][j]->getPiece();
            if (p && p->isWhite() == byWhite) {
                if (p->canMove(this, boxes[i][j], boxes[x][y])) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::pair<int, int> Board::findKing(bool white) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* p = boxes[i][j]->getPiece();
            if (p && p->isWhite() == white && dynamic_cast<King*>(p)) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // should never happen if king is on board
}

bool Board::hasAnyLegalMove(bool white) {
    for (int sx = 0; sx < 8; ++sx) {
        for (int sy = 0; sy < 8; ++sy) {
            Spot* start = boxes[sx][sy];
            Piece* p = start->getPiece();
            if (!p || p->isWhite() != white) continue;

            for (int ex = 0; ex < 8; ++ex) {
                for (int ey = 0; ey < 8; ++ey) {
                    Spot* end = boxes[ex][ey];

                    if (!p->canMove(this, start, end)) continue;

                    // simulate move
                    Piece* captured = end->getPiece();
                    end->setPiece(p);
                    start->setPiece(nullptr);

                    auto [kx, ky] = findKing(white);
                    bool inCheck = isSquareUnderAttack(kx, ky, !white);

                    // undo move
                    start->setPiece(p);
                    end->setPiece(captured);

                    if (!inCheck) return true;
                }
            }
        }
    }
    return false;
}

void Board::resetBoard() {
    // White
    boxes[0][0] = new Spot(0, 0, new Rook(true));
    boxes[0][1] = new Spot(0, 1, new Knight(true));
    boxes[0][2] = new Spot(0, 2, new Bishop(true));
    boxes[0][3] = new Spot(0, 3, new Queen(true));
    boxes[0][4] = new Spot(0, 4, new King(true));
    boxes[0][5] = new Spot(0, 5, new Bishop(true));
    boxes[0][6] = new Spot(0, 6, new Knight(true));
    boxes[0][7] = new Spot(0, 7, new Rook(true));
    for (int j = 0; j < 8; ++j)
        boxes[1][j] = new Spot(1, j, new Pawn(true));

    // Black
    boxes[7][0] = new Spot(7, 0, new Rook(false));
    boxes[7][1] = new Spot(7, 1, new Knight(false));
    boxes[7][2] = new Spot(7, 2, new Bishop(false));
    boxes[7][3] = new Spot(7, 3, new Queen(false));
    boxes[7][4] = new Spot(7, 4, new King(false));
    boxes[7][5] = new Spot(7, 5, new Bishop(false));
    boxes[7][6] = new Spot(7, 6, new Knight(false));
    boxes[7][7] = new Spot(7, 7, new Rook(false));
    for (int j = 0; j < 8; ++j)
        boxes[6][j] = new Spot(6, j, new Pawn(false));

    // Empty
    for (int i = 2; i < 6; ++i)
        for (int j = 0; j < 8; ++j)
            boxes[i][j] = new Spot(i, j, nullptr);
}
