#include "game.h"
#include "benchmark.h"
#include "perft.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "perft") {
        runPerftSuite();
        return 0;
    }

    if (argc > 1 && std::string(argv[1]) == "bench") {
        benchmark();
        return 0;
    }
    
    std::cout << "Starting a new game..." << std::endl;
    std::cout << "(Use './chess perft' or './chess bench' to run tests)" << std::endl;
    Game g;
    g.play();
    return 0;
}