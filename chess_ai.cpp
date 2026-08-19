#include "chess_ai.h"
#include <iostream>
#include <algorithm>

Move ChessAI::getBestMove(Board& board, Color aiColor, int maxDepth) {
    tt.clear();
    nodesExplored = 0;
    Move bestMove(0, 0, 0, 0);

    for (int depth = 1; depth <= maxDepth; depth++) {
        int alpha = std::numeric_limits<int>::min() + 1;
        int beta = std::numeric_limits<int>::max() - 1;
        int bestScore = std::numeric_limits<int>::min() + 1;
        
        std::vector<Move> legalMoves = board.generateLegalMoves(aiColor);
        if (legalMoves.empty()) break;
        
        // Very basic move ordering for ID: try TT move first if available
        // In Phase 4 we will fully implement move ordering
        Move currentBestMove(0,0,0,0);

        for (const auto& move : legalMoves) {
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int score = -negamax(board, depth - 1, -beta, -alpha, aiColor == WHITE ? BLACK : WHITE);
            
            board.undoMove(move, captured, prevState);

            if (score > bestScore) {
                bestScore = score;
                currentBestMove = move;
            }
            alpha = std::max(alpha, score);
        }
        
        bestMove = currentBestMove;
        // Check time management here later (Phase 4)
    }
    
    std::cout << "Nodes explored: " << nodesExplored << std::endl;
    return bestMove;
}

int ChessAI::negamax(Board& board, int depth, int alpha, int beta, Color currentTurn) {
    nodesExplored++;
    
    int originalAlpha = alpha;
    unsigned long long hashKey = Zobrist::computeHash(board, currentTurn);
    int ttScore;
    Move ttMove(0,0,0,0);
    
    if (tt.probe(hashKey, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }
    
    if (depth == 0) {
        int eval = board.evaluate();
        return (currentTurn == WHITE) ? eval : -eval;
    }
    
    if (board.isCheckmate(currentTurn)) {
        return -10000;
    }
    
    if (board.isStalemate(currentTurn) || board.isDraw()) {
        return 0;
    }

    std::vector<Move> moves = board.generateLegalMoves(currentTurn);
    if (moves.empty()) return -10000;
    
    int maxEval = std::numeric_limits<int>::min() + 1;
    Move bestMoveForTT(0,0,0,0);
    
    for (const auto& move : moves) {
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        int eval = -negamax(board, depth - 1, -beta, -alpha, currentTurn == WHITE ? BLACK : WHITE);
        
        board.undoMove(move, captured, prevState);
        
        if (eval > maxEval) {
            maxEval = eval;
            bestMoveForTT = move;
        }
        alpha = std::max(alpha, eval);
        if (alpha >= beta) break;
    }
    
    Bound bound = EXACT;
    if (maxEval <= originalAlpha) bound = UPPER_BOUND;
    else if (maxEval >= beta) bound = LOWER_BOUND;
    
    tt.store(hashKey, depth, maxEval, bound, bestMoveForTT);
    
    return maxEval;
}