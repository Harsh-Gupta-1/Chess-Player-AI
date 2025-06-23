#include "chess_ai.h"
#include <iostream>
#include <algorithm>

Move ChessAI::getBestMove(Board& board, Color aiColor, int depth) {
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
    if (depth == 0) return board.evaluate();
    
    if (board.isCheckmate(currentTurn)) {
        return maximizing ? -10000 : 10000;
    }
    
    if (board.isStalemate(currentTurn) || board.isDraw()) {
        return 0;
    }

    std::vector<Move> moves = board.generateLegalMoves(currentTurn);
    
    if (maximizing) {
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
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
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
            if (beta <= alpha) break;
        }
        return minEval;
    }
}