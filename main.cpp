#include "game.h"
#include "benchmark.h"
#include "perft.h"
#include "uci.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "perft") {
        runPerftSuite();
        return 0;
    }

    if (argc > 1 && std::string(argv[1]) == "bench") {
        benchmark(argc, argv);
        return 0;
    }
    
    if (argc > 1 && std::string(argv[1]) == "uci") {
        UCI::loop();
        return 0;
    }
    
    if (argc > 1 && std::string(argv[1]) == "play") {
        std::cout << "Starting a new game..." << std::endl;
        std::cout << "(Use './chess perft' or './chess bench' to run tests)" << std::endl;
        Game g;
        g.play();
        return 0;
    }
    
    // Default to UCI mode if no arguments are provided.
    // This is required because GUIs like Arena and CuteChess 
    // launch the executable with no arguments and immediately send "uci" via stdin.
    UCI::loop();
    return 0;
}