#include "zobrist.h"
#include "board.h"
#include <random>

namespace Zobrist {
    unsigned long long pieceKeys[2][7][64];
    unsigned long long enPassantKeys[8];
    unsigned long long castleKeys[16];
    unsigned long long sideKey;

    unsigned long long random64() {
        static std::mt19937_64 rng(1337); // Fixed seed for reproducible hashes
        std::uniform_int_distribution<unsigned long long> dist;
        return dist(rng);
    }

    void init() {
        for (int c = 0; c < 2; c++) {
            for (int p = 0; p < 7; p++) {
                for (int s = 0; s < 64; s++) {
                    pieceKeys[c][p][s] = random64();
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            enPassantKeys[i] = random64();
        }
        for (int i = 0; i < 16; i++) {
            castleKeys[i] = random64();
        }
        sideKey = random64();
    }

    unsigned long long computeHash(const Board& board, Color turn) {
        unsigned long long h = 0;
        
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Piece p = board.getPiece(x, y);
                if (p.type != EMPTY) {
                    h ^= pieceKeys[p.color][p.type][x * 8 + y];
                }
            }
        }
        
        int castleState = 0;
        if (board.gameState.whiteCanCastleKingside) castleState |= 1;
        if (board.gameState.whiteCanCastleQueenside) castleState |= 2;
        if (board.gameState.blackCanCastleKingside) castleState |= 4;
        if (board.gameState.blackCanCastleQueenside) castleState |= 8;
        h ^= castleKeys[castleState];
        
        if (board.gameState.hasEnPassant) {
            h ^= enPassantKeys[board.gameState.enPassantY];
        }
        
        if (turn == BLACK) {
            h ^= sideKey;
        }
        
        return h;
    }
}
