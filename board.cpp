#include "board.h"
#include "zobrist.h"
#include <iostream>

const int Board::MG_VALUE[6] = { 0, 82, 337, 365, 477, 1025 };
const int Board::EG_VALUE[6] = { 0, 94, 281, 297, 512,  936 };

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

Board::Board() {
    board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
    setupBoard();
}

void Board::setupBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = Piece();
        }
    }

    for (int i = 0; i < 8; ++i) {
        board[1][i] = Piece(PAWN, WHITE);
        board[6][i] = Piece(PAWN, BLACK);
    }
    board[0][0] = board[0][7] = Piece(ROOK, WHITE);
    board[7][0] = board[7][7] = Piece(ROOK, BLACK);
    board[0][1] = board[0][6] = Piece(KNIGHT, WHITE);
    board[7][1] = board[7][6] = Piece(KNIGHT, BLACK);
    board[0][2] = board[0][5] = Piece(BISHOP, WHITE);
    board[7][2] = board[7][5] = Piece(BISHOP, BLACK);
    board[0][3] = Piece(QUEEN, WHITE);
    board[7][3] = Piece(QUEEN, BLACK);
    board[0][4] = Piece(KING, WHITE);
    board[7][4] = Piece(KING, BLACK);
    
    gameState.whiteCanCastleKingside = true;
    gameState.whiteCanCastleQueenside = true;
    gameState.blackCanCastleKingside = true;
    gameState.blackCanCastleQueenside = true;
    gameState.hasEnPassant = false;
    gameState.enPassantX = -1;
    gameState.enPassantY = -1;
    
    gameState.zobristKey = Zobrist::computeHash(*this, WHITE);
    
    historyPly = 0;
    history[historyPly++] = gameState.zobristKey;
}

Color Board::loadFEN(const std::string& fen) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = Piece();
        }
    }
    
    int rank = 7, file = 0;
    size_t i = 0;
    
    for (; i < fen.length() && fen[i] != ' '; ++i) {
        char c = fen[i];
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0';
        } else {
            Color color = isupper(c) ? WHITE : BLACK;
            PieceType type = EMPTY;
            switch (tolower(c)) {
                case 'p': type = PAWN; break;
                case 'n': type = KNIGHT; break;
                case 'b': type = BISHOP; break;
                case 'r': type = ROOK; break;
                case 'q': type = QUEEN; break;
                case 'k': type = KING; break;
            }
            if (isInBounds(rank, file)) {
                board[rank][file] = Piece(type, color);
            }
            file++;
        }
    }
    
    i++;
    Color activeColor = WHITE;
    if (i < fen.length() && fen[i] == 'b') activeColor = BLACK;
    if (i < fen.length()) {
        while (i < fen.length() && fen[i] != ' ') i++;
    }
    i++;
    
    gameState.whiteCanCastleKingside = false;
    gameState.whiteCanCastleQueenside = false;
    gameState.blackCanCastleKingside = false;
    gameState.blackCanCastleQueenside = false;
    
    if (i < fen.length() && fen[i] != '-') {
        while (i < fen.length() && fen[i] != ' ') {
            switch (fen[i]) {
                case 'K': gameState.whiteCanCastleKingside = true; break;
                case 'Q': gameState.whiteCanCastleQueenside = true; break;
                case 'k': gameState.blackCanCastleKingside = true; break;
                case 'q': gameState.blackCanCastleQueenside = true; break;
            }
            i++;
        }
    } else {
        if (i < fen.length()) i++;
    }
    i++;
    
    gameState.hasEnPassant = false;
    gameState.enPassantX = -1;
    gameState.enPassantY = -1;
    
    if (i < fen.length() && fen[i] != '-') {
        char fileChar = fen[i++];
        char rankChar = fen[i++];
        gameState.enPassantY = fileChar - 'a';
        gameState.enPassantX = rankChar - '1';
        gameState.hasEnPassant = true;
    } else {
        if (i < fen.length()) i++;
    }
    
    while (i < fen.length() && fen[i] == ' ') i++;
    
    gameState.halfmoveClock = 0;
    if (i < fen.length()) {
        std::string halfmoveStr = "";
        while (i < fen.length() && fen[i] != ' ') {
            halfmoveStr += fen[i++];
        }
        if (!halfmoveStr.empty()) gameState.halfmoveClock = std::stoi(halfmoveStr);
    }
    
    while (i < fen.length() && fen[i] == ' ') i++;
    
    gameState.fullmoveNumber = 1;
    if (i < fen.length()) {
        std::string fullmoveStr = "";
        while (i < fen.length() && fen[i] != ' ') {
            fullmoveStr += fen[i++];
        }
        if (!fullmoveStr.empty()) gameState.fullmoveNumber = std::stoi(fullmoveStr);
    }
    
    gameState.zobristKey = Zobrist::computeHash(*this, activeColor);
    
    historyPly = 0;
    history[historyPly++] = gameState.zobristKey;
    
    return activeColor;
}

