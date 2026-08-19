#include "board.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "zobrist.h"

const int Board::MG_VALUE[7] = { 0, 82, 337, 365, 477, 1025, 0 };
const int Board::EG_VALUE[7] = { 0, 94, 281, 297, 512, 936, 0 };

const int Board::PAWN_MG[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    { 98, 134,  61,  95,  68, 126,  34, -11},
    {-6,   7,  26,  31,  62,  11,   8, -24},
    {-14,  13,   6,  21,  23,  12,  17, -23},
    {-27,  -2,  -5,  12,  17,   6,  10, -25},
    {-26,  -4,  -4, -10,   3,   3,  33, -12},
    {-35,  -1, -20, -23, -15,  24,  38, -22},
    {  0,   0,   0,   0,   0,   0,   0,   0}
};

const int Board::PAWN_EG[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    {178, 173, 158, 134, 147, 132, 165, 187},
    { 94, 100,  85,  67,  56,  53,  82,  84},
    { 32,  24,  13,   5,  -2,   4,  17,  17},
    { 13,   9,  -3,  -7,  -7,  -8,   3,  -1},
    {  4,   7,  -6,   1,   0,  -5,  -1,  -8},
    { 13,   8,   8,  10,  13,   0,   2,  -7},
    {  0,   0,   0,   0,   0,   0,   0,   0}
};

const int Board::KNIGHT_MG[8][8] = {
    {-167, -89, -34, -49,  61, -97, -15, -107},
    { -73, -41,  72,  36,  23,  62,   7,  -17},
    { -47,  60,  37,  65,  84, 129,  73,   44},
    {  -9,  17,  19,  53,  37,  69,  18,   22},
    { -13,   4,  16,  13,  28,  19,  21,   -8},
    { -23,  -9,  12,  10,  19,  17,  25,  -16},
    { -29, -53, -12,  -3,  -1,  18, -14,  -19},
    {-105, -21, -58, -33, -17, -28, -19,  -23}
};

const int Board::KNIGHT_EG[8][8] = {
    {-58, -38, -13, -28, -31, -27, -63, -99},
    {-25,  -8, -25,  -2,  -9, -25, -24, -52},
    {-24, -20,  10,   9,  -1,  -9, -19, -41},
    {-17,   3,  22,  22,  22,  11,   8, -18},
    {-18,  -6,  16,  25,  16,  17,   4, -18},
    {-23,  -3,  -1,  15,  10,  -3, -20, -22},
    {-42, -20, -10,  -5,  -2, -20, -23, -44},
    {-29, -51, -23, -38, -29, -27, -43, -36}
};

const int Board::BISHOP_MG[8][8] = {
    {-29,   4, -82, -37, -25, -42,   7,  -8},
    {-26,  16, -18, -13,  30,  59,  18, -47},
    {-16,  37,  43,  40,  35,  50,  37,  -2},
    { -4,   5,  19,  50,  37,  37,   7,  -2},
    { -6,  13,  13,  26,  34,  12,  10,   4},
    {  0,  15,  15,  15,  14,  27,  18,  10},
    {  4,  15,  16,   0,   7,  21,  33,   1},
    {-33,  -3, -14, -21, -13, -12, -39, -21}
};

const int Board::BISHOP_EG[8][8] = {
    {-14, -21, -11,  -8,  -7,  -9, -17, -24},
    { -8,  -4,   7, -12,  -3, -13,  -4, -14},
    {  2,  -8,   0,  -1,  -2,   6,   0,   4},
    { -3,   9,  12,   9,  14,  10,   3,   2},
    { -6,   3,  13,  19,   7,  10,  -3,  -9},
    {-12,  -3,   8,  10,  13,   3,  -7, -15},
    {-14, -18,  -7,  -1,   4,  -9, -15, -27},
    {-23,  -9, -23,  -5,  -9, -16,  -5, -17}
};

const int Board::ROOK_MG[8][8] = {
    { 32,  42,  32,  51,  63,   9,  31,  43},
    { 27,  32,  58,  62,  80,  67,  26,  44},
    { -5,  19,  26,  36,  17,  45,  61,  16},
    {-24, -11,   7,  26,  24,  35,  -8, -20},
    {-36, -26, -12,  -1,   9,  -7,   6, -23},
    {-45, -25, -16, -17,   3,   0,  -5, -33},
    {-44, -16, -20,  -9,  -1,  11,  -6, -71},
    {-19, -13,   1,  17,  16,   7, -37, -26}
};

const int Board::ROOK_EG[8][8] = {
    { 13,  10,  18,  15,  12,  12,   8,   5},
    { 11,  13,  13,  11,  -3,   3,   8,   3},
    {  7,   7,   7,   5,   4,  -3,  -5,  -3},
    {  4,   3,  13,   1,   2,   1,  -1,   2},
    {  3,   5,   8,   4,  -5,  -6,  -8, -11},
    { -4,   0,  -5,  -1,  -7, -12,  -8, -16},
    { -6,  -6,   0,   2,  -9,  -9, -11,  -3},
    { -9,   2,   3,  -1,  -5, -13,   4, -20}
};

const int Board::QUEEN_MG[8][8] = {
    {-28,   0,  29,  12,  59,  44,  43,  45},
    {-24, -39,  -5,   1, -16,  57,  28,  54},
    {-13, -17,   7,   8,  29,  56,  47,  57},
    {-27, -27, -16, -16,  -1,  17,  -2,   1},
    { -9, -26,  -9, -10,  -2,  -4,   3,  -3},
    {-14,   2, -11,  -2,  -5,   2,  14,   5},
    {-35,  -8,  11,   0,   8,  -7,  -6,  14},
    {-20, -27, -36, -15, -12, -21, -22, -20}
};

const int Board::QUEEN_EG[8][8] = {
    { -9,  22,  22,  27,  27,  19,  10,  20},
    {-17,  20,  32,  41,  58,  25,  30,   0},
    {-20,   6,   9,  49,  47,  35,  19,   9},
    {  3,  22,  24,  45,  57,  40,  57,  36},
    {-18,  28,  19,  47,  31,  34,  12,  11},
    { 16,  20,  22,  51,  25,  60,  12,  27},
    { 25,   8,  12,  43,  43,  22,  16,  23},
    {-14, -15, -15, -13, -10, -24, -20, -11}
};

