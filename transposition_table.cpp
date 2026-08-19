#include "transposition_table.h"
#include <cstring>

TranspositionTable::TranspositionTable(int numEntries) {
    size = numEntries;
    table = new TTEntry[size];
    for(int i=0; i<size; i++) {
        table[i] = TTEntry();
    }
}

TranspositionTable::~TranspositionTable() {
    delete[] table;
}

void TranspositionTable::clear() {
    for(int i=0; i<size; i++) {
        table[i] = TTEntry();
    }
}

void TranspositionTable::store(unsigned long long key, int depth, int ply, int score, Bound bound, Move bestMove, bool& collision) {
    int index = key % size;
    // Check collision (valid entry and different key)
    collision = table[index].valid && table[index].key != key;
    
    // Always replace scheme
    table[index].key = key;
    // Normalize mate scores to be relative to the node, not the root
    if (score > 9000) score += ply;
    else if (score < -9000) score -= ply;

    table[index].depth = depth;
    table[index].score = score;
    table[index].bound = bound;
    table[index].bestMove = bestMove;
    table[index].valid = true;
}

bool TranspositionTable::probe(unsigned long long key, int depth, int ply, int alpha, int beta, int& returnScore, Move& bestMove, bool& hit) {
    int index = key % size;
    TTEntry& entry = table[index];

    hit = false;
    if (entry.valid && entry.key == key) {
        hit = true;
        bestMove = entry.bestMove;
        if (entry.depth >= depth) {
            // Reconstruct the root-relative score from the node-relative score stored in TT
            int score = entry.score;
            if (score > 9000) score -= ply;
            else if (score < -9000) score += ply;

            if (entry.bound == EXACT) {
                returnScore = score;
                return true;
            } else if (entry.bound == UPPER_BOUND && score <= alpha) {
                returnScore = score;
                return true;
            } else if (entry.bound == LOWER_BOUND && score >= beta) {
                returnScore = score;
                return true;
            }
        }
    }
    return false;
}
