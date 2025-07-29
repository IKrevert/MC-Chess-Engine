#include "Piece.h"

Piece::Piece(bool isWhite) : white(isWhite), killed(false) {}

bool Piece::isWhite() const { return white; }
void Piece::setWhite(bool w) { white = w; }

bool Piece::isKilled() const { return killed; }
void Piece::setKilled(bool k) { killed = k; }


