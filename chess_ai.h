#ifndef CHESS_AI_H
#define CHESS_AI_H

#include "board.h"
#include "transposition_table.h"
#include "zobrist.h"
#include <limits>
#include <chrono>
#include <cstring>

class ChessAI {
public:
    TranspositionTable tt;
    long long nodesExplored = 0;
    
    // Time management
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    long long timeLimitMs = 1000;
    bool timeOut = false;
    
    // Heuristics
    Move killerMoves[100][2];
    int historyMoves[2][64][64];
    
    ChessAI() : tt(1000003) {
        Zobrist::init();
        for(int i=0; i<100; i++) {
            killerMoves[i][0] = Move(0,0,0,0);
            killerMoves[i][1] = Move(0,0,0,0);
        }
        std::memset(historyMoves, 0, sizeof(historyMoves));
    }
    
    Move getBestMove(Board& board, Color aiColor, int maxDepth);

private:
    int negamax(Board& board, int depth, int ply, int alpha, int beta, Color currentTurn);
    int scoreMove(const Move& move, const Move& ttMove, const Board& board, int ply, Color currentTurn);
};

#endif // CHESS_AI_H