void Board::printBoard() {
    for (int i = 7; i >= 0; --i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            const Piece& p = board[i][j];
            char symbol = '.';
            if (p.type != EMPTY) {
                switch (p.type) {
                    case PAWN:   symbol = 'P'; break;
                    case KNIGHT: symbol = 'N'; break;
                    case BISHOP: symbol = 'B'; break;
                    case ROOK:   symbol = 'R'; break;
                    case QUEEN:  symbol = 'Q'; break;
                    case KING:   symbol = 'K'; break;
                    case EMPTY:  break;
                }
                if (p.color == BLACK) symbol = tolower(symbol);
            }
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

bool Board::isInBounds(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

Piece Board::getPiece(int x, int y) const {
    return board[x][y];
}

std::pair<int, int> Board::findKing(Color color) const {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y].type == KING && board[x][y].color == color) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

bool Board::isSquareUnderAttack(int x, int y, Color byColor) const {
    int pawnDir = (byColor == WHITE) ? 1 : -1;
    if (isInBounds(x - pawnDir, y - 1) && board[x - pawnDir][y - 1].type == PAWN && board[x - pawnDir][y - 1].color == byColor) return true;
    if (isInBounds(x - pawnDir, y + 1) && board[x - pawnDir][y + 1].type == PAWN && board[x - pawnDir][y + 1].color == byColor) return true;

    int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (int i = 0; i < 8; i++) {
        int nx = x + knightMoves[i][0], ny = y + knightMoves[i][1];
        if (isInBounds(nx, ny) && board[nx][ny].type == KNIGHT && board[nx][ny].color == byColor) return true;
    }

    int diagDirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int nx = x + i * diagDirs[d][0], ny = y + i * diagDirs[d][1];
            if (!isInBounds(nx, ny)) break;
            if (board[nx][ny].type != EMPTY) {
                if (board[nx][ny].color == byColor && (board[nx][ny].type == BISHOP || board[nx][ny].type == QUEEN)) return true;
                break;
            }
        }
    }

    int straightDirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int nx = x + i * straightDirs[d][0], ny = y + i * straightDirs[d][1];
            if (!isInBounds(nx, ny)) break;
            if (board[nx][ny].type != EMPTY) {
                if (board[nx][ny].color == byColor && (board[nx][ny].type == ROOK || board[nx][ny].type == QUEEN)) return true;
                break;
            }
        }
    }

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (isInBounds(nx, ny) && board[nx][ny].type == KING && board[nx][ny].color == byColor) return true;
        }
    }

    return false;
}

bool Board::isInCheck(Color color) const {
    std::pair<int, int> kingPos = findKing(color);
    if (kingPos.first == -1) {
        return false;
    }
    bool result = isSquareUnderAttack(kingPos.first, kingPos.second, color == WHITE ? BLACK : WHITE);
    return result;
}

