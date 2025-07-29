#include "Player.h"

bool Player::isWhiteSide() const { return whiteSide; }
bool Player::isHumanPlayer() const { return humanPlayer; }

HumanPlayer::HumanPlayer(bool white) {
    whiteSide = white;
    humanPlayer = true;
}

ComputerPlayer::ComputerPlayer(bool white) {
    whiteSide = white;
    humanPlayer = false;
}