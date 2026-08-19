#ifndef PERFT_H
#define PERFT_H

#include "board.h"

long long perft(Board& board, Color turn, int depth);
void runPerftSuite();

#endif // PERFT_H