void Board::makeMove(const Move& m) {
    if (historyPly < 1024) {
        history[historyPly++] = gameState.zobristKey;
    }
    Piece movingPiece = board[m.fromX][m.fromY];
    
    if (movingPiece.type == PAWN || board[m.toX][m.toY].type != EMPTY) {
        gameState.halfmoveClock = 0;
    } else {
        gameState.halfmoveClock++;
    }
    
    if (movingPiece.color == BLACK) {
        gameState.fullmoveNumber++;
    }
    
    int oldCastle = 0;
    if (gameState.whiteCanCastleKingside) oldCastle |= 1;
    if (gameState.whiteCanCastleQueenside) oldCastle |= 2;
    if (gameState.blackCanCastleKingside) oldCastle |= 4;
    if (gameState.blackCanCastleQueenside) oldCastle |= 8;
    gameState.zobristKey ^= Zobrist::castleKeys[oldCastle];
    
    if (gameState.hasEnPassant) {
        gameState.zobristKey ^= Zobrist::enPassantKeys[gameState.enPassantY];
    }
    
    gameState.zobristKey ^= Zobrist::pieceKeys[movingPiece.color][movingPiece.type][m.fromX * 8 + m.fromY];
    
    if (m.isEnPassant) {
        Piece capturedPawn = board[m.fromX][m.toY];
        gameState.zobristKey ^= Zobrist::pieceKeys[capturedPawn.color][capturedPawn.type][m.fromX * 8 + m.toY];
        board[m.fromX][m.toY] = Piece();
    } else {
        Piece captured = board[m.toX][m.toY];
        if (captured.type != EMPTY) {
            gameState.zobristKey ^= Zobrist::pieceKeys[captured.color][captured.type][m.toX * 8 + m.toY];
        }
    }
    
    if (m.isCastle) {
        if (m.toY == 6) {
            Piece rook = board[m.fromX][7];
            gameState.zobristKey ^= Zobrist::pieceKeys[rook.color][rook.type][m.fromX * 8 + 7];
            gameState.zobristKey ^= Zobrist::pieceKeys[rook.color][rook.type][m.fromX * 8 + 5];
            board[m.fromX][5] = board[m.fromX][7];
            board[m.fromX][7] = Piece();
        } else if (m.toY == 2) {
            Piece rook = board[m.fromX][0];
            gameState.zobristKey ^= Zobrist::pieceKeys[rook.color][rook.type][m.fromX * 8 + 0];
            gameState.zobristKey ^= Zobrist::pieceKeys[rook.color][rook.type][m.fromX * 8 + 3];
            board[m.fromX][3] = board[m.fromX][0];
            board[m.fromX][0] = Piece();
        }
    }
    
    board[m.toX][m.toY] = movingPiece;
    board[m.fromX][m.fromY] = Piece();
    
    Piece placedPiece = movingPiece;
    if (m.promotion != EMPTY) {
        placedPiece = Piece(m.promotion, movingPiece.color);
        board[m.toX][m.toY] = placedPiece;
    }
    
    gameState.zobristKey ^= Zobrist::pieceKeys[placedPiece.color][placedPiece.type][m.toX * 8 + m.toY];
    
    updateGameState(m, movingPiece);
    
    int newCastle = 0;
    if (gameState.whiteCanCastleKingside) newCastle |= 1;
    if (gameState.whiteCanCastleQueenside) newCastle |= 2;
    if (gameState.blackCanCastleKingside) newCastle |= 4;
    if (gameState.blackCanCastleQueenside) newCastle |= 8;
    gameState.zobristKey ^= Zobrist::castleKeys[newCastle];
    
    if (gameState.hasEnPassant) {
        gameState.zobristKey ^= Zobrist::enPassantKeys[gameState.enPassantY];
    }
    
    gameState.zobristKey ^= Zobrist::sideKey;
}

void Board::undoMove(const Move& m, const Piece& captured, const GameState& prevState) {
    Piece movingPiece = board[m.toX][m.toY];
    
    if (m.promotion != EMPTY) {
        movingPiece = Piece(PAWN, movingPiece.color);
    }
    
    board[m.fromX][m.fromY] = movingPiece;
    board[m.toX][m.toY] = captured;
    
    if (m.isEnPassant) {
        board[m.fromX][m.toY] = Piece(PAWN, movingPiece.color == WHITE ? BLACK : WHITE);
        board[m.toX][m.toY] = Piece();
    }
    
    if (m.isCastle) {
        if (m.toY == 6) {
            board[m.fromX][7] = board[m.fromX][5];
            board[m.fromX][5] = Piece();
        } else if (m.toY == 2) {
            board[m.fromX][0] = board[m.fromX][3];
            board[m.fromX][3] = Piece();
        }
    }
    
    gameState = prevState;
    if (historyPly > 0) {
        historyPly--;
    }
}

