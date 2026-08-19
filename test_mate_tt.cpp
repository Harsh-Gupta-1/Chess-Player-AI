#include "board.h"
#include "chess_ai.h"
#include <iostream>
#include <cassert>

void run_tt_mate_tests() {
    std::cout << "--- Starting TT Mate Score Regression Tests ---" << std::endl;
    
    // 1. Test basic TT Mate Score normalization invariant.
    ChessAI ai;
    Board board;
    
    // Let's manually store a mate score in the TT at ply 5.
    // Say we found a forced mate in 2. The score returned by the evaluation would be 10000 - mate_ply.
    // If mate happens at ply 7, mate_ply = 7. 
    // Score returned is 10000 - 7 = 9993.
    // When we store this at current ply = 5, TT should normalize it to 9993 + 5 = 9998.
    
    unsigned long long testKey = 123456789ULL;
    Move dummyMove(0,0,0,0);
    
    // store(key, depth, ply, score, bound, bestMove)
    bool collision;
    ai.tt.store(testKey, 10, 5, 9993, EXACT, dummyMove, collision);
    
    // Now let's retrieve it from ply 2.
    // We probe at ply 2. TT should reconstruct the score:
    // score = 9998 - 2 = 9996. (Which represents mate at ply 4 from the new root, meaning mate in 2 plies).
    int returnScore = 0;
    Move returnMove(0,0,0,0);
    bool hit;
    ai.tt.probe(testKey, 5, 2, -10000, 10000, returnScore, returnMove, hit);
    
    std::cout << "Test 1 [Winning Mate Normalization]: ";
    if (hit && returnScore == 9996) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL (Expected 9996, Got " << returnScore << ")" << std::endl;
        assert(false);
    }
    
    // 2. Test Losing Mate Normalization
    // Engine finds it gets mated at ply 8. Score = -10000 + 8 = -9992.
    // We are at ply 3. TT stores: -9992 - 3 = -9995.
    ai.tt.store(testKey + 1, 10, 3, -9992, EXACT, dummyMove, collision);
    
    // Retrieve at ply 6.
    // TT reconstructs: -9995 + 6 = -9989 (meaning mate at ply 11 from the new root. 11 - 6 = 5 plies to mate. Which matches 8 - 3 = 5 plies to mate).
    ai.tt.probe(testKey + 1, 5, 6, -10000, 10000, returnScore, returnMove, hit);
    
    std::cout << "Test 2 [Losing Mate Normalization]: ";
    if (hit && returnScore == -9989) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL (Expected -9989, Got " << returnScore << ")" << std::endl;
        assert(false);
    }
    
    // FEN: k7/1R6/2P5/8/8/8/8/2R3K1 w - - 0 1
    // White plays Rc8#
    Board boardMateIn1;
    boardMateIn1.loadFEN("k7/1R6/2P5/8/8/8/8/2R3K1 w - - 0 1");
    
    ChessAI aiMate;
    aiMate.timeLimitMs = 10000;
    
    std::cout << "Test 3 [Iterative Deepening Mate in 1]: ";
    Move m = aiMate.getBestMove(boardMateIn1, WHITE, 3);
    
    std::cout << "Debug: Best move chosen is: " << m.fromX << "," << m.fromY << " -> " << m.toX << "," << m.toY << std::endl;
    
    // Expect 9999 (Mate in 1 from root)
    if ((m.fromX == 0 && m.fromY == 2 && m.toX == 7 && m.toY == 2) || 
        (m.fromX == 0 && m.fromY == 2 && m.toX == 0 && m.toY == 0)) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL (Move was: fromX=" << m.fromX << " fromY=" << m.fromY << " toX=" << m.toX << " toY=" << m.toY << ")" << std::endl;
        assert(false);
    }
    
    std::cout << "--- All TT Mate Regression Tests Passed ---" << std::endl;
}

int main() {
    run_tt_mate_tests();
    return 0;
}
