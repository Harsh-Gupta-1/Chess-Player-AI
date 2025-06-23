#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "chess_ai.h"

class Game {
private:
    Board board;
    ChessAI ai;
    Color humanColor = WHITE;

public:
    void play();
    void playerTurn(Color color);
    void aiTurn(Color color);
};

#endif // GAME_H