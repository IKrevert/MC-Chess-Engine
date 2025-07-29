#include "Pawn.h"
#include "Spot.h"
#include <cmath>
#include <iostream>


Pawn::Pawn(bool white) : Piece(white) {}

bool Pawn::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;

    int direction = isWhite() ? 1 : -1;
    int dx = end->getX() - start->getX();
    int dy = abs(start->getY() - end->getY());

    // Move forward by 1
    if (dy == 0 && dx == direction && end->getPiece() == nullptr)
        return true;

    // Move forward by 2 from starting rank
    if (dy == 0 && dx == 2 * direction && end->getPiece() == nullptr &&
        ((isWhite() && start->getX() == 1) || (!isWhite() && start->getX() == 6)))
        return true;

    // Diagonal capture or en passant
    if (dy == 1 && dx == direction) {
        if (end->getPiece() != nullptr)
            return true;

        // En passant: use the passed-in target directly
        if (end->getX() == enPassantTarget.first && end->getY() == enPassantTarget.second)
            return true;
    }

    return false;
}

