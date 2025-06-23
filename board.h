#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <vector>

class Board {
private:
    static const int BOARD_SIZE = 8;
    static const int PAWN_TABLE[8][8];
    static const int KNIGHT_TABLE[8][8];
    static const int BISHOP_TABLE[8][8];
    static const int KING_TABLE[8][8];
    std::vector<std::vector<Piece>> board;
    int evaluateMobility();
    int evaluatePawnStructure();

public:
    GameState gameState;

    Board();
    void setupBoard();
    void printBoard();
    bool isInBounds(int x, int y) const;
    Piece getPiece(int x, int y) const;
    std::pair<int, int> findKing(Color color) const;
    bool isSquareUnderAttack(int x, int y, Color byColor) const;
    bool isInCheck(Color color) const;
    void makeMove(const Move& m);
    void undoMove(const Move& m, const Piece& captured, const GameState& prevState);
    void updateGameState(const Move& m, const Piece& movingPiece);
    std::vector<Move> generateMoves(Color color);
    void generatePawnMoves(int x, int y, Color color, std::vector<Move>& moves);
    void generateKnightMoves(int x, int y, Color color, std::vector<Move>& moves);
    void generateBishopMoves(int x, int y, Color color, std::vector<Move>& moves);
    void generateRookMoves(int x, int y, Color color, std::vector<Move>& moves);
    void generateQueenMoves(int x, int y, Color color, std::vector<Move>& moves);
    void generateKingMoves(int x, int y, Color color, std::vector<Move>& moves);
    std::vector<Move> generateLegalMoves(Color color);
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool isDraw();
    int evaluate();
};

#endif // BOARD_H