void Board::updateGameState(const Move& m, const Piece& movingPiece) {
    gameState.hasEnPassant = false;
    
    if (movingPiece.type == PAWN && abs(m.toX - m.fromX) == 2) {
        gameState.enPassantX = (m.fromX + m.toX) / 2;
        gameState.enPassantY = m.fromY;
        gameState.hasEnPassant = true;
    }
    
    if (movingPiece.type == KING) {
        if (movingPiece.color == WHITE) {
            gameState.whiteCanCastleKingside = false;
            gameState.whiteCanCastleQueenside = false;
        } else {
            gameState.blackCanCastleKingside = false;
            gameState.blackCanCastleQueenside = false;
        }
    }
    
    if (movingPiece.type == ROOK) {
        if (movingPiece.color == WHITE) {
            if (m.fromX == 0 && m.fromY == 0) gameState.whiteCanCastleQueenside = false;
            if (m.fromX == 0 && m.fromY == 7) gameState.whiteCanCastleKingside = false;
        } else {
            if (m.fromX == 7 && m.fromY == 0) gameState.blackCanCastleQueenside = false;
            if (m.fromX == 7 && m.fromY == 7) gameState.blackCanCastleKingside = false;
        }
    }
    
    if (m.toX == 0 && m.toY == 0) gameState.whiteCanCastleQueenside = false;
    if (m.toX == 0 && m.toY == 7) gameState.whiteCanCastleKingside = false;
    if (m.toX == 7 && m.toY == 0) gameState.blackCanCastleQueenside = false;
    if (m.toX == 7 && m.toY == 7) gameState.blackCanCastleKingside = false;
}

std::vector<Move> Board::generateMoves(Color color) {
    std::vector<Move> moves;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            Piece p = board[x][y];
            if (p.type == EMPTY || p.color != color) continue;

            switch (p.type) {
                case EMPTY:
                    break;
                case PAWN:
                    generatePawnMoves(x, y, color, moves);
                    break;
                case KNIGHT:
                    generateKnightMoves(x, y, color, moves);
                    break;
                case BISHOP:
                    generateBishopMoves(x, y, color, moves);
                    break;
                case ROOK:
                    generateRookMoves(x, y, color, moves);
                    break;
                case QUEEN:
                    generateQueenMoves(x, y, color, moves);
                    break;
                case KING:
                    generateKingMoves(x, y, color, moves);
                    break;
            }
        }
    }
    
    return moves;
}

void Board::generatePawnMoves(int x, int y, Color color, std::vector<Move>& moves) {
    int dir = (color == WHITE) ? 1 : -1;
    int startRow = (color == WHITE) ? 1 : 6;
    int promotionRow = (color == WHITE) ? 7 : 0;
    
    if (isInBounds(x + dir, y) && board[x + dir][y].type == EMPTY) {
        if (x + dir == promotionRow) {
            moves.emplace_back(x, y, x + dir, y, QUEEN);
            moves.emplace_back(x, y, x + dir, y, ROOK);
            moves.emplace_back(x, y, x + dir, y, BISHOP);
            moves.emplace_back(x, y, x + dir, y, KNIGHT);
        } else {
            moves.emplace_back(x, y, x + dir, y);
        }
        
        if (x == startRow && board[x + 2 * dir][y].type == EMPTY) {
            moves.emplace_back(x, y, x + 2 * dir, y);
        }
    }
    
    for (int dy = -1; dy <= 1; dy += 2) {
        if (isInBounds(x + dir, y + dy)) {
            Piece target = board[x + dir][y + dy];
            if (target.type != EMPTY && target.color != color) {
                if (x + dir == promotionRow) {
                    moves.emplace_back(x, y, x + dir, y + dy, QUEEN);
                    moves.emplace_back(x, y, x + dir, y + dy, ROOK);
                    moves.emplace_back(x, y, x + dir, y + dy, BISHOP);
                    moves.emplace_back(x, y, x + dir, y + dy, KNIGHT);
                } else {
                    moves.emplace_back(x, y, x + dir, y + dy);
                }
            }
        }
    }
    
    if (gameState.hasEnPassant && x + dir == gameState.enPassantX) {
        if (y + 1 == gameState.enPassantY || y - 1 == gameState.enPassantY) {
            moves.emplace_back(x, y, gameState.enPassantX, gameState.enPassantY, EMPTY, true);
        }
    }
}

