#pragma once
#include "Player.h"
#include "Spot.h"
#include "Piece.h"

class Move {
private:
    Player* player;
    Spot* start;
    Spot* end;
    Piece* pieceMoved;
    Piece* pieceKilled;
    bool castlingMove;

public:
    Move(Player* player, Spot* start, Spot* end);

    Player* getPlayer() const;
    Spot* getStart() const;
    Spot* getEnd() const;
    Piece* getPieceMoved() const;
    Piece* getPieceKilled() const;
    void setPieceKilled(Piece* piece);
    bool isCastlingMove() const;
    void setCastlingMove(bool castling);
};