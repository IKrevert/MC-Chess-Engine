#pragma once

class Player {
protected:
    bool whiteSide;
    bool humanPlayer;

public:
    virtual ~Player() = default;
    bool isWhiteSide() const;
    bool isHumanPlayer() const;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(bool white);
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(bool white);
};
