#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <vector>

class Board {
public:
    struct MoveList {
        Move moves[256];
        int count;
        MoveList() : count(0) {}
        inline void push_back(const Move& m) { moves[count++] = m; }
        inline Move* begin() { return moves; }
        inline Move* end() { return moves + count; }
        inline int size() const { return count; }
        inline bool empty() const { return count == 0; }
        inline Move& operator[](int i) { return moves[i]; }
        inline const Move& operator[](int i) const { return moves[i]; }
    };

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
    void initCache();
    Color loadFEN(const std::string& fen);
    void printBoard();
    bool isInBounds(int x, int y) const;
    Piece getPiece(int x, int y) const;
    std::pair<int, int> findKing(Color color) const;
    std::pair<int, int> kingPos[2]; // Cached king positions for WHITE and BLACK
    int pieceCount[2][6]; // Caches the number of pieces of each type for each color
    
    struct PawnEntry {
        unsigned long long key;
        int mgScore;
        int egScore;
        bool valid;
        PawnEntry() : key(0), mgScore(0), egScore(0), valid(false) {}
    };
    PawnEntry pawnTable[16384];
    bool isSquareUnderAttack(int x, int y, Color byColor) const;
    bool isInCheck(Color color) const;
    void makeMove(const Move& m);
    void undoMove(const Move& m, const Piece& captured, const GameState& prevState);
    void updateGameState(const Move& m, const Piece& movingPiece);
    void generateMoves(Color color, MoveList& moves);
    void generatePawnMoves(int x, int y, Color color, MoveList& moves);
    void generateKnightMoves(int x, int y, Color color, MoveList& moves);
    void generateBishopMoves(int x, int y, Color color, MoveList& moves);
    void generateRookMoves(int x, int y, Color color, MoveList& moves);
    void generateQueenMoves(int x, int y, Color color, MoveList& moves);
    void generateKingMoves(int x, int y, Color color, MoveList& moves);
    void generateLegalMoves(Color color, MoveList& legalMoves);
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool hasNonPawnMaterial(Color color) const;
    int evaluate();
};

#endif // BOARD_H