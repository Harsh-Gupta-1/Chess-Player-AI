#ifndef TT_H
#define TT_H

#include "piece.h"

enum Bound { EXACT, UPPER_BOUND, LOWER_BOUND };

struct TTEntry {
    unsigned long long key;
    int depth;
    int score;
    Bound bound;
    Move bestMove;
    bool valid;

    TTEntry() : key(0), depth(0), score(0), bound(EXACT), bestMove(0,0,0,0), valid(false) {}
};

class TranspositionTable {
private:
    TTEntry* table;
    int size;

public:
    TranspositionTable(int numEntries);
    ~TranspositionTable();

    void store(unsigned long long key, int depth, int ply, int score, Bound bound, Move bestMove, bool& collision);
    bool probe(unsigned long long key, int depth, int ply, int alpha, int beta, int& returnScore, Move& bestMove, bool& hit);
    void clear();
};

struct PawnEntry {
    unsigned long long key;
    int mgScore;
    int egScore;
};

class PawnTable {
public:
    static PawnEntry table[131072];
    static void store(unsigned long long key, int mgScore, int egScore);
    static std::pair<int, int> probe(unsigned long long key);
};

#endif // TT_H
