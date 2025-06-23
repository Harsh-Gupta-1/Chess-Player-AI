#include "game.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <limits>

void Game::play() {
    while (true) {
        board.printBoard();
        
        if (board.isCheckmate(WHITE)) {
            std::cout << "Black wins by checkmate!" << std::endl;
            break;
        } else if (board.isCheckmate(BLACK)) {
            std::cout << "White wins by checkmate!" << std::endl;
            break;
        } else if (board.isStalemate(WHITE) || board.isStalemate(BLACK)) {
            std::cout << "Draw by stalemate!" << std::endl;
            break;
        } else if (board.isDraw()) {
            std::cout << "Draw by insufficient material!" << std::endl;
            break;
        }
        
        if (board.isInCheck(WHITE)) std::cout << "White is in check!" << std::endl;
        if (board.isInCheck(BLACK)) std::cout << "Black is in check!" << std::endl;
        
        if (humanColor == WHITE) {
            playerTurn(WHITE);
            board.printBoard();
            aiTurn(BLACK);
        } else {
            aiTurn(WHITE);
            board.printBoard();
            playerTurn(BLACK);
        }
    }
}

void Game::clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Game::parseMove(const std::string& input, std::string& from, std::string& to, std::string& promotion) {
    std::istringstream iss(input);
    std::string token;
    std::vector<std::string> tokens;
    
    // Split input by spaces
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 2 || tokens.size() > 3) {
        return false;
    }
    
    from = tokens[0];
    to = tokens[1];
    promotion = (tokens.size() == 3) ? tokens[2] : "";
    
    return true;
}

void Game::playerTurn(Color color) {
    while (true) {
        std::string from, to, promotion = "";
        std::string input;
        
        std::cout << "Your move (e.g. e2 e4, or e7 e8 Q for promotion, or 'quit' to exit): ";
        
        // Clear any previous input errors
        if (std::cin.fail()) {
            clearInputStream();
        }
        
        // Get the entire line to handle any input gracefully
        if (!std::getline(std::cin, input)) {
            std::cout << "Error reading input. Please try again.\n";
            clearInputStream();
            continue;
        }
        
        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
        input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
        
        // Check for quit command
        if (input == "quit" || input == "exit") {
            std::cout << "Game terminated by player.\n";
            exit(0);
        }
        
        // Handle empty input
        if (input.empty()) {
            std::cout << "Please enter a move.\n";
            continue;
        }
        
        // Parse the move
        if (!parseMove(input, from, to, promotion)) {
            std::cout << "Invalid input format. Please use format: 'e2 e4' or 'e7 e8 Q'\n";
            continue;
        }
        
        // Validate square notation
        if (from.length() != 2 || to.length() != 2 ||
            from[0] < 'a' || from[0] > 'h' || to[0] < 'a' || to[0] > 'h' ||
            from[1] < '1' || from[1] > '8' || to[1] < '1' || to[1] > '8') {
            std::cout << "Invalid square notation. Use letters a-h and numbers 1-8 (e.g. e2, d4).\n";
            continue;
        }

        int fx = from[1] - '1';
        int fy = from[0] - 'a';
        int tx = to[1] - '1';
        int ty = to[0] - 'a';

        PieceType promoPiece = EMPTY;
        if (!promotion.empty()) {
            if (promotion.length() != 1) {
                std::cout << "Promotion piece must be a single character (Q, R, B, or N).\n";
                continue;
            }
            
            switch (toupper(promotion[0])) {
                case 'Q': promoPiece = QUEEN; break;
                case 'R': promoPiece = ROOK; break;
                case 'B': promoPiece = BISHOP; break;
                case 'N': promoPiece = KNIGHT; break;
                default:
                    std::cout << "Invalid promotion piece. Use Q (Queen), R (Rook), B (Bishop), or N (Knight).\n";
                    continue;
            }
        }

        std::vector<Move> legalMoves = board.generateLegalMoves(color);
        bool found = false;

        for (const auto& move : legalMoves) {
            if (move.fromX == fx && move.fromY == fy &&
                move.toX == tx && move.toY == ty &&
                move.promotion == promoPiece) {
                board.makeMove(move);
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "Invalid move. This move is not legal in the current position.\n";
            std::cout << "Make sure:\n";
            std::cout << "  - You're moving your own piece\n";
            std::cout << "  - The move follows the piece's movement rules\n";
            std::cout << "  - The move doesn't put your king in check\n";
        } else {
            break;
        }
    }
}

void Game::aiTurn(Color color) {
    std::cout << "AI is thinking..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        Move best = ai.getBestMove(board, color, 4);
        auto end = std::chrono::high_resolution_clock::now();
        double moveTime = std::chrono::duration<double>(end - start).count();
        std::cout << "AI took " << moveTime << " seconds to decide the move.\n";
        
        if (best.fromX == 0 && best.fromY == 0 && best.toX == 0 && best.toY == 0) {
            std::cout << "AI has no legal moves!" << std::endl;
            return;
        }
        
        board.makeMove(best);
        
        std::cout << "AI played: " << char('a' + best.fromY) << best.fromX + 1 
                  << " to " << char('a' + best.toY) << best.toX + 1;
        
        if (best.promotion != EMPTY) {
            char promoChar = 'Q';
            switch (best.promotion) {
                case ROOK: promoChar = 'R'; break;
                case BISHOP: promoChar = 'B'; break;
                case KNIGHT: promoChar = 'N'; break;
                default: break;
            }
            std::cout << " (" << promoChar << ")";
        }
        
        if (best.isCastle) {
            std::cout << " (Castle)";
        }
        
        if (best.isEnPassant) {
            std::cout << " (En Passant)";
        }
        
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error in AI calculation: " << e.what() << std::endl;
        std::cout << "AI forfeits the game." << std::endl;
    }
}