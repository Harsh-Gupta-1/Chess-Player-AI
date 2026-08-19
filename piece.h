#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <string>

#include <cstdint>

typedef uint64_t Bitboard;

enum Color { WHITE, BLACK };
enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

constexpr Bitboard setBit(int sq) { return 1ULL << sq; }
constexpr Bitboard clearBit(int sq) { return ~(1ULL << sq); }


struct Piece {
    PieceType type;
    Color color;
    Piece(PieceType t = EMPTY, Color c = WHITE) : type(t), color(c) {}
};

struct Move {
    int fromX, fromY, toX, toY;
    PieceType promotion;
    bool isEnPassant;
    bool isCastle;
    
    Move() : fromX(0), fromY(0), toX(0), toY(0), promotion(EMPTY), isEnPassant(false), isCastle(false) {}
    Move(int fx, int fy, int tx, int ty, PieceType prom = EMPTY, bool enPass = false, bool castle = false) 
        : fromX(fx), fromY(fy), toX(tx), toY(ty), promotion(prom), isEnPassant(enPass), isCastle(castle) {}
};

struct GameState {
    bool whiteCanCastleKingside;
    bool whiteCanCastleQueenside;
    bool blackCanCastleKingside;
    bool blackCanCastleQueenside;
    int enPassantX, enPassantY;
    bool hasEnPassant;
    unsigned long long zobristKey;
    unsigned long long pawnKey;
    int halfmoveClock;
    int fullmoveNumber;
    int mgScore;
    int egScore;
    
    GameState() : whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
                  blackCanCastleKingside(true), blackCanCastleQueenside(true),
                  enPassantX(-1), enPassantY(-1), hasEnPassant(false), zobristKey(0), pawnKey(0),
                  halfmoveClock(0), fullmoveNumber(1), mgScore(0), egScore(0) {}
};

#endif // PIECE_H