void Board::generateKnightMoves(int x, int y, Color color, std::vector<Move>& moves) {
    int dx[] = {-2,-1,1,2,2,1,-1,-2};
    int dy[] = {1,2,2,1,-1,-2,-2,-1};
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (isInBounds(nx, ny)) {
            Piece target = board[nx][ny];
            if (target.type == EMPTY || target.color != color) {
                moves.emplace_back(x, y, nx, ny);
            }
        }
    }
}

void Board::generateBishopMoves(int x, int y, Color color, std::vector<Move>& moves) {
    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int nx = x + i * dirs[d][0], ny = y + i * dirs[d][1];
            if (!isInBounds(nx, ny)) break;
            
            Piece target = board[nx][ny];
            if (target.type == EMPTY) {
                moves.emplace_back(x, y, nx, ny);
            } else {
                if (target.color != color) {
                    moves.emplace_back(x, y, nx, ny);
                }
                break;
            }
        }
    }
}

void Board::generateRookMoves(int x, int y, Color color, std::vector<Move>& moves) {
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int nx = x + i * dirs[d][0], ny = y + i * dirs[d][1];
            if (!isInBounds(nx, ny)) break;
            
            Piece target = board[nx][ny];
            if (target.type == EMPTY) {
                moves.emplace_back(x, y, nx, ny);
            } else {
                if (target.color != color) {
                    moves.emplace_back(x, y, nx, ny);
                }
                break;
            }
        }
    }
}

void Board::generateQueenMoves(int x, int y, Color color, std::vector<Move>& moves) {
    generateBishopMoves(x, y, color, moves);
    generateRookMoves(x, y, color, moves);
}

void Board::generateKingMoves(int x, int y, Color color, std::vector<Move>& moves) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (isInBounds(nx, ny)) {
                Piece target = board[nx][ny];
                if (target.type == EMPTY || target.color != color) {
                    moves.emplace_back(x, y, nx, ny);
                }
            }
        }
    }
    
    if (!isInCheck(color)) {
        if (color == WHITE) {
            if (gameState.whiteCanCastleKingside && 
                board[0][5].type == EMPTY && board[0][6].type == EMPTY &&
                !isSquareUnderAttack(0, 5, BLACK) && !isSquareUnderAttack(0, 6, BLACK)) {
                moves.emplace_back(x, y, 0, 6, EMPTY, false, true);
            }
            if (gameState.whiteCanCastleQueenside && 
                board[0][1].type == EMPTY && board[0][2].type == EMPTY && board[0][3].type == EMPTY &&
                !isSquareUnderAttack(0, 2, BLACK) && !isSquareUnderAttack(0, 3, BLACK)) {
                moves.emplace_back(x, y, 0, 2, EMPTY, false, true);
            }
        } else {
            if (gameState.blackCanCastleKingside && 
                board[7][5].type == EMPTY && board[7][6].type == EMPTY &&
                !isSquareUnderAttack(7, 5, WHITE) && !isSquareUnderAttack(7, 6, WHITE)) {
                moves.emplace_back(x, y, 7, 6, EMPTY, false, true);
            }
            if (gameState.blackCanCastleQueenside && 
                board[7][1].type == EMPTY && board[7][2].type == EMPTY && board[7][3].type == EMPTY &&
                !isSquareUnderAttack(7, 2, WHITE) && !isSquareUnderAttack(7, 3, WHITE)) {
                moves.emplace_back(x, y, 7, 2, EMPTY, false, true);
            }
        }
    }
}

