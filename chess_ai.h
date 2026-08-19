#ifndef CHESS_AI_H
#define CHESS_AI_H

#include "board.h"
#include "transposition_table.h"
#include "zobrist.h"
#include <limits>
#include <chrono>
#include <cstring>
#include <atomic>

class ChessAI {
public:
    TranspositionTable tt;
    long long nodesExplored = 0;
    
    // Time management
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    long long timeLimitMs = 1000;
    std::atomic<bool> stopSearch{false};
    
    // Telemetry
    struct SearchStats {
        long long ttHits = 0;
        long long ttCutoffs = 0;
        long long qNodes = 0;
        long long nullCutoffs = 0;
        long long lmrReductions = 0;
        long long pvsResearches = 0;
        
        void clear() {
            ttHits = ttCutoffs = qNodes = nullCutoffs = lmrReductions = pvsResearches = 0;
        }
    } stats;
    
    
    // Heuristics
    Move killerMoves[100][2];
    int historyMoves[2][64][64];
    
    // Ablation Flags
    bool enableNullMove = true;
    bool enableLMR = true;
    bool enableKiller = true;
    bool enableHistory = true;
    
    ChessAI() : tt(4000003) { // 4 Million entries (~128MB RAM)
        Zobrist::init();
        for(int i=0; i<100; i++) {
            killerMoves[i][0] = Move(0,0,0,0);
            killerMoves[i][1] = Move(0,0,0,0);
        }
        std::memset(historyMoves, 0, sizeof(historyMoves));
    }
    
    Move getBestMove(Board& board, Color aiColor, int maxDepth);
    int negamax(Board& board, int depth, int ply, int alpha, int beta, Color currentTurn, bool allowNull);

private:
    int quiescence(Board& board, int ply, int alpha, int beta, Color currentTurn);
    int scoreMove(const Move& move, const Move& ttMove, const Board& board, int ply, Color currentTurn);
};

#endif // CHESS_AI_H