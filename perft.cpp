#include "perft.h"
#include <iostream>
#include <chrono>
#include <vector>

long long perft(Board& board, Color turn, int depth) {
    if (depth == 0) return 1;
    
    long long nodes = 0;
    std::vector<Move> moves = board.generateLegalMoves(turn);
    
    for (const auto& move : moves) {
        GameState prevState = board.gameState;
        Piece captured = board.getPiece(move.toX, move.toY);
        board.makeMove(move);
        
        nodes += perft(board, turn == WHITE ? BLACK : WHITE, depth - 1);
        
        board.undoMove(move, captured, prevState);
    }
    return nodes;
}

void testPosition(const std::string& fen, int depth, long long expectedNodes) {
    Board board;
    Color turn = board.loadFEN(fen);
    
    auto start = std::chrono::high_resolution_clock::now();
    long long nodes = perft(board, turn, depth);
    auto end = std::chrono::high_resolution_clock::now();
    
    double duration = std::chrono::duration<double>(end - start).count();
    
    std::cout << "FEN: " << fen << std::endl;
    std::cout << "Depth: " << depth << " | Nodes: " << nodes;
    if (nodes == expectedNodes) {
        std::cout << " [PASS]";
    } else {
        std::cout << " [FAIL! Expected " << expectedNodes << "]";
    }
    std::cout << " | Time: " << duration << "s" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
}

void runPerftSuite() {
    std::cout << "--- Starting PERFT Suite ---" << std::endl;
    
    // Position 1: Initial
    testPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281);
    
    // Position 2: Kiwipete
    testPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 97862);
    
    // Position 3: Endgames / edge cases
    testPosition("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4, 43238);
    
    std::cout << "--- PERFT Suite Finished ---" << std::endl;
}