const int Board::KING_MG[8][8] = {
    {-65,  23,  16, -15, -56, -34,   2,  13},
    { 29,  -1, -20,  -7,  -8,  -4, -38, -29},
    { -9,  24,   2, -16, -20,   6,  22, -22},
    {-17, -20, -12, -27, -30, -25, -14, -36},
    {-49, -1, -27, -39, -46, -44, -33, -51},
    {-14, -14, -22, -46, -44, -30, -15, -27},
    {  1,   7,  -8, -64, -43, -16,   9,   8},
    {-15,  36,  12, -54,   8, -28,  24,  14}
};

const int Board::KING_EG[8][8] = {
    {-74, -35, -18, -18, -11,  15,   4, -17},
    {-12,  17,  14,  17,  17,  38,  23,  11},
    { 10,  17,  23,  15,  20,  45,  44,  13},
    { -8,  22,  24,  27,  26,  33,  26,   3},
    {-18,  -4,  21,  24,  27,  23,   9, -11},
    {-19,  -3,  11,  21,  23,  16,   7,  -9},
    {-27, -11,   4,  13,  14,   4,  -5, -17},
    {-53, -34, -21, -11, -28, -14, -24, -43}
};

namespace Attacks {
    Bitboard knightAttacks[64];
    Bitboard kingAttacks[64];
    Bitboard pawnAttacks[2][64];
    Bitboard rayAttacks[8][64]; // N, S, E, W, NE, NW, SE, SW

    void init() {
        for (int sq = 0; sq < 64; ++sq) {
            int r = sq / 8;
            int f = sq % 8;
            
            // Knight attacks
            Bitboard n = 0;
            if (r >= 2 && f >= 1) n |= (1ULL << (sq - 17));
            if (r >= 2 && f <= 6) n |= (1ULL << (sq - 15));
            if (r >= 1 && f >= 2) n |= (1ULL << (sq - 10));
            if (r >= 1 && f <= 5) n |= (1ULL << (sq - 6));
            if (r <= 6 && f >= 2) n |= (1ULL << (sq + 6));
            if (r <= 6 && f <= 5) n |= (1ULL << (sq + 10));
            if (r <= 5 && f >= 1) n |= (1ULL << (sq + 15));
            if (r <= 5 && f <= 6) n |= (1ULL << (sq + 17));
            knightAttacks[sq] = n;
            
            // King attacks
            Bitboard k = 0;
            if (r >= 1) k |= (1ULL << (sq - 8));
            if (r <= 6) k |= (1ULL << (sq + 8));
            if (f >= 1) k |= (1ULL << (sq - 1));
            if (f <= 6) k |= (1ULL << (sq + 1));
            if (r >= 1 && f >= 1) k |= (1ULL << (sq - 9));
            if (r >= 1 && f <= 6) k |= (1ULL << (sq - 7));
            if (r <= 6 && f >= 1) k |= (1ULL << (sq + 7));
            if (r <= 6 && f <= 6) k |= (1ULL << (sq + 9));
            kingAttacks[sq] = k;
            
            // Pawn attacks
            Bitboard pw = 0;
            if (r <= 6 && f >= 1) pw |= (1ULL << (sq + 7));
            if (r <= 6 && f <= 6) pw |= (1ULL << (sq + 9));
            pawnAttacks[WHITE][sq] = pw;
            
            Bitboard pb = 0;
            if (r >= 1 && f >= 1) pb |= (1ULL << (sq - 9));
            if (r >= 1 && f <= 6) pb |= (1ULL << (sq - 7));
            pawnAttacks[BLACK][sq] = pb;
            
            // Rays
            Bitboard ray = 0;
            for (int i=r+1; i<8; ++i) ray |= (1ULL << (i*8 + f)); rayAttacks[0][sq] = ray; // N
            ray = 0;
            for (int i=r-1; i>=0; --i) ray |= (1ULL << (i*8 + f)); rayAttacks[1][sq] = ray; // S
            ray = 0;
            for (int i=f+1; i<8; ++i) ray |= (1ULL << (r*8 + i)); rayAttacks[2][sq] = ray; // E
            ray = 0;
            for (int i=f-1; i>=0; --i) ray |= (1ULL << (r*8 + i)); rayAttacks[3][sq] = ray; // W
            ray = 0;
            for (int i=r+1, j=f+1; i<8 && j<8; ++i, ++j) ray |= (1ULL << (i*8 + j)); rayAttacks[4][sq] = ray; // NE
            ray = 0;
            for (int i=r+1, j=f-1; i<8 && j>=0; ++i, --j) ray |= (1ULL << (i*8 + j)); rayAttacks[5][sq] = ray; // NW
            ray = 0;
            for (int i=r-1, j=f+1; i>=0 && j<8; --i, ++j) ray |= (1ULL << (i*8 + j)); rayAttacks[6][sq] = ray; // SE
            ray = 0;
            for (int i=r-1, j=f-1; i>=0 && j>=0; --i, --j) ray |= (1ULL << (i*8 + j)); rayAttacks[7][sq] = ray; // SW
        }
    }
    
    Bitboard getRayAttacks(int sq, int dir, Bitboard occupied) {
        Bitboard attacks = rayAttacks[dir][sq];
        Bitboard blockers = attacks & occupied;
        if (blockers) {
            int blockerSq = (dir == 0 || dir == 4 || dir == 5 || dir == 2) ? __builtin_ctzll(blockers) : 63 - __builtin_clzll(blockers);
            attacks ^= rayAttacks[dir][blockerSq];
        }
        return attacks;
    }
    
    Bitboard getBishopAttacks(int sq, Bitboard occupied) {
        return getRayAttacks(sq, 4, occupied) | getRayAttacks(sq, 5, occupied) | getRayAttacks(sq, 6, occupied) | getRayAttacks(sq, 7, occupied);
    }
    
    Bitboard getRookAttacks(int sq, Bitboard occupied) {
        return getRayAttacks(sq, 0, occupied) | getRayAttacks(sq, 1, occupied) | getRayAttacks(sq, 2, occupied) | getRayAttacks(sq, 3, occupied);
    }
    
    Bitboard getQueenAttacks(int sq, Bitboard occupied) {
        return getRookAttacks(sq, occupied) | getBishopAttacks(sq, occupied);
    }
}

Board::Board() {
    Attacks::init();
    setupBoard();
}

