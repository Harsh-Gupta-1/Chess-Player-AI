#include "game.h"
#include <iostream>
#include <chrono>

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

void Game::playerTurn(Color color) {
    while (true) {
        std::string from, to, promotion = "";
        std::cout << "Your move (e.g. e2 e4, or e7 e8 Q for promotion): ";
        std::cin >> from >> to;
        
        if (std::cin.peek() == ' ') {
            std::cin >> promotion;
        }

        if (from.length() != 2 || to.length() != 2 ||
            from[0] < 'a' || from[0] > 'h' || to[0] < 'a' || to[0] > 'h' ||
            from[1] < '1' || from[1] > '8' || to[1] < '1' || to[1] > '8') {
            std::cout << "Invalid input format. Try again.\n";
            continue;
        }

        int fx = from[1] - '1';
        int fy = from[0] - 'a';
        int tx = to[1] - '1';
        int ty = to[0] - 'a';

        PieceType promoPiece = EMPTY;
        if (!promotion.empty()) {
            switch (toupper(promotion[0])) {
                case 'Q': promoPiece = QUEEN; break;
                case 'R': promoPiece = ROOK; break;
                case 'B': promoPiece = BISHOP; break;
                case 'N': promoPiece = KNIGHT; break;
                default:
                    std::cout << "Invalid promotion piece. Use Q, R, B, or N.\n";
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
            std::cout << "Invalid move. Try again.\n";
        } else {
            break;
        }
    }
}

void Game::aiTurn(Color color) {
    std::cout << "AI is thinking..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
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
}