#include "Knight.h"
#include "Spot.h"
#include <cmath>

Knight::Knight(bool white) : Piece(white) {}

bool Knight::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;
    int x = abs(start->getX() - end->getX());
    int y = abs(start->getY() - end->getY());
    return x * y == 2;
}