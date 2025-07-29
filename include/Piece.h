#pragma once
#include <iostream> 

class Board;
class Spot;

class Piece {
protected:
    bool killed;
    bool white;
     bool hasMoved = false;

public:
    Piece(bool isWhite);
    virtual ~Piece() = default;

    bool isWhite() const;
    void setWhite(bool white);

    bool isKilled() const;
    void setKilled(bool killed);

    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    virtual bool canMove(Board* board, Spot* start, Spot* end, std::pair<int, int> enPassantTarget = {-1, -1}) = 0;


};
