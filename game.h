#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "chess_ai.h"

class Game {
private:
    Board board;
    ChessAI ai;
    Color humanColor = WHITE;
    
    // Helper methods for improved error handling
    void clearInputStream();
    bool parseMove(const std::string& input, std::string& from, std::string& to, std::string& promotion);

public:
    void play();
    void playerTurn(Color color);
    void aiTurn(Color color);
};

#endif // GAME_H