#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool white);
    bool canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget = {-1, -1}) override;

};
