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

        if (depth == 1 && !scoredMoves.empty()) {
            currentBestMove = scoredMoves[0].second;
        }

        for (const auto& pair : scoredMoves) {
            const Move& move = pair.second;
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int score = -negamax(board, depth - 1, 1, -beta, -alpha, aiColor == WHITE ? BLACK : WHITE, true);
            
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
        
        // UCI info output
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (ms == 0) ms = 1;
        std::cerr << "info depth " << depth 
                  << " score cp " << bestScore
                  << " nodes " << nodesExplored
                  << " time " << ms
                  << " nps " << (nodesExplored * 1000 / ms)
                  << std::endl;
    }
    
    return bestMove;
}

int ChessAI::negamax(Board& board, int depth, int ply, int alpha, int beta, Color currentTurn, bool allowNull) {
    // Reverted back to 2048 to prevent huge syscall overhead on Windows
    if ((nodesExplored & 2047) == 0) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() >= timeLimitMs) {
            timeOut = true;
        }
    }
    
    if (timeOut) return 0;
    
    nodesExplored++;
    
    int originalAlpha = alpha;
    unsigned long long hashKey = board.gameState.zobristKey;
    int ttScore;
    Move ttMove(0,0,0,0);
    
    if (tt.probe(hashKey, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }
    
    // Cap maximum search depth to prevent stack overflow from runaway check extensions
    if (depth == 0 || ply >= 64) {
        return quiescence(board, alpha, beta, currentTurn);
    }
    
    bool inCheck = board.isInCheck(currentTurn);
    
    // Null Move Pruning: if we can pass our turn and still get a beta cutoff,
    // the position is so good we can prune it.
    if (allowNull && depth >= 3 && !inCheck) {
        // Make null move: just flip side to move via Zobrist
        GameState prevState = board.gameState;
        board.gameState.zobristKey ^= Zobrist::sideKey;
        if (board.gameState.hasEnPassant) {
            board.gameState.zobristKey ^= Zobrist::enPassantKeys[board.gameState.enPassantY];
            board.gameState.hasEnPassant = false;
        }
        
        int R = (depth > 6) ? 3 : 2; // Adaptive reduction
        int nullScore = -negamax(board, depth - 1 - R, ply + 1, -beta, -beta + 1, 
                                  currentTurn == WHITE ? BLACK : WHITE, false);
        
        board.gameState = prevState; // Undo null move
        
        if (timeOut) return 0;
        if (nullScore >= beta) {
            return beta;
        }
    }
    
    std::vector<Move> moves = board.generateLegalMoves(currentTurn);
    
    if (moves.empty()) {
        if (inCheck) {
            return -10000 + ply; // Checkmate
        }
        return 0; // Stalemate
    }
    
    if (board.isDraw()) {
        return 0;
    }
    
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
    int moveCount = 0;
    
    for (const auto& pair : scoredMoves) {
        const Move& move = pair.second;
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        int extension = (inCheck) ? 1 : 0;
        int nextDepth = depth - 1 + extension;
        int eval;
        
        moveCount++;
        
        // Late Move Reductions: moves ordered late are unlikely to be best,
        // so search them at reduced depth first
        bool isCapture = (captured.type != EMPTY);
        bool givesCheck = board.isInCheck(currentTurn == WHITE ? BLACK : WHITE);
        int reduction = 0;
        if (moveCount > 3 && depth >= 3 && !inCheck && !isCapture && !givesCheck && move.promotion == EMPTY) {
            reduction = 1;
            if (moveCount > 6) reduction = 2;
        }
        
        if (moveCount == 1) {
            eval = -negamax(board, nextDepth, ply + 1, -beta, -alpha, currentTurn == WHITE ? BLACK : WHITE, true);
        } else {
            // Try reduced depth first (LMR)
            eval = -negamax(board, nextDepth - reduction, ply + 1, -alpha - 1, -alpha, currentTurn == WHITE ? BLACK : WHITE, true);
            // Re-search at full depth if it looks promising
            if (eval > alpha && (reduction > 0 || eval < beta)) {
                eval = -negamax(board, nextDepth, ply + 1, -beta, -alpha, currentTurn == WHITE ? BLACK : WHITE, true);
            }
        }
        
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