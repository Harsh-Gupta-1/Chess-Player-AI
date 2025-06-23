#ifndef CHESS_AI_H
#define CHESS_AI_H

#include "board.h"
#include <limits>

class ChessAI {
public:
    long long nodesExplored = 0;
    Move getBestMove(Board& board, Color aiColor, int depth);

private:
    int minimax(Board& board, int depth, int alpha, int beta, Color currentTurn, bool maximizing);
};

#endif // CHESS_AI_H