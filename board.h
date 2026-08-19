#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <vector>

class Board {
private:
    static const int BOARD_SIZE = 8;
    static const int MG_VALUE[6]; // EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN (KING not used for material)
    static const int EG_VALUE[6];
    
    static const int PAWN_MG[8][8];
    static const int PAWN_EG[8][8];
    static const int KNIGHT_MG[8][8];
    static const int KNIGHT_EG[8][8];
    static const int BISHOP_MG[8][8];
    static const int BISHOP_EG[8][8];
    static const int ROOK_MG[8][8];
    static const int ROOK_EG[8][8];
    static const int QUEEN_MG[8][8];
    static const int QUEEN_EG[8][8];
    static const int KING_MG[8][8];
    static const int KING_EG[8][8];
    
    std::vector<std::vector<Piece>> board;
    int evaluateMobility();
    std::pair<int, int> evaluatePawnStructure();

public:
    GameState gameState;
    unsigned long long history[1024];
    int historyPly;

    Board();
    bool isRepetition() const;
    bool isInsufficientMaterial();
    bool isDraw();
    void setupBoard();
    Color loadFEN(const std::string& fen);
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
    int evaluate();
};

#endif // BOARD_H