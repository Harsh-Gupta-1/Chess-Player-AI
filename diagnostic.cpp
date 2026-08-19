#include "board.h"
#include "chess_ai.h"
#include "zobrist.h"
#include <iostream>

int main() {
    Board board;
    ChessAI ai;
    ai.timeLimitMs = 10000000; // no time limit
    
    // Test 1: Evaluate starting position
    std::cout << "=== DIAGNOSTIC TEST ===" << std::endl;
    int startEval = board.evaluate();
    std::cout << "Starting position eval (should be ~0): " << startEval << std::endl;
    
    // Test 2: Check zobrist consistency
    unsigned long long hash1 = board.gameState.zobristKey;
    unsigned long long hash2 = Zobrist::computeHash(board, WHITE);
    std::cout << "Zobrist incremental: " << hash1 << std::endl;
    std::cout << "Zobrist computed:    " << hash2 << std::endl;
    std::cout << "Zobrist match: " << (hash1 == hash2 ? "YES" : "NO - BUG!") << std::endl;
    
    // Test 3: Make a move and check zobrist
    std::vector<Move> moves = board.generateLegalMoves(WHITE);
    if (!moves.empty()) {
        Move e2e4(1, 4, 3, 4); // e2 to e4
        // Find actual e2e4 move
        for (const Move& m : moves) {
            if (m.fromX == 1 && m.fromY == 4 && m.toX == 3 && m.toY == 4) {
                GameState prev = board.gameState;
                Piece cap = board.getPiece(m.toX, m.toY);
                board.makeMove(m);
                
                unsigned long long afterMoveInc = board.gameState.zobristKey;
                unsigned long long afterMoveComp = Zobrist::computeHash(board, BLACK);
                std::cout << "\nAfter e2e4:" << std::endl;
                std::cout << "Zobrist incremental: " << afterMoveInc << std::endl;
                std::cout << "Zobrist computed:    " << afterMoveComp << std::endl;
                std::cout << "Zobrist match: " << (afterMoveInc == afterMoveComp ? "YES" : "NO - BUG!") << std::endl;
                
                board.undoMove(m, cap, prev);
                
                unsigned long long afterUndoInc = board.gameState.zobristKey;
                std::cout << "\nAfter undo:" << std::endl;
                std::cout << "Zobrist matches original: " << (afterUndoInc == hash1 ? "YES" : "NO - BUG!") << std::endl;
                break;
            }
        }
    }
    
    // Test 4: AI best move from starting position
    std::cout << "\n=== AI TEST (depth 4) ===" << std::endl;
    Move best = ai.getBestMove(board, WHITE, 4);
    std::cout << "Best move: " << (char)('a' + best.fromY) << (best.fromX + 1)
              << (char)('a' + best.toY) << (best.toX + 1) << std::endl;
    std::cout << "Nodes explored: " << ai.nodesExplored << std::endl;
    
    // Test 5: Eval after white's best move
    {
        GameState prev = board.gameState;
        Piece cap = board.getPiece(best.toX, best.toY);
        board.makeMove(best);
        int evalAfterWhite = board.evaluate();
        std::cout << "Eval after white move (should be positive for white): " << evalAfterWhite << std::endl;
        board.undoMove(best, cap, prev);
    }
    
    // Test 6: Check negamax sign - does it maximize correctly?
    std::cout << "\n=== NEGAMAX SIGN TEST ===" << std::endl;
    board.setupBoard();
    // Remove black queen to make white obviously winning
    // We'll use FEN: rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    Color turn = board.loadFEN("rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int queenDownEval = board.evaluate();
    std::cout << "Eval with black queen removed (should be VERY positive ~+900): " << queenDownEval << std::endl;
    
    // Now, does the AI playing WHITE return a move that maintains this advantage?
    Move bestWhite = ai.getBestMove(board, WHITE, 4);
    std::cout << "AI (white, winning) plays: " << (char)('a' + bestWhite.fromY) << (bestWhite.fromX + 1)
              << (char)('a' + bestWhite.toY) << (bestWhite.toX + 1) << std::endl;
    
    // Test 7: Same position but AI plays BLACK (should play defensively)
    turn = board.loadFEN("rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    Move bestBlack = ai.getBestMove(board, BLACK, 4);
    std::cout << "AI (black, losing) plays: " << (char)('a' + bestBlack.fromY) << (bestBlack.fromX + 1)
              << (char)('a' + bestBlack.toY) << (bestBlack.toX + 1) << std::endl;
    
    std::cout << "\n=== ALL TESTS COMPLETE ===" << std::endl;
    return 0;
}
