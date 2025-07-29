// Bishop.cpp
#include "Board.h"
#include "Bishop.h"
#include "Spot.h"
#include <cmath>

Bishop::Bishop(bool white) : Piece(white) {}


bool Bishop::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int>) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;

    int dx = end->getX() - start->getX();
    int dy = end->getY() - start->getY();

    if (abs(dx) != abs(dy))
        return false;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    int x = start->getX() + stepX;
    int y = start->getY() + stepY;

    while (x != end->getX() && y != end->getY()) {
        if (board->getBox(x, y)->getPiece() != nullptr)
            return false;
        x += stepX;
        y += stepY;
    }

    return true;
}