void Board::setupBoard() {
    for(int i=0; i<7; ++i) pieces[i] = 0;
    colors[WHITE] = 0;
    colors[BLACK] = 0;
    loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    gameState.zobristKey = Zobrist::computeHash(*this, WHITE);
}

Piece Board::getPiece(int sq) const {
    return pieceList[sq];
}

Piece Board::getPiece(int x, int y) const {
    return getPiece(x * 8 + y);
}

Color Board::loadFEN(const std::string& fen) {
    for(int i=0; i<7; ++i) pieces[i] = 0;
    colors[WHITE] = 0;
    colors[BLACK] = 0;
    for(int i=0; i<64; ++i) pieceList[i] = Piece(EMPTY, WHITE);
    gameState.mgScore = 0;
    gameState.egScore = 0;
    
    std::istringstream iss(fen);
    std::string boardFen, turn, castling, enPassant;
    int halfmove = 0, fullmove = 1;
    iss >> boardFen >> turn >> castling >> enPassant >> halfmove >> fullmove;
    
    int rank = 7, file = 0;
    for (char c : boardFen) {
        if (c == '/') { rank--; file = 0; }
        else if (isdigit(c)) { file += (c - '0'); }
        else {
            Color col = islower(c) ? BLACK : WHITE;
            PieceType t;
            char l = tolower(c);
            if (l == 'p') t = PAWN;
            else if (l == 'n') t = KNIGHT;
            else if (l == 'b') t = BISHOP;
            else if (l == 'r') t = ROOK;
            else if (l == 'q') t = QUEEN;
            else t = KING;
            
            int sq = rank * 8 + file;
            pieces[t] |= (1ULL << sq);
            colors[col] |= (1ULL << sq);
            pieceList[sq] = Piece(t, col);
            int mgPsq = 0, egPsq = 0;
            int pRank = (col == WHITE) ? 7 - rank : rank;
            switch(t) {
                case PAWN:   mgPsq = PAWN_MG[pRank][file]; egPsq = PAWN_EG[pRank][file]; break;
                case KNIGHT: mgPsq = KNIGHT_MG[pRank][file]; egPsq = KNIGHT_EG[pRank][file]; break;
                case BISHOP: mgPsq = BISHOP_MG[pRank][file]; egPsq = BISHOP_EG[pRank][file]; break;
                case ROOK:   mgPsq = ROOK_MG[pRank][file]; egPsq = ROOK_EG[pRank][file]; break;
                case QUEEN:  mgPsq = QUEEN_MG[pRank][file]; egPsq = QUEEN_EG[pRank][file]; break;
                case KING:   mgPsq = KING_MG[pRank][file]; egPsq = KING_EG[pRank][file]; break;
                default: break;
            }
            int mgVal = MG_VALUE[t] + mgPsq;
            int egVal = EG_VALUE[t] + egPsq;
            if (col == WHITE) {
                gameState.mgScore += mgVal;
                gameState.egScore += egVal;
            } else {
                gameState.mgScore -= mgVal;
                gameState.egScore -= egVal;
            }
            file++;
        }
    }
    
    int savedMgScore = gameState.mgScore;
    int savedEgScore = gameState.egScore;
    gameState = GameState();
    gameState.mgScore = savedMgScore;
    gameState.egScore = savedEgScore;
    gameState.whiteCanCastleKingside = castling.find('K') != std::string::npos;
    gameState.whiteCanCastleQueenside = castling.find('Q') != std::string::npos;
    gameState.blackCanCastleKingside = castling.find('k') != std::string::npos;
    gameState.blackCanCastleQueenside = castling.find('q') != std::string::npos;
    
    if (enPassant != "-") {
        gameState.hasEnPassant = true;
        gameState.enPassantX = enPassant[1] - '1';
        gameState.enPassantY = enPassant[0] - 'a';
    } else {
        gameState.hasEnPassant = false;
    }
    
    gameState.halfmoveClock = halfmove;
    gameState.fullmoveNumber = fullmove;
    
    initCache();
    Color c_turn = turn == "w" ? WHITE : BLACK;
    gameState.zobristKey = Zobrist::computeHash(*this, c_turn);
    return c_turn;
}

void Board::initCache() {
    for(int c=0; c<2; ++c) {
        for(int t=0; t<7; ++t) {
            pieceCount[c][t] = __builtin_popcountll(pieces[t] & colors[c]);
        }
        kingPos[c].first = -1;
        kingPos[c].second = -1;
        if (pieces[KING] & colors[c]) {
            int sq = __builtin_ctzll(pieces[KING] & colors[c]);
            kingPos[c] = {sq / 8, sq % 8};
        }
    }
    historyPly = 0;
}

bool Board::isSquareUnderAttack(int sq, Color byColor) const {
    Bitboard occ = colors[WHITE] | colors[BLACK];
    if (Attacks::pawnAttacks[byColor == WHITE ? BLACK : WHITE][sq] & pieces[PAWN] & colors[byColor]) return true;
    if (Attacks::knightAttacks[sq] & pieces[KNIGHT] & colors[byColor]) return true;
    if (Attacks::getBishopAttacks(sq, occ) & (pieces[BISHOP] | pieces[QUEEN]) & colors[byColor]) return true;
    if (Attacks::getRookAttacks(sq, occ) & (pieces[ROOK] | pieces[QUEEN]) & colors[byColor]) return true;
    if (Attacks::kingAttacks[sq] & pieces[KING] & colors[byColor]) return true;
    return false;
}

bool Board::isSquareUnderAttack(int x, int y, Color byColor) const {
    return isSquareUnderAttack(x * 8 + y, byColor);
}

bool Board::isInCheck(Color color) const {
    int ksq = kingPos[color].first * 8 + kingPos[color].second;
    return isSquareUnderAttack(ksq, color == WHITE ? BLACK : WHITE);
}


int getPieceValue(Piece p, int sq) {
    if (p.type == EMPTY) return 0;
    int r = sq / 8;
    int f = sq % 8;
    int score = Board::MG_VALUE[p.type];
    if (p.color == WHITE) r = 7 - r;
    if (p.type == PAWN) score += Board::PAWN_MG[r][f];
    else if (p.type == KNIGHT) score += Board::KNIGHT_MG[r][f];
    else if (p.type == BISHOP) score += Board::BISHOP_MG[r][f];
    else if (p.type == ROOK) score += Board::ROOK_MG[r][f];
    else if (p.type == QUEEN) score += Board::QUEEN_MG[r][f];
    else if (p.type == KING) score += Board::KING_MG[r][f];
    return p.color == WHITE ? score : -score;
}

