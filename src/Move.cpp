#include "Move.h"

Move::Move(Player* player, Spot* start, Spot* end)
    : player(player), start(start), end(end), castlingMove(false) {
    pieceMoved = start->getPiece();
    pieceKilled = end->getPiece();
}

Player* Move::getPlayer() const { return player; }
Spot* Move::getStart() const { return start; }
Spot* Move::getEnd() const { return end; }
Piece* Move::getPieceMoved() const { return pieceMoved; }
Piece* Move::getPieceKilled() const { return pieceKilled; }
void Move::setPieceKilled(Piece* piece) { pieceKilled = piece; }
bool Move::isCastlingMove() const { return castlingMove; }
void Move::setCastlingMove(bool castling) { castlingMove = castling; }
