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

void benchmark(int argc, char* argv[]) {
    ChessAI ai;
    ai.timeLimitMs = 1000000; // Disable time limit for benchmark testing
    
    // Parse ablation flags
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-no-null") ai.enableNullMove = false;
        if (arg == "-no-lmr") ai.enableLMR = false;
        if (arg == "-no-killer") ai.enableKiller = false;
        if (arg == "-no-history") ai.enableHistory = false;
    }

    
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

    ChessAI::SearchStats totalStats;
    totalStats.clear();

    for (const auto& pos : positions) {
        Board board;
        Color turn = board.loadFEN(pos.fen);
        
        auto start = std::chrono::high_resolution_clock::now();
        (void)ai.getBestMove(board, turn, depth);
        auto end = std::chrono::high_resolution_clock::now();
        
        double duration = std::chrono::duration<double>(end - start).count();
        long long nodes = ai.nodesExplored;
        
        totalNodes += nodes;
        totalTime += duration;
        
        totalStats.qNodes += ai.stats.qNodes;
        totalStats.betaCutoffs += ai.stats.betaCutoffs;
        totalStats.firstMoveCutoffs += ai.stats.firstMoveCutoffs;
        totalStats.ttProbes += ai.stats.ttProbes;
        totalStats.ttHits += ai.stats.ttHits;
        totalStats.ttUsableHits += ai.stats.ttUsableHits;
        totalStats.ttCutoffs += ai.stats.ttCutoffs;
        totalStats.ttStores += ai.stats.ttStores;
        totalStats.ttCollisions += ai.stats.ttCollisions;
        totalStats.pvsSearches += ai.stats.pvsSearches;
        totalStats.pvsResearches += ai.stats.pvsResearches;
        totalStats.lmrAttempts += ai.stats.lmrAttempts;
        totalStats.lmrReductions += ai.stats.lmrReductions;
        totalStats.lmrResearches += ai.stats.lmrResearches;
        totalStats.nullAttempts += ai.stats.nullAttempts;
        totalStats.nullCutoffs += ai.stats.nullCutoffs;
        totalStats.killerHits += ai.stats.killerHits;
        totalStats.historyHits += ai.stats.historyHits;
        
        double nps = nodes / duration;
        
        std::cout << std::left << std::setw(20) << pos.name
                  << std::setw(15) << nodes
                  << std::setw(15) << std::fixed << std::setprecision(4) << duration
                  << std::setw(15) << static_cast<long long>(nps) << std::endl;
    }

    std::cout << std::string(65, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "TOTAL"
              << std::setw(15) << totalNodes
              << std::setw(15) << std::fixed << std::setprecision(4) << totalTime
              << std::setw(15) << static_cast<long long>(totalNodes / totalTime) << "  (Avg NPS)" << std::endl;
    std::cout << std::string(65, '-') << std::endl;
    
    // Programmatic telemetry block
    std::cout << "\n[TELEMETRY]" << std::endl;
    std::cout << "Nodes: " << totalNodes << std::endl;
    std::cout << "QNodes: " << totalStats.qNodes << std::endl;
    std::cout << "BetaCutoffs: " << totalStats.betaCutoffs << std::endl;
    std::cout << "FirstMoveCutoffs: " << totalStats.firstMoveCutoffs << std::endl;
    std::cout << "TTProbes: " << totalStats.ttProbes << std::endl;
    std::cout << "TTHits: " << totalStats.ttHits << std::endl;
    std::cout << "TTUsableHits: " << totalStats.ttUsableHits << std::endl;
    std::cout << "TTCutoffs: " << totalStats.ttCutoffs << std::endl;
    std::cout << "TTStores: " << totalStats.ttStores << std::endl;
    std::cout << "TTCollisions: " << totalStats.ttCollisions << std::endl;
    std::cout << "PVSSearches: " << totalStats.pvsSearches << std::endl;
    std::cout << "PVSResearches: " << totalStats.pvsResearches << std::endl;
    std::cout << "LMRAttempts: " << totalStats.lmrAttempts << std::endl;
    std::cout << "LMRReductions: " << totalStats.lmrReductions << std::endl;
    std::cout << "LMRResearches: " << totalStats.lmrResearches << std::endl;
    std::cout << "NullAttempts: " << totalStats.nullAttempts << std::endl;
    std::cout << "NullCutoffs: " << totalStats.nullCutoffs << std::endl;
    std::cout << "KillerHits: " << totalStats.killerHits << std::endl;
    std::cout << "HistoryHits: " << totalStats.historyHits << std::endl;
    std::cout << "[/TELEMETRY]" << std::endl;
}