int getPieceValueEg(Piece p, int sq) {
    if (p.type == EMPTY) return 0;
    int r = sq / 8;
    int f = sq % 8;
    int score = Board::EG_VALUE[p.type];
    if (p.color == WHITE) r = 7 - r;
    if (p.type == PAWN) score += Board::PAWN_EG[r][f];
    else if (p.type == KNIGHT) score += Board::KNIGHT_EG[r][f];
    else if (p.type == BISHOP) score += Board::BISHOP_EG[r][f];
    else if (p.type == ROOK) score += Board::ROOK_EG[r][f];
    else if (p.type == QUEEN) score += Board::QUEEN_EG[r][f];
    else if (p.type == KING) score += Board::KING_EG[r][f];
    return p.color == WHITE ? score : -score;
}

void Board::makeMove(const Move& m) {
    if (historyPly < 1024) {
        history[historyPly++] = gameState.zobristKey;
    }
    
    int fromSq = m.fromX * 8 + m.fromY;
    int toSq = m.toX * 8 + m.toY;
    
    Piece p = pieceList[fromSq];
    Piece captured = pieceList[toSq];
    
    if (p.type == PAWN || captured.type != EMPTY) {
        gameState.halfmoveClock = 0;
    } else {
        gameState.halfmoveClock++;
    }
    
    if (p.color == BLACK) {
        gameState.fullmoveNumber++;
    }
    
    Bitboard fromMask = 1ULL << fromSq;
    Bitboard toMask = 1ULL << toSq;
    Bitboard moveMask = fromMask | toMask;
    
    // Remove from original square
    pieces[p.type] ^= fromMask;
    colors[p.color] ^= fromMask;
    pieceList[fromSq] = Piece(EMPTY, WHITE);
    gameState.mgScore -= getPieceValue(p, fromSq);
    gameState.egScore -= getPieceValueEg(p, fromSq);
    gameState.zobristKey ^= Zobrist::pieceKeys[p.color][p.type][fromSq];
    if (p.type == PAWN) gameState.pawnKey ^= Zobrist::pieceKeys[p.color][PAWN][fromSq];
    
    // Handle capture
    if (captured.type != EMPTY) {
        pieces[captured.type] ^= toMask;
        colors[captured.color] ^= toMask;
        pieceCount[captured.color][captured.type]--;
        gameState.mgScore -= getPieceValue(captured, toSq);
        gameState.egScore -= getPieceValueEg(captured, toSq);
        gameState.zobristKey ^= Zobrist::pieceKeys[captured.color][captured.type][toSq];
        if (captured.type == PAWN) gameState.pawnKey ^= Zobrist::pieceKeys[captured.color][PAWN][toSq];
    }
    
    // If it's a promotion, we change the piece type now
    if (m.promotion != EMPTY) {
        pieceCount[p.color][PAWN]--;
        pieceCount[p.color][m.promotion]++;
        p.type = m.promotion;
    }
    
    // Place piece at new square
    pieceList[toSq] = p;
    gameState.mgScore += getPieceValue(p, toSq);
    gameState.egScore += getPieceValueEg(p, toSq);
    gameState.zobristKey ^= Zobrist::pieceKeys[p.color][p.type][toSq];
    if (p.type == PAWN) gameState.pawnKey ^= Zobrist::pieceKeys[p.color][PAWN][toSq];
    
    // Special moves
    if (m.isCastle) {
        int r = p.color == WHITE ? 0 : 7;
        int rookFromSq = r*8 + (m.toY == 6 ? 7 : 0);
        int rookToSq = r*8 + (m.toY == 6 ? 5 : 3);
        
        pieces[ROOK] ^= (1ULL << rookFromSq) | (1ULL << rookToSq);
        colors[p.color] ^= (1ULL << rookFromSq) | (1ULL << rookToSq);
        
        Piece rookPiece(ROOK, p.color);
        pieceList[rookFromSq] = Piece(EMPTY, WHITE);
        pieceList[rookToSq] = rookPiece;
        
        gameState.mgScore -= getPieceValue(rookPiece, rookFromSq);
        gameState.egScore -= getPieceValueEg(rookPiece, rookFromSq);
        gameState.mgScore += getPieceValue(rookPiece, rookToSq);
        gameState.egScore += getPieceValueEg(rookPiece, rookToSq);
        
        gameState.zobristKey ^= Zobrist::pieceKeys[p.color][ROOK][rookFromSq];
        gameState.zobristKey ^= Zobrist::pieceKeys[p.color][ROOK][rookToSq];
    } else if (m.isEnPassant) {
        int capSq = fromSq / 8 * 8 + toSq % 8;
        pieces[PAWN] ^= (1ULL << capSq);
        Color capColor = p.color == WHITE ? BLACK : WHITE;
        colors[capColor] ^= (1ULL << capSq);
        pieceCount[capColor][PAWN]--;
        
        Piece capPawn(PAWN, capColor);
        pieceList[capSq] = Piece(EMPTY, WHITE);
        
        gameState.mgScore -= getPieceValue(capPawn, capSq);
        gameState.egScore -= getPieceValueEg(capPawn, capSq);
        gameState.zobristKey ^= Zobrist::pieceKeys[capColor][PAWN][capSq];
        gameState.pawnKey ^= Zobrist::pieceKeys[capColor][PAWN][capSq];
    }
    
    // Add piece to bitboard (doing it after promotion check)
    pieces[p.type] ^= toMask;
    colors[p.color] ^= toMask;
    
    // Update king pos
    if (p.type == KING) {
        kingPos[p.color] = {m.toX, m.toY};
    }
    
    // Remove old castling rights from Zobrist
    int oldCastle = 0;
    if (gameState.whiteCanCastleKingside) oldCastle |= 1;
    if (gameState.whiteCanCastleQueenside) oldCastle |= 2;
    if (gameState.blackCanCastleKingside) oldCastle |= 4;
    if (gameState.blackCanCastleQueenside) oldCastle |= 8;
    gameState.zobristKey ^= Zobrist::castleKeys[oldCastle];
    
    // Remove old en passant
    if (gameState.hasEnPassant) {
        gameState.zobristKey ^= Zobrist::enPassantKeys[gameState.enPassantY];
    }
    
    updateGameState(m, p);
    
    // Add new castling rights
    int newCastle = 0;
    if (gameState.whiteCanCastleKingside) newCastle |= 1;
    if (gameState.whiteCanCastleQueenside) newCastle |= 2;
    if (gameState.blackCanCastleKingside) newCastle |= 4;
    if (gameState.blackCanCastleQueenside) newCastle |= 8;
    gameState.zobristKey ^= Zobrist::castleKeys[newCastle];
    
    // Add new en passant
    if (gameState.hasEnPassant) {
        gameState.zobristKey ^= Zobrist::enPassantKeys[gameState.enPassantY];
    }
    
    // Switch turn
    gameState.zobristKey ^= Zobrist::sideKey;
}

