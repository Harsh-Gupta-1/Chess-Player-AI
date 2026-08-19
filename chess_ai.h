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
        long long qNodes = 0;
        long long betaCutoffs = 0;
        long long firstMoveCutoffs = 0;
        long long ttProbes = 0;
        long long ttHits = 0;
        long long ttUsableHits = 0;
        long long ttCutoffs = 0;
        long long ttStores = 0;
        long long ttCollisions = 0;
        long long pvsSearches = 0;
        long long pvsResearches = 0;
        long long lmrAttempts = 0;
        long long lmrReductions = 0;
        long long lmrResearches = 0;
        long long nullAttempts = 0;
        long long nullCutoffs = 0;
        long long killerHits = 0;
        long long historyHits = 0;
        
        void clear() {
            qNodes = betaCutoffs = firstMoveCutoffs = ttProbes = ttHits = ttUsableHits = ttCutoffs = ttStores = ttCollisions = 0;
            pvsSearches = pvsResearches = lmrAttempts = lmrReductions = lmrResearches = 0;
            nullAttempts = nullCutoffs = killerHits = historyHits = 0;
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
    
    ChessAI() : tt(4194304) { // exactly 2^22 entries (~134MB RAM)
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