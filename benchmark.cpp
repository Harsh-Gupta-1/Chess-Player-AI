#include "benchmark.h"
#include "board.h"
#include "chess_ai.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>

struct BenchmarkPosition {
    std::string name;
    std::string fen;
};

void benchmark() {
    ChessAI ai;
    std::vector<BenchmarkPosition> positions = {
        {"Start Position", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        {"Kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"},
        {"Middlegame", "r1bqk2r/pppp1ppp/2n2n2/2b1p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 6 5"},
        {"Endgame", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"}
    };
    
    int depth = 4;
    long long totalNodes = 0;
    double totalTime = 0.0;

    std::cout << "--- Starting Search Benchmark ---" << std::endl;
    std::cout << "Search Depth: " << depth << std::endl << std::endl;
    
    std::cout << std::left << std::setw(20) << "Position"
              << std::setw(15) << "Nodes"
              << std::setw(15) << "Time (s)"
              << std::setw(15) << "NPS" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (const auto& pos : positions) {
        Board board;
        Color turn = board.loadFEN(pos.fen);
        
        auto start = std::chrono::high_resolution_clock::now();
        Move best = ai.getBestMove(board, turn, depth);
        auto end = std::chrono::high_resolution_clock::now();
        
        double duration = std::chrono::duration<double>(end - start).count();
        long long nodes = ai.nodesExplored;
        
        totalNodes += nodes;
        totalTime += duration;
        
        double nps = nodes / duration;
        
        std::cout << std::left << std::setw(20) << pos.name
                  << std::setw(15) << nodes
                  << std::setw(15) << std::fixed << std::setprecision(4) << duration
                  << std::setw(15) << static_cast<long long>(nps) << std::endl;
    }

    std::cout << std::string(65, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "TOTAL"
              << std::setw(15) << totalNodes
              << std::setw(15) << totalTime
              << std::setw(15) << static_cast<long long>(totalNodes / totalTime) << " (Avg NPS)" << std::endl;
    std::cout << "---------------------------------" << std::endl;
}