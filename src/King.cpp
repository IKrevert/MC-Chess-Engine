#include "King.h"
#include "Board.h"
#include "Spot.h"
#include "Rook.h"
#include <cmath>

King::King(bool white) : Piece(white), castlingDone(false) {}

bool King::isCastlingDone() const { return castlingDone; }
void King::setCastlingDone(bool d) { castlingDone = d; }

bool King::canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget) {
    if (end->getPiece() && end->getPiece()->isWhite() == this->isWhite())
        return false;
    int x = abs(start->getX() - end->getX());
    int y = abs(start->getY() - end->getY());
    if (x + y == 1) return true;
    return isValidCastling(board, start, end);
}

bool King::isCastlingMove(Spot* start, Spot* end) {
    // A castling move is a horizontal move of 2 squares by the king
    return (start->getX() == end->getX()) && (abs(start->getY() - end->getY()) == 2);
}

bool King::isValidCastling(Board* board, Spot* start, Spot* end) {
    if (this->isCastlingDone() || this->getHasMoved()) return false;

    int rank = start->getX();
    int diff = end->getY() - start->getY();

    if (diff == 2) { // Kingside castling
        Spot* f = board->getBox(rank, 5);
        Spot* g = board->getBox(rank, 6);
        Spot* h = board->getBox(rank, 7);
        if (!f->getPiece() && !g->getPiece() && h->getPiece()) {
            Rook* rook = dynamic_cast<Rook*>(h->getPiece());
            if (rook && !rook->isKilled() && !rook->getHasMoved())
                return true;
        }
    } else if (diff == -2) { // Queenside castling
        Spot* d = board->getBox(rank, 3);
        Spot* c = board->getBox(rank, 2);
        Spot* b = board->getBox(rank, 1);
        Spot* a = board->getBox(rank, 0);
        if (!d->getPiece() && !c->getPiece() && !b->getPiece() && a->getPiece()) {
            Rook* rook = dynamic_cast<Rook*>(a->getPiece());
            if (rook && !rook->isKilled() && !rook->getHasMoved())
                return true;
        }
    }

    return false;
}