void Board::undoMove(const Move& m, const Piece& captured, const GameState& prevState) {
    int fromSq = m.fromX * 8 + m.fromY;
    int toSq = m.toX * 8 + m.toY;
    
    Piece p = pieceList[toSq]; // It's currently at toSq
    if (m.promotion != EMPTY) p.type = PAWN;
    
    Bitboard fromMask = 1ULL << fromSq;
    Bitboard toMask = 1ULL << toSq;
    Bitboard moveMask = fromMask | toMask;
    
    // Reverse the move for the piece
    if (m.promotion != EMPTY) {
        pieces[m.promotion] ^= toMask;
        pieces[PAWN] ^= fromMask;
        colors[p.color] ^= moveMask;
        pieceCount[p.color][PAWN]++;
        pieceCount[p.color][m.promotion]--;
    } else {
        pieces[p.type] ^= moveMask;
        colors[p.color] ^= moveMask;
    }
    
    pieceList[fromSq] = p;
    pieceList[toSq] = captured;
    
    if (captured.type != EMPTY) {
        pieces[captured.type] ^= toMask;
        colors[captured.color] ^= toMask;
        pieceCount[captured.color][captured.type]++;
    }
    
    if (m.isCastle) {
        int r = p.color == WHITE ? 0 : 7;
        int rookFromSq = r*8 + (m.toY == 6 ? 7 : 0);
        int rookToSq = r*8 + (m.toY == 6 ? 5 : 3);
        
        pieces[ROOK] ^= (1ULL << rookFromSq) | (1ULL << rookToSq);
        colors[p.color] ^= (1ULL << rookFromSq) | (1ULL << rookToSq);
        
        pieceList[rookFromSq] = Piece(ROOK, p.color);
        pieceList[rookToSq] = Piece(EMPTY, WHITE);
    } else if (m.isEnPassant) {
        int capSq = fromSq / 8 * 8 + toSq % 8;
        Color capColor = p.color == WHITE ? BLACK : WHITE;
        pieces[PAWN] ^= (1ULL << capSq);
        colors[capColor] ^= (1ULL << capSq);
        pieceCount[capColor][PAWN]++;
        pieceList[capSq] = Piece(PAWN, capColor);
        pieceList[toSq] = Piece(EMPTY, WHITE); // Because it was empty before capture
    }
    
    if (p.type == KING) {
        kingPos[p.color] = {m.fromX, m.fromY};
    }
    
    gameState = prevState;
    historyPly--;
}

void Board::updateGameState(const Move& m, const Piece& movingPiece) {
    // Basic stuff
    gameState.hasEnPassant = false;
    if (movingPiece.type == PAWN && abs(m.fromX - m.toX) == 2) {
        gameState.hasEnPassant = true;
        gameState.enPassantX = (m.fromX + m.toX) / 2;
        gameState.enPassantY = m.fromY;
    }
    
    if (movingPiece.type == KING) {
        if (movingPiece.color == WHITE) {
            gameState.whiteCanCastleKingside = false;
            gameState.whiteCanCastleQueenside = false;
        } else {
            gameState.blackCanCastleKingside = false;
            gameState.blackCanCastleQueenside = false;
        }
    } else if (movingPiece.type == ROOK) {
        if (m.fromX == 0 && m.fromY == 0) gameState.whiteCanCastleQueenside = false;
        if (m.fromX == 0 && m.fromY == 7) gameState.whiteCanCastleKingside = false;
        if (m.fromX == 7 && m.fromY == 0) gameState.blackCanCastleQueenside = false;
        if (m.fromX == 7 && m.fromY == 7) gameState.blackCanCastleKingside = false;
    }
    // Handle rook captures revoking castling rights
    if (m.toX == 0 && m.toY == 0) gameState.whiteCanCastleQueenside = false;
    if (m.toX == 0 && m.toY == 7) gameState.whiteCanCastleKingside = false;
    if (m.toX == 7 && m.toY == 0) gameState.blackCanCastleQueenside = false;
    if (m.toX == 7 && m.toY == 7) gameState.blackCanCastleKingside = false;
}