std::vector<Move> Board::generateLegalMoves(Color color) {
    std::vector<Move> allMoves = generateMoves(color);
    std::vector<Move> legalMoves;
    
    for (const Move& move : allMoves) {
        GameState prevState = gameState;
        Piece captured = getPiece(move.toX, move.toY);
        
        makeMove(move);
        
        if (!isInCheck(color)) {
            legalMoves.push_back(move);
        }
        
        undoMove(move, captured, prevState);
    }
    
    return legalMoves;
}

bool Board::isCheckmate(Color color) {
    return isInCheck(color) && generateLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) {
    return !isInCheck(color) && generateLegalMoves(color).empty();
}

bool Board::isRepetition() const {
    if (historyPly < 4) return false;
    int limit = std::max(0, historyPly - gameState.halfmoveClock);
    int repetitions = 0;
    // We only need to check every 2 plies (same side to move)
    for (int i = historyPly - 4; i >= limit; i -= 2) {
        if (history[i] == gameState.zobristKey) {
            repetitions++;
            if (repetitions >= 1) return true; // 2-fold repetition is enough to score a draw in search
        }
    }
    return false;
}

bool Board::isInsufficientMaterial() {
    std::vector<PieceType> whitePieces, blackPieces;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y].type != EMPTY && board[x][y].type != KING) {
                if (board[x][y].color == WHITE) {
                    whitePieces.push_back(board[x][y].type);
                } else {
                    blackPieces.push_back(board[x][y].type);
                }
            }
        }
    }
    
    if (whitePieces.empty() && blackPieces.empty()) return true;
    
    if ((whitePieces.size() == 1 && blackPieces.empty() && 
         (whitePieces[0] == BISHOP || whitePieces[0] == KNIGHT)) ||
        (blackPieces.size() == 1 && whitePieces.empty() && 
         (blackPieces[0] == BISHOP || blackPieces[0] == KNIGHT))) {
        return true;
    }
    
    return false;
}

bool Board::isDraw() {
    if (gameState.halfmoveClock >= 100) return true;
    return isInsufficientMaterial();
}

int Board::evaluateMobility() {
    int whiteMobility = generateMoves(WHITE).size();
    int blackMobility = generateMoves(BLACK).size();
    return (whiteMobility - blackMobility) * 10;
}

std::pair<int, int> Board::evaluatePawnStructure() {
    int mgScore = 0;
    int egScore = 0;
    
    int whitePawnsOnFile[8] = {0};
    int blackPawnsOnFile[8] = {0};
    int maxBlackPawnX[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
    int minWhitePawnX[8] = {8,8,8,8,8,8,8,8};
    
    // First pass: gather pawn data
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[x][y].type == PAWN) {
                if (board[x][y].color == WHITE) {
                    whitePawnsOnFile[y]++;
                    if (x < minWhitePawnX[y]) minWhitePawnX[y] = x;
                } else {
                    blackPawnsOnFile[y]++;
                    if (x > maxBlackPawnX[y]) maxBlackPawnX[y] = x;
                }
            }
        }
    }
    
    // Second pass: evaluate
    for (int y = 0; y < 8; y++) {
        if (whitePawnsOnFile[y] > 1) {
            mgScore -= (whitePawnsOnFile[y] - 1) * 50;
            egScore -= (whitePawnsOnFile[y] - 1) * 50;
        }
        if (blackPawnsOnFile[y] > 1) {
            mgScore += (blackPawnsOnFile[y] - 1) * 50;
            egScore += (blackPawnsOnFile[y] - 1) * 50;
        }
        
        for (int x = 0; x < 8; x++) {
            if (board[x][y].type != PAWN) continue;
            
            if (board[x][y].color == WHITE) {
                // Isolated pawn (only penalize if not doubled, to match original behavior)
                if (whitePawnsOnFile[y] == 1) {
                    bool isolated = true;
                    if (y > 0 && whitePawnsOnFile[y-1] > 0) isolated = false;
                    if (y < 7 && whitePawnsOnFile[y+1] > 0) isolated = false;
                    if (isolated) { mgScore -= 20; egScore -= 20; }
                }
                
                // Passed pawn
                bool passed = true;
                if (maxBlackPawnX[y] > x) passed = false;
                if (y > 0 && maxBlackPawnX[y-1] > x) passed = false;
                if (y < 7 && maxBlackPawnX[y+1] > x) passed = false;
                
                if (passed) {
                    int bonus = 20 + (x - 1) * 10;
                    mgScore += bonus;
                    egScore += bonus * 2;
                }
            } else {
                // Isolated pawn (only penalize if not doubled)
                if (blackPawnsOnFile[y] == 1) {
                    bool isolated = true;
                    if (y > 0 && blackPawnsOnFile[y-1] > 0) isolated = false;
                    if (y < 7 && blackPawnsOnFile[y+1] > 0) isolated = false;
                    if (isolated) { mgScore += 20; egScore += 20; }
                }
                
                // Passed pawn
                bool passed = true;
                if (minWhitePawnX[y] < x) passed = false;
                if (y > 0 && minWhitePawnX[y-1] < x) passed = false;
                if (y < 7 && minWhitePawnX[y+1] < x) passed = false;
                
                if (passed) {
                    int bonus = 20 + (6 - x) * 10;
                    mgScore -= bonus;
                    egScore -= bonus * 2;
                }
            }
        }
    }
    
    return std::make_pair(mgScore, egScore);
}

