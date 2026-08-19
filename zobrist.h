#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "piece.h"

class Board;

namespace Zobrist {
    extern unsigned long long pieceKeys[2][7][64];
    extern unsigned long long enPassantKeys[8];
    extern unsigned long long castleKeys[16];
    extern unsigned long long sideKey;

    void init();
    unsigned long long computeHash(const Board& board, Color turn);
}

#endif // ZOBRIST_H
