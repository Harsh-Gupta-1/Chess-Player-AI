#include "transposition_table.h"

TranspositionTable::TranspositionTable(int numEntries) {
    size = numEntries;
    table = new TTEntry[size];
}

TranspositionTable::~TranspositionTable() {
    delete[] table;
}

void TranspositionTable::clear() {
    for (int i = 0; i < size; i++) {
        table[i].valid = false;
    }
}

void TranspositionTable::store(unsigned long long key, int depth, int score, Bound bound, Move bestMove) {
    int index = key % size;
    // Always replace scheme
    table[index].key = key;
    table[index].depth = depth;
    table[index].score = score;
    table[index].bound = bound;
    table[index].bestMove = bestMove;
    table[index].valid = true;
}

bool TranspositionTable::probe(unsigned long long key, int depth, int alpha, int beta, int& returnScore, Move& bestMove) {
    int index = key % size;
    TTEntry& entry = table[index];

    if (entry.valid && entry.key == key) {
        bestMove = entry.bestMove;
        if (entry.depth >= depth) {
            if (entry.bound == EXACT) {
                returnScore = entry.score;
                return true;
            } else if (entry.bound == UPPER_BOUND && entry.score <= alpha) {
                returnScore = entry.score;
                return true;
            } else if (entry.bound == LOWER_BOUND && entry.score >= beta) {
                returnScore = entry.score;
                return true;
            }
        }
    }
    return false;
}
