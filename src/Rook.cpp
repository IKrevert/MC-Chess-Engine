// Rook.cpp
#include "Board.h"
#include "Rook.h"
#include "Spot.h"

Rook::Rook(bool white) : Piece(white) {}

bool Rook::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int>) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;

    int startX = start->getX(), startY = start->getY();
    int endX = end->getX(), endY = end->getY();

    if (startX != endX && startY != endY)
        return false;

    int stepX = (endX == startX) ? 0 : (endX > startX ? 1 : -1);
    int stepY = (endY == startY) ? 0 : (endY > startY ? 1 : -1);

    int x = startX + stepX;
    int y = startY + stepY;

    while (x != endX || y != endY) {
        if (board->getBox(x, y)->getPiece() != nullptr)
            return false;
        x += stepX;
        y += stepY;
    }

    return true;
}
