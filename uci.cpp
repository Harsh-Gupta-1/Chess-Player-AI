#include "uci.h"
#include <iostream>
#include <sstream>
#include <vector>

std::string UCI::moveToString(const Move& m) {
    std::string s = "";
    s += (char)('a' + m.fromY);
    s += (char)('1' + m.fromX);
    s += (char)('a' + m.toY);
    s += (char)('1' + m.toX);
    if (m.promotion != EMPTY) {
        if (m.promotion == QUEEN) s += 'q';
        else if (m.promotion == ROOK) s += 'r';
        else if (m.promotion == BISHOP) s += 'b';
        else if (m.promotion == KNIGHT) s += 'n';
    }
    return s;
}

Move UCI::parseMove(Board& board, Color color, const std::string& moveStr) {
    std::vector<Move> moves = board.generateLegalMoves(color);
    for (const Move& m : moves) {
        if (moveToString(m) == moveStr) {
            return m;
        }
    }
    return Move(0,0,0,0);
}

void UCI::loop() {
    std::string line;
    Board board;
    ChessAI ai;
    Color turn = WHITE;
    
    std::setvbuf(stdin, NULL, _IONBF, 0);
    std::setvbuf(stdout, NULL, _IONBF, 0);
    
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command == "uci") {
            std::cout << "id name Chess-Player-AI" << std::endl;
            std::cout << "id author Harsh Gupta" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (command == "ucinewgame") {
            ai.tt.clear();
        }
        else if (command == "position") {
            std::string arg;
            iss >> arg;
            if (arg == "startpos") {
                board.setupBoard();
                turn = WHITE;
                iss >> arg;
            } else if (arg == "fen") {
                std::string fen = "", token;
                for (int i=0; i<6; i++) {
                    if (iss >> token) fen += token + (i < 5 ? " " : "");
                }
                turn = board.loadFEN(fen);
                iss >> arg;
            }
            
            if (arg == "moves") {
                std::string moveStr;
                while (iss >> moveStr) {
                    Move m = parseMove(board, turn, moveStr);
                    board.makeMove(m);
                    turn = (turn == WHITE) ? BLACK : WHITE;
                }
            }
        }
        else if (command == "go") {
            int depth = 4; // Default to 4 if absolutely no args are provided
            bool useTime = false;
            long long timeRemaining = 0;
            long long increment = 0;
            long long exactMovetime = 0;
            bool useExactMovetime = false;
            
            std::string arg;
            while (iss >> arg) {
                if (arg == "depth") { iss >> depth; }
                else if (arg == "movetime") { iss >> exactMovetime; useExactMovetime = true; useTime = true; }
                else if (arg == "wtime" && turn == WHITE) { iss >> timeRemaining; useTime = true; }
                else if (arg == "btime" && turn == BLACK) { iss >> timeRemaining; useTime = true; }
                else if (arg == "winc" && turn == WHITE) { iss >> increment; }
                else if (arg == "binc" && turn == BLACK) { iss >> increment; }
                else if (arg == "wtime" || arg == "btime" || arg == "winc" || arg == "binc") {
                    // Ignore opponent's time
                    long long dummy; iss >> dummy;
                }
            }
            
            if (useTime) {
                long long allocatedTime;
                if (useExactMovetime) {
                    allocatedTime = exactMovetime;
                } else {
                    // Allocate 1/30th of remaining time, plus a fraction of the increment
                    allocatedTime = (timeRemaining / 30) + (increment * 3 / 4);
                    
                    // Critical safety threshold: Never use more than remaining time minus a buffer
                    if (allocatedTime > timeRemaining - 50) {
                        allocatedTime = timeRemaining - 50;
                    }
                    if (allocatedTime < 10) allocatedTime = 10;
                }
                
                ai.timeLimitMs = allocatedTime;
                // If depth wasn't explicitly provided via 'go depth X', search as deep as possible within time
                if (depth == 4 && (timeRemaining > 0 || useExactMovetime)) {
                    depth = 64; 
                }
            } else {
                ai.timeLimitMs = 1000000; // Practically infinite if no time limit
            }
            
            Move best = ai.getBestMove(board, turn, depth);
            std::cout << "bestmove " << moveToString(best) << std::endl;
        }
        else if (command == "quit") {
            break;
        }
    }
}
