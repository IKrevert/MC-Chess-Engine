#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(bool white);
    bool canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget = {-1, -1}) override;

};