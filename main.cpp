#include "game.h"
#include "benchmark.h"
#include <iostream>

int main() {
    // Uncomment the following line to run the benchmark
    // benchmark();
    
    std::cout << "Starting a new game..." << std::endl;
    Game g;
    g.play();
    return 0;
}