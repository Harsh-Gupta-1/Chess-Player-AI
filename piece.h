#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <string>

enum Color { WHITE, BLACK };
enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

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
    
    GameState() : whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
                  blackCanCastleKingside(true), blackCanCastleQueenside(true),
                  enPassantX(-1), enPassantY(-1), hasEnPassant(false) {}
};

#endif // PIECE_H