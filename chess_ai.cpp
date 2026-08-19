#include "chess_ai.h"
#include <iostream>
#include <algorithm>

Move ChessAI::getBestMove(Board& board, Color aiColor, int depth) {
    tt.clear();
    nodesExplored = 0;
    int bestScore = (aiColor == WHITE) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    Move bestMove(0, 0, 0, 0);
    
    std::vector<Move> legalMoves = board.generateLegalMoves(aiColor);
    if (legalMoves.empty()) return bestMove;

    for (const auto& move : legalMoves) {
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        int score = minimax(board, depth - 1, std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), aiColor == WHITE ? BLACK : WHITE, aiColor == BLACK);
        
        board.undoMove(move, captured, prevState);

        if ((aiColor == WHITE && score > bestScore) || (aiColor == BLACK && score < bestScore)) {
            bestScore = score;
            bestMove = move;
        }
    }
    std::cout << "Nodes explored: " << nodesExplored << std::endl;
    return bestMove;
}

int ChessAI::minimax(Board& board, int depth, int alpha, int beta, Color currentTurn, bool maximizing) {
    nodesExplored++;
    
    int originalAlpha = alpha;
    unsigned long long hashKey = Zobrist::computeHash(board, currentTurn);
    int ttScore;
    Move ttMove(0,0,0,0);
    
    if (tt.probe(hashKey, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }
    
    if (depth == 0) return board.evaluate();
    
    if (board.isCheckmate(currentTurn)) {
        return maximizing ? -10000 : 10000;
    }
    
    if (board.isStalemate(currentTurn) || board.isDraw()) {
        return 0;
    }

    std::vector<Move> moves = board.generateLegalMoves(currentTurn);
    
    if (maximizing) {
        Move bestMoveForTT(0,0,0,0);
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int eval = minimax(board, depth - 1, alpha, beta, 
                             currentTurn == WHITE ? BLACK : WHITE, false);
            
            board.undoMove(move, captured, prevState);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (alpha > originalAlpha) bestMoveForTT = move;
            if (beta <= alpha) break;
        }
        
        Bound bound = EXACT;
        if (maxEval <= originalAlpha) bound = UPPER_BOUND;
        else if (maxEval >= beta) bound = LOWER_BOUND;
        tt.store(hashKey, depth, maxEval, bound, bestMoveForTT);
        
        return maxEval;
    } else {
        Move bestMoveForTT(0,0,0,0);
        int originalBeta = beta;
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int eval = minimax(board, depth - 1, alpha, beta, 
                             currentTurn == WHITE ? BLACK : WHITE, true);
            
            board.undoMove(move, captured, prevState);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta < originalBeta) bestMoveForTT = move;
            if (beta <= alpha) break;
        }
        
        Bound bound = EXACT;
        if (minEval <= alpha) bound = UPPER_BOUND;
        else if (minEval >= originalBeta) bound = LOWER_BOUND;
        tt.store(hashKey, depth, minEval, bound, bestMoveForTT);
        
        return minEval;
    }
}