#pragma once
#include <iostream>
#include "Spot.h"

class Board {
private:
    Spot* boxes[8][8];

public:
    Board();
    ~Board();
    Spot* getBox(int x, int y);
    void resetBoard();
    bool isSquareUnderAttack(int x, int y, bool byWhite);
    std::pair<int, int> findKing(bool white);
    bool hasAnyLegalMove(bool white);
    std::pair<int, int> getEnPassantTarget() const;
};