#ifndef CHESS_AI_H
#define CHESS_AI_H

#include "board.h"
#include "transposition_table.h"
#include "zobrist.h"
#include <limits>

class ChessAI {
public:
    TranspositionTable tt;
    long long nodesExplored = 0;
    
    ChessAI() : tt(1000003) {
        Zobrist::init();
    }
    
    Move getBestMove(Board& board, Color aiColor, int depth);

private:
    int minimax(Board& board, int depth, int alpha, int beta, Color currentTurn, bool maximizing);
};

#endif // CHESS_AI_H