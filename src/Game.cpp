#include <iostream>
#include "Game.h"
#include "King.h"
#include "Pawn.h"

Game* currentGame = nullptr;

Game::Game(Player* p1, Player* p2) {
    players[0] = p1;
    players[1] = p2;
    board.resetBoard();
    currentTurn = p1->isWhiteSide() ? p1 : p2;
    status = GameStatus::ACTIVE;
}

Game::~Game() {
    for (auto move : movesPlayed)
        delete move;
}

Board& Game::getBoard() {
    return board;
}

Player* Game::getCurrentTurn() const {
    return currentTurn;
}


bool Game::isEnd() const {
    return status != GameStatus::ACTIVE;
}

GameStatus Game::getStatus() const {
    return status;
}

void Game::setStatus(GameStatus s) {
    status = s;
}

bool Game::playerMove(Player* player, int startX, int startY, int endX, int endY) {
    Spot* startBox = board.getBox(startX, startY);
    Spot* endBox = board.getBox(endX, endY);
    Move* move = new Move(player, startBox, endBox);
    return makeMove(move, player);
}

bool Game::makeMove(Move* move, Player* player) {
    Piece* sourcePiece = move->getStart()->getPiece();
    if (!sourcePiece) return false;
    if (player != currentTurn) return false;
    if (sourcePiece->isWhite() != player->isWhiteSide()) return false;

    Pawn* pawn = dynamic_cast<Pawn*>(sourcePiece);

    // Only call canMove once, and pass enPassantTarget if it's a pawn
    if (!(pawn 
        ? pawn->canMove(&board, move->getStart(), move->getEnd(), enPassantTarget)
        : sourcePiece->canMove(&board, move->getStart(), move->getEnd()))) {
        return false;
    }

    // Only reset AFTER move is validated
    std::pair<int, int> oldEnPassantTarget = enPassantTarget;
    enPassantTarget = { -1, -1 };

    Piece* destPiece = move->getEnd()->getPiece();
    if (destPiece) {
        destPiece->setKilled(true);
        move->setPieceKilled(destPiece);
    }

    // En passant capture (must use old target)
    if (pawn) {
        int sx = move->getStart()->getX();
        int sy = move->getStart()->getY();
        int ex = move->getEnd()->getX();
        int ey = move->getEnd()->getY();

        if (abs(ex - sx) == 2) {
            enPassantTarget = { (sx + ex) / 2, sy };
        }

        if (ey != sy && !destPiece) {
            Spot* capturedPawnSpot = board.getBox(sx, ey);
            Piece* captured = capturedPawnSpot->getPiece();
            if (captured && dynamic_cast<Pawn*>(captured)) {
                captured->setKilled(true);
                move->setPieceKilled(captured);
                capturedPawnSpot->setPiece(nullptr);
            }
        }
    }

    // Castling logic
    King* king = dynamic_cast<King*>(sourcePiece);
    if (king && king->isCastlingMove(move->getStart(), move->getEnd())) {
        if (!king->isValidCastling(&board, move->getStart(), move->getEnd())) return false;

        move->setCastlingMove(true);
        king->setCastlingDone(true);

        int rank = move->getStart()->getX();
        if (move->getEnd()->getY() == 6) { // Kingside
            Spot* rookStart = board.getBox(rank, 7);
            Spot* rookEnd = board.getBox(rank, 5);
            rookEnd->setPiece(rookStart->getPiece());
            rookStart->setPiece(nullptr);
        } else if (move->getEnd()->getY() == 2) { // Queenside
            Spot* rookStart = board.getBox(rank, 0);
            Spot* rookEnd = board.getBox(rank, 3);
            rookEnd->setPiece(rookStart->getPiece());
            rookStart->setPiece(nullptr);
        }
    }

    // Move piece
    move->getEnd()->setPiece(sourcePiece);
    move->getStart()->setPiece(nullptr);
    movesPlayed.push_back(move);
    sourcePiece->setHasMoved(true);

    // Win detection
    if (destPiece && dynamic_cast<King*>(destPiece)) {
        setStatus(player->isWhiteSide() ? GameStatus::WHITE_WIN : GameStatus::BLACK_WIN);
    }

    // Toggle turn
    currentTurn = (currentTurn == players[0]) ? players[1] : players[0];

    // Check, Checkmate, Stalemate
    bool opponentWhite = currentTurn->isWhiteSide();
    auto [kx, ky] = board.findKing(opponentWhite);

    if (board.isSquareUnderAttack(kx, ky, !opponentWhite)) {
        if (!board.hasAnyLegalMove(opponentWhite)) {
            setStatus(opponentWhite ? GameStatus::BLACK_WIN : GameStatus::WHITE_WIN);
            std::cout << "Checkmate!\n";
        } else {
            std::cout << "Check!\n";
        }
    } else if (!board.hasAnyLegalMove(opponentWhite)) {
        setStatus(GameStatus::STALEMATE);
        std::cout << "Stalemate!\n";
    }

    return true;
}
