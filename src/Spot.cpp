#include "Spot.h"

Spot::Spot(int x, int y, Piece* piece) : x(x), y(y), piece(piece) {}

Piece* Spot::getPiece() const { return piece; }
void Spot::setPiece(Piece* p) { piece = p; }
int Spot::getX() const { return x; }
void Spot::setX(int x) { this->x = x; }
int Spot::getY() const { return y; }
void Spot::setY(int y) { this->y = y; }