void Board::generatePawnMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard pawns = pieces[PAWN] & colors[color];
    Bitboard empty = ~(colors[WHITE] | colors[BLACK]);
    Bitboard enemies = colors[color == WHITE ? BLACK : WHITE];
    int dir = (color == WHITE) ? 8 : -8;
    
    // Single push
    Bitboard singlePushes = (color == WHITE ? pawns << 8 : pawns >> 8) & empty;
    Bitboard validPushes = singlePushes & target;
    Bitboard p = validPushes;
    while (p) {
        int toSq = __builtin_ctzll(p);
        int fromSq = toSq - dir;
        if (toSq / 8 == (color == WHITE ? 7 : 0)) {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, QUEEN));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, ROOK));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, BISHOP));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, KNIGHT));
        } else {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8));
        }
        p &= p - 1;
    }
    
    // Double push
    Bitboard doublePushes = (color == WHITE ? (singlePushes & 0x0000000000FF0000ULL) << 8 : (singlePushes & 0x0000FF0000000000ULL) >> 8) & empty;
    validPushes = doublePushes & target;
    p = validPushes;
    while (p) {
        int toSq = __builtin_ctzll(p);
        int fromSq = toSq - 2 * dir;
        moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8));
        p &= p - 1;
    }
    
    // Captures
    Bitboard attacksLeft = (color == WHITE) ? (pawns & ~0x0101010101010101ULL) << 7 : (pawns & ~0x0101010101010101ULL) >> 9;
    Bitboard attacksRight = (color == WHITE) ? (pawns & ~0x8080808080808080ULL) << 9 : (pawns & ~0x8080808080808080ULL) >> 7;
    
    Bitboard validLeft = attacksLeft & enemies & target;
    p = validLeft;
    while (p) {
        int toSq = __builtin_ctzll(p);
        int fromSq = toSq - (color == WHITE ? 7 : -9);
        if (toSq / 8 == (color == WHITE ? 7 : 0)) {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, QUEEN));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, ROOK));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, BISHOP));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, KNIGHT));
        } else {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8));
        }
        p &= p - 1;
    }
    
    Bitboard validRight = attacksRight & enemies & target;
    p = validRight;
    while (p) {
        int toSq = __builtin_ctzll(p);
        int fromSq = toSq - (color == WHITE ? 9 : -7);
        if (toSq / 8 == (color == WHITE ? 7 : 0)) {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, QUEEN));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, ROOK));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, BISHOP));
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, KNIGHT));
        } else {
            moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8));
        }
        p &= p - 1;
    }
    
    // En passant
    if (gameState.hasEnPassant) {
        int epSq = gameState.enPassantX * 8 + gameState.enPassantY;
        Bitboard epMask = 1ULL << epSq;
        if (epMask & target) {
            Bitboard epLeft = attacksLeft & epMask;
            if (epLeft) {
                int toSq = __builtin_ctzll(epLeft);
                int fromSq = toSq - (color == WHITE ? 7 : -9);
                moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, EMPTY, true));
            }
            Bitboard epRight = attacksRight & epMask;
            if (epRight) {
                int toSq = __builtin_ctzll(epRight);
                int fromSq = toSq - (color == WHITE ? 9 : -7);
                moves.push_back(Move(fromSq/8, fromSq%8, toSq/8, toSq%8, EMPTY, true));
            }
        }
    }
}

void Board::generateKnightMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard knights = pieces[KNIGHT] & colors[color];
    while (knights) {
        int sq = __builtin_ctzll(knights);
        Bitboard attacks = Attacks::knightAttacks[sq] & target;
        while (attacks) {
            int toSq = __builtin_ctzll(attacks);
            moves.push_back(Move(sq/8, sq%8, toSq/8, toSq%8));
            attacks &= attacks - 1;
        }
        knights &= knights - 1;
    }
}

void Board::generateBishopMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard bishops = pieces[BISHOP] & colors[color];
    Bitboard occ = colors[WHITE] | colors[BLACK];
    while (bishops) {
        int sq = __builtin_ctzll(bishops);
        Bitboard attacks = Attacks::getBishopAttacks(sq, occ) & target;
        while (attacks) {
            int toSq = __builtin_ctzll(attacks);
            moves.push_back(Move(sq/8, sq%8, toSq/8, toSq%8));
            attacks &= attacks - 1;
        }
        bishops &= bishops - 1;
    }
}

void Board::generateRookMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard rooks = pieces[ROOK] & colors[color];
    Bitboard occ = colors[WHITE] | colors[BLACK];
    while (rooks) {
        int sq = __builtin_ctzll(rooks);
        Bitboard attacks = Attacks::getRookAttacks(sq, occ) & target;
        while (attacks) {
            int toSq = __builtin_ctzll(attacks);
            moves.push_back(Move(sq/8, sq%8, toSq/8, toSq%8));
            attacks &= attacks - 1;
        }
        rooks &= rooks - 1;
    }
}

void Board::generateQueenMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard queens = pieces[QUEEN] & colors[color];
    Bitboard occ = colors[WHITE] | colors[BLACK];
    while (queens) {
        int sq = __builtin_ctzll(queens);
        Bitboard attacks = Attacks::getQueenAttacks(sq, occ) & target;
        while (attacks) {
            int toSq = __builtin_ctzll(attacks);
            moves.push_back(Move(sq/8, sq%8, toSq/8, toSq%8));
            attacks &= attacks - 1;
        }
        queens &= queens - 1;
    }
}

void Board::generateKingMoves(Color color, MoveList& moves, Bitboard target) {
    Bitboard king = pieces[KING] & colors[color];
    if (king) {
        int sq = __builtin_ctzll(king);
        Bitboard attacks = Attacks::kingAttacks[sq] & target;
        while (attacks) {
            int toSq = __builtin_ctzll(attacks);
            moves.push_back(Move(sq/8, sq%8, toSq/8, toSq%8));
            attacks &= attacks - 1;
        }
        
        // Castling
        if (!isInCheck(color)) {
            if (color == WHITE) {
                if (gameState.whiteCanCastleKingside) {
                    if (!((colors[WHITE] | colors[BLACK]) & ((1ULL << 5) | (1ULL << 6)))) {
                        if (!isSquareUnderAttack(5, BLACK) && !isSquareUnderAttack(6, BLACK)) {
                            moves.push_back(Move(0, 4, 0, 6, EMPTY, false, true));
                        }
                    }
                }
                if (gameState.whiteCanCastleQueenside) {
                    if (!((colors[WHITE] | colors[BLACK]) & ((1ULL << 1) | (1ULL << 2) | (1ULL << 3)))) {
                        if (!isSquareUnderAttack(2, BLACK) && !isSquareUnderAttack(3, BLACK)) {
                            moves.push_back(Move(0, 4, 0, 2, EMPTY, false, true));
                        }
                    }
                }
            } else {
                if (gameState.blackCanCastleKingside) {
                    if (!((colors[WHITE] | colors[BLACK]) & ((1ULL << 61) | (1ULL << 62)))) {
                        if (!isSquareUnderAttack(61, WHITE) && !isSquareUnderAttack(62, WHITE)) {
                            moves.push_back(Move(7, 4, 7, 6, EMPTY, false, true));
                        }
                    }
                }
                if (gameState.blackCanCastleQueenside) {
                    if (!((colors[WHITE] | colors[BLACK]) & ((1ULL << 57) | (1ULL << 58) | (1ULL << 59)))) {
                        if (!isSquareUnderAttack(58, WHITE) && !isSquareUnderAttack(59, WHITE)) {
                            moves.push_back(Move(7, 4, 7, 2, EMPTY, false, true));
                        }
                    }
                }
            }
        }
    }
}

