#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <string>
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

    Bitboard pieces[7]; // 1=PAWN, 2=KNIGHT, 3=BISHOP, 4=ROOK, 5=QUEEN, 6=KING
    Bitboard colors[2]; // 0=WHITE, 1=BLACK
    Piece pieceList[64];

    static const int MG_VALUE[7]; 
    static const int EG_VALUE[7];
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

    GameState gameState;
    unsigned long long history[1024];
    int historyPly;
    
    std::pair<int, int> kingPos[2]; 
    int pieceCount[2][7]; 
    
    struct PawnEntry {
        unsigned long long key;
        int mgScore;
        int egScore;
        bool valid;
        PawnEntry() : key(0), mgScore(0), egScore(0), valid(false) {}
    };
    PawnEntry pawnTable[16384];

    Board();
    void setupBoard();
    Color loadFEN(const std::string& fen);
    void printBoard();
    
    Piece getPiece(int x, int y) const;
    Piece getPiece(int sq) const;
    
    bool isSquareUnderAttack(int sq, Color byColor) const;
    bool isSquareUnderAttack(int x, int y, Color byColor) const;
    
    bool isInCheck(Color color) const;
    
    void makeMove(const Move& m);
    void undoMove(const Move& m, const Piece& captured, const GameState& prevState);
    
    void generateMoves(Color color, MoveList& moves);
    void generateLegalMoves(Color color, MoveList& legalMoves);
    
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool isRepetition() const;
    bool isInsufficientMaterial();
    bool isDraw();
    bool hasNonPawnMaterial(Color color) const;
    
    int evaluate();

private:
    void updateGameState(const Move& m, const Piece& movingPiece);
    void initCache();
    std::pair<int, int> evaluatePawnStructure();
    
    // Internal bitboard helpers
    void generatePawnMoves(Color color, MoveList& moves, Bitboard target);
    void generateKnightMoves(Color color, MoveList& moves, Bitboard target);
    void generateBishopMoves(Color color, MoveList& moves, Bitboard target);
    void generateRookMoves(Color color, MoveList& moves, Bitboard target);
    void generateQueenMoves(Color color, MoveList& moves, Bitboard target);
    void generateKingMoves(Color color, MoveList& moves, Bitboard target);
};

// Attack tables
namespace Attacks {
    void init();
    extern Bitboard knightAttacks[64];
    extern Bitboard kingAttacks[64];
    extern Bitboard pawnAttacks[2][64];
    
    Bitboard getRayAttacks(int sq, int dir, Bitboard occupied);
    Bitboard getBishopAttacks(int sq, Bitboard occupied);
    Bitboard getRookAttacks(int sq, Bitboard occupied);
    Bitboard getQueenAttacks(int sq, Bitboard occupied);
}

#endif // BOARD_H
