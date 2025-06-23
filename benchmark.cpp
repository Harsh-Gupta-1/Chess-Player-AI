#include "benchmark.h"
#include "board.h"
#include "chess_ai.h"
#include <iostream>
#include <chrono>
#include <iomanip>

void benchmark() {
    ChessAI ai;
    std::vector<int> depths = {2, 3, 4, 5};
    int moveCount = 10;

    std::cout << std::left << std::setw(10) << "Depth"
              << std::setw(20) << "Avg Time (s)"
              << std::setw(25) << "Avg Nodes Searched" << std::endl;

    for (int depth : depths) {
        double totalTime = 0.0;
        long long totalNodes = 0;

        for (int i = 0; i < moveCount; i++) {
            Board board;
            Color aiColor = WHITE;

            auto start = std::chrono::high_resolution_clock::now();
            Move best = ai.getBestMove(board, aiColor, depth);
            auto end = std::chrono::high_resolution_clock::now();

            double duration = std::chrono::duration<double>(end - start).count();
            totalTime += duration;
            totalNodes += ai.nodesExplored;
        }

        std::cout << std::left << std::setw(10) << depth
                  << std::setw(20) << totalTime / moveCount
                  << std::setw(25) << totalNodes / moveCount << std::endl;
    }
}