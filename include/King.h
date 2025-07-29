// King.h
#pragma once
#include "Piece.h"

class King : public Piece {
private:
    bool castlingDone;

public:
    King(bool white);

    bool isCastlingDone() const;
    void setCastlingDone(bool castlingDone);
    bool canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget = {-1, -1}) override;
    bool isValidCastling(Board* board, Spot* start, Spot* end);
    bool isCastlingMove(Spot* start, Spot* end);
};