int Board::evaluate() {
    int mgScore = 0;
    int egScore = 0;
    int gamePhase = 0;
    
    int whiteBishops = 0, blackBishops = 0;
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece p = board[x][y];
            if (p.type == EMPTY) continue;
            
            int mgValue = MG_VALUE[p.type];
            int egValue = EG_VALUE[p.type];
            int mgPos = 0, egPos = 0;
            int rank = (p.color == WHITE) ? 7 - x : x;
            
            switch (p.type) {
                case PAWN:   
                    mgPos = PAWN_MG[rank][y];
                    egPos = PAWN_EG[rank][y];
                    break;
                case KNIGHT: 
                    mgPos = KNIGHT_MG[rank][y];
                    egPos = KNIGHT_EG[rank][y];
                    gamePhase += 1;
                    break;
                case BISHOP: 
                    mgPos = BISHOP_MG[rank][y];
                    egPos = BISHOP_EG[rank][y];
                    gamePhase += 1;
                    if (p.color == WHITE) whiteBishops++;
                    else blackBishops++;
                    break;
                case ROOK:   
                    mgPos = ROOK_MG[rank][y];
                    egPos = ROOK_EG[rank][y];
                    gamePhase += 2;
                    // Rook on open/semi-open file bonus
                    {
                        bool ownPawn = false, oppPawn = false;
                        for (int rx = 0; rx < 8; rx++) {
                            if (board[rx][y].type == PAWN) {
                                if (board[rx][y].color == p.color) ownPawn = true;
                                else oppPawn = true;
                            }
                        }
                        if (!ownPawn && !oppPawn) { mgPos += 20; egPos += 20; }
                        else if (!ownPawn) { mgPos += 10; egPos += 10; }
                    }
                    break;
                case QUEEN:  
                    mgPos = QUEEN_MG[rank][y];
                    egPos = QUEEN_EG[rank][y];
                    gamePhase += 4;
                    break;
                case KING:   
                    mgPos = KING_MG[rank][y];
                    egPos = KING_EG[rank][y];
                    break;
                case EMPTY: break;
            }
            
            int mgTotal = mgValue + mgPos;
            int egTotal = egValue + egPos;
            
            if (p.color == WHITE) {
                mgScore += mgTotal;
                egScore += egTotal;
            } else {
                mgScore -= mgTotal;
                egScore -= egTotal;
            }
        }
    }
    
    // Bishop pair bonus
    if (whiteBishops >= 2) { mgScore += 30; egScore += 30; }
    if (blackBishops >= 2) { mgScore -= 30; egScore -= 30; }
    
    // Development penalties: pieces still on starting squares block development
    // White minor pieces on back rank
    if (board[0][1].type == KNIGHT && board[0][1].color == WHITE) mgScore -= 15;
    if (board[0][6].type == KNIGHT && board[0][6].color == WHITE) mgScore -= 15;
    if (board[0][2].type == BISHOP && board[0][2].color == WHITE) mgScore -= 15;
    if (board[0][5].type == BISHOP && board[0][5].color == WHITE) mgScore -= 15;
    // Black minor pieces on back rank
    if (board[7][1].type == KNIGHT && board[7][1].color == BLACK) mgScore += 15;
    if (board[7][6].type == KNIGHT && board[7][6].color == BLACK) mgScore += 15;
    if (board[7][2].type == BISHOP && board[7][2].color == BLACK) mgScore += 15;
    if (board[7][5].type == BISHOP && board[7][5].color == BLACK) mgScore += 15;
    
    // Castling bonus: reward having castled (king on g1/c1 or g8/c8)
    // Penalty for losing castling rights without castling
    if (!gameState.whiteCanCastleKingside && !gameState.whiteCanCastleQueenside) {
        // White can no longer castle - check if king is safely castled
        if (board[0][6].type == KING && board[0][6].color == WHITE) mgScore += 30; // castled kingside
        else if (board[0][2].type == KING && board[0][2].color == WHITE) mgScore += 30; // castled queenside
    }
    if (!gameState.blackCanCastleKingside && !gameState.blackCanCastleQueenside) {
        if (board[7][6].type == KING && board[7][6].color == BLACK) mgScore -= 30;
        else if (board[7][2].type == KING && board[7][2].color == BLACK) mgScore -= 30;
    }
    
    // King Safety (Midgame only)
    std::pair<int, int> whiteKing = findKing(WHITE);
    std::pair<int, int> blackKing = findKing(BLACK);
    
    // White King Safety
    if (whiteKing.second >= 5) { // Kingside (f, g, h files)
        int penalty = 0;
        if (board[1][5].type != PAWN || board[1][5].color != WHITE) penalty += 15; // f2
        if (board[1][6].type != PAWN || board[1][6].color != WHITE) penalty += 20; // g2
        if (board[1][7].type != PAWN || board[1][7].color != WHITE) penalty += 15; // h2
        if (board[2][6].type == PAWN && board[2][6].color == WHITE) penalty -= 10; // g3 is okay
        mgScore -= penalty;
    } else if (whiteKing.second <= 2) { // Queenside (a, b, c files)
        int penalty = 0;
        if (board[1][0].type != PAWN || board[1][0].color != WHITE) penalty += 10; // a2
        if (board[1][1].type != PAWN || board[1][1].color != WHITE) penalty += 15; // b2
        if (board[1][2].type != PAWN || board[1][2].color != WHITE) penalty += 15; // c2
        mgScore -= penalty;
    } else {
        mgScore -= 30; // King in center
    }
    
    // Black King Safety
    if (blackKing.second >= 5) { // Kingside
        int penalty = 0;
        if (board[6][5].type != PAWN || board[6][5].color != BLACK) penalty += 15; // f7
        if (board[6][6].type != PAWN || board[6][6].color != BLACK) penalty += 20; // g7
        if (board[6][7].type != PAWN || board[6][7].color != BLACK) penalty += 15; // h7
        if (board[5][6].type == PAWN && board[5][6].color == BLACK) penalty -= 10; // g6 is okay
        mgScore += penalty; // positive score is bad for black
    } else if (blackKing.second <= 2) { // Queenside
        int penalty = 0;
        if (board[6][0].type != PAWN || board[6][0].color != BLACK) penalty += 10; // a7
        if (board[6][1].type != PAWN || board[6][1].color != BLACK) penalty += 15; // b7
        if (board[6][2].type != PAWN || board[6][2].color != BLACK) penalty += 15; // c7
        mgScore += penalty;
    } else {
        mgScore += 30; // King in center
    }
    
    std::pair<int, int> pawnStructScore = evaluatePawnStructure();
    mgScore += pawnStructScore.first;
    egScore += pawnStructScore.second;
    
    // Mobility evaluation removed: Profiler showed it took 35% of CPU time, 
    // tanking NPS. PSQTs already cover 90% of mobility rewards.
    
    // Tapered Eval Interpolation
    // gamePhase max is 24 (4 knights=4, 4 bishops=4, 4 rooks=8, 2 queens=8)
    if (gamePhase > 24) gamePhase = 24;
    int phase = gamePhase;
    int score = (mgScore * phase + egScore * (24 - phase)) / 24;
    
    return score;
}