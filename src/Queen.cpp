#include "Queen.h"
#include "Spot.h"
#include <cmath>
#include "Board.h"

Queen::Queen(bool white) : Piece(white) {}

bool Queen::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;

    int startX = start->getX(), startY = start->getY();
    int endX = end->getX(), endY = end->getY();
    int dx = endX - startX;
    int dy = endY - startY;

    if (abs(dx) == abs(dy)) {
        // Diagonal move
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;
        int x = startX + stepX;
        int y = startY + stepY;

        while (x != endX && y != endY) {
            if (board->getBox(x, y)->getPiece() != nullptr)
                return false;
            x += stepX;
            y += stepY;
        }
        return true;

    } else if (startX == endX || startY == endY) {
        // Straight move (like rook)
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

    return false;
}