void Board::generateMoves(Color color, MoveList& moves) {
    Bitboard target = ~colors[color]; // Can move to empty or enemy squares
    generatePawnMoves(color, moves, target);
    generateKnightMoves(color, moves, target);
    generateBishopMoves(color, moves, target);
    generateRookMoves(color, moves, target);
    generateQueenMoves(color, moves, target);
    generateKingMoves(color, moves, target);
}

void Board::generateLegalMoves(Color color, MoveList& legalMoves) {
    MoveList pseudoMoves;
    generateMoves(color, pseudoMoves);
    
    for (int i=0; i<pseudoMoves.size(); ++i) {
        Move m = pseudoMoves[i];
        Piece captured = getPiece(m.toX * 8 + m.toY);
        GameState prevState = gameState;
        makeMove(m);
        if (!isInCheck(color)) {
            legalMoves.push_back(m);
        }
        undoMove(m, captured, prevState);
    }
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;
    MoveList moves;
    generateLegalMoves(color, moves);
    return moves.empty();
}

bool Board::isStalemate(Color color) {
    if (isInCheck(color)) return false;
    MoveList moves;
    generateLegalMoves(color, moves);
    return moves.empty();
}

bool Board::hasNonPawnMaterial(Color color) const {
    return (pieces[KNIGHT] | pieces[BISHOP] | pieces[ROOK] | pieces[QUEEN]) & colors[color];
}

bool Board::isInsufficientMaterial() {
    Bitboard allPieces = colors[WHITE] | colors[BLACK];
    if (pieces[PAWN] || pieces[ROOK] || pieces[QUEEN]) return false;
    
    int whiteKnights = __builtin_popcountll(pieces[KNIGHT] & colors[WHITE]);
    int blackKnights = __builtin_popcountll(pieces[KNIGHT] & colors[BLACK]);
    int whiteBishops = __builtin_popcountll(pieces[BISHOP] & colors[WHITE]);
    int blackBishops = __builtin_popcountll(pieces[BISHOP] & colors[BLACK]);
    
    if (whiteKnights == 0 && blackKnights == 0 && whiteBishops == 0 && blackBishops == 0) return true;
    if (whiteKnights == 1 && blackKnights == 0 && whiteBishops == 0 && blackBishops == 0) return true;
    if (whiteKnights == 0 && blackKnights == 1 && whiteBishops == 0 && blackBishops == 0) return true;
    if (whiteKnights == 0 && blackKnights == 0 && whiteBishops == 1 && blackBishops == 0) return true;
    if (whiteKnights == 0 && blackKnights == 0 && whiteBishops == 0 && blackBishops == 1) return true;
    return false;
}

bool Board::isRepetition() const {
    if (historyPly < 4) return false;
    int reps = 0;
    for (int i = historyPly - 4; i >= 0; i -= 2) {
        if (history[i] == gameState.zobristKey) {
            reps++;
            if (reps >= 1) return true; // 2-fold repetition in our context for fast draw detection
        }
    }
    return false;
}

bool Board::isDraw() {
    return gameState.halfmoveClock >= 100 || isRepetition() || isInsufficientMaterial();
}

std::pair<int, int> Board::evaluatePawnStructure() {
    int hashIndex = gameState.pawnKey & 16383;
    PawnEntry& entry = pawnTable[hashIndex];
    if (entry.valid && entry.key == gameState.pawnKey) {
        return {entry.mgScore, entry.egScore};
    }
    
    int mgScore = 0;
    int egScore = 0;
    
    Bitboard whitePawns = pieces[PAWN] & colors[WHITE];
    Bitboard blackPawns = pieces[PAWN] & colors[BLACK];
    
    int whitePawnsOnFile[8] = {0};
    int blackPawnsOnFile[8] = {0};
    
    Bitboard wp = whitePawns;
    while (wp) {
        int sq = __builtin_ctzll(wp);
        whitePawnsOnFile[sq % 8]++;
        wp &= wp - 1;
    }
    Bitboard bp = blackPawns;
    while (bp) {
        int sq = __builtin_ctzll(bp);
        blackPawnsOnFile[sq % 8]++;
        bp &= bp - 1;
    }
    
    for (int f = 0; f < 8; f++) {
        if (whitePawnsOnFile[f] > 1) {
            mgScore -= (whitePawnsOnFile[f] - 1) * 50;
            egScore -= (whitePawnsOnFile[f] - 1) * 50;
        }
        if (blackPawnsOnFile[f] > 1) {
            mgScore += (blackPawnsOnFile[f] - 1) * 50;
            egScore += (blackPawnsOnFile[f] - 1) * 50;
        }
    }
    
    wp = whitePawns;
    while (wp) {
        int sq = __builtin_ctzll(wp);
        int r = sq / 8;
        int f = sq % 8;
        if (whitePawnsOnFile[f] == 1) {
            bool isolated = true;
            if (f > 0 && whitePawnsOnFile[f-1] > 0) isolated = false;
            if (f < 7 && whitePawnsOnFile[f+1] > 0) isolated = false;
            if (isolated) { mgScore -= 20; egScore -= 20; }
        }
        bool passed = true;
        for (int checkR = r + 1; checkR < 8 && passed; checkR++) {
            for (int df = -1; df <= 1; df++) {
                int cf = f + df;
                if (cf >= 0 && cf < 8 && ((blackPawns >> (checkR * 8 + cf)) & 1)) { passed = false; break; }
            }
        }
        if (passed) {
            int bonus = 20 + (r - 1) * 10;
            mgScore += bonus;
            egScore += bonus * 2;
        }
        wp &= wp - 1;
    }
    
    bp = blackPawns;
    while (bp) {
        int sq = __builtin_ctzll(bp);
        int r = sq / 8;
        int f = sq % 8;
        if (blackPawnsOnFile[f] == 1) {
            bool isolated = true;
            if (f > 0 && blackPawnsOnFile[f-1] > 0) isolated = false;
            if (f < 7 && blackPawnsOnFile[f+1] > 0) isolated = false;
            if (isolated) { mgScore += 20; egScore += 20; }
        }
        bool passed = true;
        for (int checkR = r - 1; checkR >= 0 && passed; checkR--) {
            for (int df = -1; df <= 1; df++) {
                int cf = f + df;
                if (cf >= 0 && cf < 8 && ((whitePawns >> (checkR * 8 + cf)) & 1)) { passed = false; break; }
            }
        }
        if (passed) {
            int bonus = 20 + (6 - r) * 10;
            mgScore -= bonus;
            egScore -= bonus * 2;
        }
        bp &= bp - 1;
    }
    
    entry.key = gameState.pawnKey;
    entry.mgScore = mgScore;
    entry.egScore = egScore;
    entry.valid = true;
    return {mgScore, egScore};
}

