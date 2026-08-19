#include "chess_ai.h"
#include <iostream>
#include <algorithm>
#include <vector>

bool isSameMove(const Move& m1, const Move& m2) {
    return m1.fromX == m2.fromX && m1.fromY == m2.fromY && m1.toX == m2.toX && m1.toY == m2.toY && m1.promotion == m2.promotion;
}

int ChessAI::scoreMove(const Move& move, const Move& ttMove, const Board& board, int ply, Color currentTurn) {
    if (isSameMove(move, ttMove)) {
        return 2000000; // Best move from TT
    }

    Piece captured = board.getPiece(move.toX, move.toY);
    if (captured.type != EMPTY) {
        Piece moving = board.getPiece(move.fromX, move.fromY);
        // MVV-LVA: Most Valuable Victim - Least Valuable Attacker
        // type: PAWN=1, KNIGHT=2, BISHOP=3, ROOK=4, QUEEN=5, KING=6
        return 1000000 + 10 * captured.type - moving.type;
    }

    if (move.promotion != EMPTY) {
        return 900000;
    }

    if (ply < 100) {
        if (isSameMove(move, killerMoves[ply][0])) return 800000;
        if (isSameMove(move, killerMoves[ply][1])) return 700000;
    }

    return historyMoves[currentTurn][move.fromX * 8 + move.fromY][move.toX * 8 + move.toY];
}

Move ChessAI::getBestMove(Board& board, Color aiColor, int maxDepth) {
    tt.clear();
    nodesExplored = 0;
    timeOut = false;
    startTime = std::chrono::steady_clock::now();
    for(int i=0; i<100; i++) {
        killerMoves[i][0] = Move(0,0,0,0);
        killerMoves[i][1] = Move(0,0,0,0);
    }
    std::memset(historyMoves, 0, sizeof(historyMoves));

    Move bestMove(0, 0, 0, 0);

    for (int depth = 1; depth <= maxDepth; depth++) {
        int alpha = std::numeric_limits<int>::min() + 1;
        int beta = std::numeric_limits<int>::max() - 1;
        int bestScore = std::numeric_limits<int>::min() + 1;
        
        std::vector<Move> legalMoves = board.generateLegalMoves(aiColor);
        if (legalMoves.empty()) break;
        
        Move currentBestMove = bestMove;

        std::vector<std::pair<int, Move>> scoredMoves;
        scoredMoves.reserve(legalMoves.size());
        for (const auto& move : legalMoves) {
            scoredMoves.push_back({scoreMove(move, currentBestMove, board, 0, aiColor), move});
        }
        std::sort(scoredMoves.begin(), scoredMoves.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
            return a.first > b.first;
        });

        for (const auto& pair : scoredMoves) {
            const Move& move = pair.second;
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int score = -negamax(board, depth - 1, 1, -beta, -alpha, aiColor == WHITE ? BLACK : WHITE);
            
            board.undoMove(move, captured, prevState);

            if (timeOut) break;

            if (score > bestScore) {
                bestScore = score;
                currentBestMove = move;
            }
            alpha = std::max(alpha, score);
        }
        
        if (timeOut && depth > 1) break; // Keep best move from previous depth if timed out
        bestMove = currentBestMove;
    }
    
    return bestMove;
}

int ChessAI::negamax(Board& board, int depth, int ply, int alpha, int beta, Color currentTurn) {
    if ((nodesExplored & 2047) == 0) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() >= timeLimitMs) {
            timeOut = true;
        }
    }
    
    if (timeOut) return 0;
    
    nodesExplored++;
    
    int originalAlpha = alpha;
    unsigned long long hashKey = Zobrist::computeHash(board, currentTurn);
    int ttScore;
    Move ttMove(0,0,0,0);
    
    if (tt.probe(hashKey, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }
    
    if (depth == 0) {
        return quiescence(board, alpha, beta, currentTurn);
    }
    
    if (board.isCheckmate(currentTurn)) {
        return -10000 + ply; // Prefer faster checkmates
    }
    
    if (board.isStalemate(currentTurn) || board.isDraw()) {
        return 0;
    }

    std::vector<Move> moves = board.generateLegalMoves(currentTurn);
    if (moves.empty()) return -10000 + ply;
    
    std::vector<std::pair<int, Move>> scoredMoves;
    scoredMoves.reserve(moves.size());
    for (const auto& move : moves) {
        scoredMoves.push_back({scoreMove(move, ttMove, board, ply, currentTurn), move});
    }
    std::sort(scoredMoves.begin(), scoredMoves.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });
    
    int maxEval = std::numeric_limits<int>::min() + 1;
    Move bestMoveForTT(0,0,0,0);
    
    for (const auto& pair : scoredMoves) {
        const Move& move = pair.second;
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        int eval = -negamax(board, depth - 1, ply + 1, -beta, -alpha, currentTurn == WHITE ? BLACK : WHITE);
        
        board.undoMove(move, captured, prevState);
        
        if (timeOut) return 0;
        
        if (eval > maxEval) {
            maxEval = eval;
            bestMoveForTT = move;
        }
        
        alpha = std::max(alpha, eval);
        if (alpha >= beta) {
            if (captured.type == EMPTY) {
                if (ply < 100 && !isSameMove(move, killerMoves[ply][0])) {
                    killerMoves[ply][1] = killerMoves[ply][0];
                    killerMoves[ply][0] = move;
                }
                historyMoves[currentTurn][move.fromX * 8 + move.fromY][move.toX * 8 + move.toY] += depth * depth;
            }
            break;
        }
    }
    
    Bound bound = EXACT;
    if (maxEval <= originalAlpha) bound = UPPER_BOUND;
    else if (maxEval >= beta) bound = LOWER_BOUND;
    
    if (!timeOut) {
        tt.store(hashKey, depth, maxEval, bound, bestMoveForTT);
    }
    
    return maxEval;
}

int ChessAI::quiescence(Board& board, int alpha, int beta, Color currentTurn) {
    if ((nodesExplored & 2047) == 0) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() >= timeLimitMs) {
            timeOut = true;
        }
    }
    
    if (timeOut) return 0;
    nodesExplored++;

    int standPat = board.evaluate();
    if (currentTurn == BLACK) standPat = -standPat;
    
    if (standPat >= beta) return beta;
    if (alpha < standPat) alpha = standPat;

    std::vector<Move> allMoves = board.generateLegalMoves(currentTurn);
    std::vector<std::pair<int, Move>> scoredCaptures;
    
    for (const Move& m : allMoves) {
        // Only consider captures and promotions in Q-Search
        if (board.getPiece(m.toX, m.toY).type != EMPTY || m.promotion != EMPTY) {
            Move dummyTT;
            scoredCaptures.push_back({scoreMove(m, dummyTT, board, 100, currentTurn), m});
        }
    }

    std::sort(scoredCaptures.begin(), scoredCaptures.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });

    for (const auto& pair : scoredCaptures) {
        const Move& move = pair.second;
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        int score = -quiescence(board, -beta, -alpha, currentTurn == WHITE ? BLACK : WHITE);
        
        board.undoMove(move, captured, prevState);
        
        if (timeOut) return 0;
        
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    
    return alpha;
}