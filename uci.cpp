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
            int depth = 4;
            long long movetime = 1000;
            bool useTime = false;
            
            std::string arg;
            while (iss >> arg) {
                if (arg == "depth") iss >> depth;
                else if (arg == "movetime") { iss >> movetime; useTime = true; }
                else if (arg == "wtime" || arg == "btime") {
                    long long timeRemaining;
                    iss >> timeRemaining;
                    movetime = timeRemaining / 30; // 1/30th of remaining time
                    useTime = true;
                }
            }
            
            if (useTime) {
                ai.timeLimitMs = movetime;
                depth = 100; // Search until time out
            } else {
                ai.timeLimitMs = 10000000; // effectively infinite
            }
            
            Move best = ai.getBestMove(board, turn, depth);
            std::cout << "bestmove " << moveToString(best) << std::endl;
        }
        else if (command == "quit") {
            break;
        }
    }
}