int Board::evaluate() {
    int mgScore = gameState.mgScore;
    int egScore = gameState.egScore;
    
    int gamePhase = pieceCount[WHITE][KNIGHT] + pieceCount[BLACK][KNIGHT]
                  + pieceCount[WHITE][BISHOP] + pieceCount[BLACK][BISHOP]
                  + 2 * (pieceCount[WHITE][ROOK] + pieceCount[BLACK][ROOK])
                  + 4 * (pieceCount[WHITE][QUEEN] + pieceCount[BLACK][QUEEN]);
    if (gamePhase > 24) gamePhase = 24;
    
    if (pieceCount[WHITE][BISHOP] >= 2) { mgScore += 30; egScore += 30; }
    if (pieceCount[BLACK][BISHOP] >= 2) { mgScore -= 30; egScore -= 30; }
    
    Bitboard whiteRooks = pieces[ROOK] & colors[WHITE];
    while (whiteRooks) {
        int sq = __builtin_ctzll(whiteRooks);
        int f = sq % 8;
        Bitboard fileMask = 0x0101010101010101ULL << f;
        bool ownPawn = (pieces[PAWN] & colors[WHITE] & fileMask) != 0;
        bool oppPawn = (pieces[PAWN] & colors[BLACK] & fileMask) != 0;
        if (!ownPawn && !oppPawn) { mgScore += 20; egScore += 20; }
        else if (!ownPawn) { mgScore += 10; egScore += 10; }
        whiteRooks &= whiteRooks - 1;
    }
    
    Bitboard blackRooks = pieces[ROOK] & colors[BLACK];
    while (blackRooks) {
        int sq = __builtin_ctzll(blackRooks);
        int f = sq % 8;
        Bitboard fileMask = 0x0101010101010101ULL << f;
        bool ownPawn = (pieces[PAWN] & colors[BLACK] & fileMask) != 0;
        bool oppPawn = (pieces[PAWN] & colors[WHITE] & fileMask) != 0;
        if (!ownPawn && !oppPawn) { mgScore -= 20; egScore -= 20; }
        else if (!ownPawn) { mgScore -= 10; egScore -= 10; }
        blackRooks &= blackRooks - 1;
    }
    
    if (pieceList[1].type == KNIGHT && pieceList[1].color == WHITE) mgScore -= 15;
    if (pieceList[6].type == KNIGHT && pieceList[6].color == WHITE) mgScore -= 15;
    if (pieceList[2].type == BISHOP && pieceList[2].color == WHITE) mgScore -= 15;
    if (pieceList[5].type == BISHOP && pieceList[5].color == WHITE) mgScore -= 15;
    if (pieceList[57].type == KNIGHT && pieceList[57].color == BLACK) mgScore += 15;
    if (pieceList[62].type == KNIGHT && pieceList[62].color == BLACK) mgScore += 15;
    if (pieceList[58].type == BISHOP && pieceList[58].color == BLACK) mgScore += 15;
    if (pieceList[61].type == BISHOP && pieceList[61].color == BLACK) mgScore += 15;
    
    if (!gameState.whiteCanCastleKingside && !gameState.whiteCanCastleQueenside) {
        if (pieceList[6].type == KING && pieceList[6].color == WHITE) mgScore += 30;
        else if (pieceList[2].type == KING && pieceList[2].color == WHITE) mgScore += 30;
    }
    if (!gameState.blackCanCastleKingside && !gameState.blackCanCastleQueenside) {
        if (pieceList[62].type == KING && pieceList[62].color == BLACK) mgScore -= 30;
        else if (pieceList[58].type == KING && pieceList[58].color == BLACK) mgScore -= 30;
    }
    
    int wkf = kingPos[WHITE].second;
    int bkf = kingPos[BLACK].second;
    
    if (wkf >= 5) {
        int penalty = 0;
        if (pieceList[13].type != PAWN || pieceList[13].color != WHITE) penalty += 15;
        if (pieceList[14].type != PAWN || pieceList[14].color != WHITE) penalty += 20;
        if (pieceList[15].type != PAWN || pieceList[15].color != WHITE) penalty += 15;
        if (pieceList[22].type == PAWN && pieceList[22].color == WHITE) penalty -= 10;
        mgScore -= penalty;
    } else if (wkf <= 2) {
        int penalty = 0;
        if (pieceList[8].type != PAWN || pieceList[8].color != WHITE) penalty += 10;
        if (pieceList[9].type != PAWN || pieceList[9].color != WHITE) penalty += 15;
        if (pieceList[10].type != PAWN || pieceList[10].color != WHITE) penalty += 15;
        mgScore -= penalty;
    } else {
        mgScore -= 30;
    }
    
    if (bkf >= 5) {
        int penalty = 0;
        if (pieceList[53].type != PAWN || pieceList[53].color != BLACK) penalty += 15;
        if (pieceList[54].type != PAWN || pieceList[54].color != BLACK) penalty += 20;
        if (pieceList[55].type != PAWN || pieceList[55].color != BLACK) penalty += 15;
        if (pieceList[46].type == PAWN && pieceList[46].color == BLACK) penalty -= 10;
        mgScore += penalty;
    } else if (bkf <= 2) {
        int penalty = 0;
        if (pieceList[48].type != PAWN || pieceList[48].color != BLACK) penalty += 10;
        if (pieceList[49].type != PAWN || pieceList[49].color != BLACK) penalty += 15;
        if (pieceList[50].type != PAWN || pieceList[50].color != BLACK) penalty += 15;
        mgScore += penalty;
    } else {
        mgScore += 30;
    }
    
    std::pair<int, int> pawnStructScore = evaluatePawnStructure();
    mgScore += pawnStructScore.first;
    egScore += pawnStructScore.second;
    
    int score = (mgScore * gamePhase + egScore * (24 - gamePhase)) / 24;
    
    return score;
}

void Board::printBoard() {
    // Simple print for debug
}
