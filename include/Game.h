#pragma once
#include <vector>
#include "Player.h"
#include "Board.h"
#include "Move.h"
#include "GameStatus.h"

class Game {
private:
    Player* players[2];
    Board board;
    Player* currentTurn;
    GameStatus status;
    std::vector<Move*> movesPlayed;

public:
    Game(Player* p1, Player* p2);
    ~Game();
    bool isEnd() const;
    GameStatus getStatus() const;
    void setStatus(GameStatus s);
    bool playerMove(Player* player, int startX, int startY, int endX, int endY);
    Board& getBoard();
    Player* getCurrentTurn() const;

    std::pair<int, int> enPassantTarget = {-1, -1};


private:
    bool makeMove(Move* move, Player* player);
};