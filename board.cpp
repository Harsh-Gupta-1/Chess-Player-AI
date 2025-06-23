#include "board.h"
#include <iostream>

const int Board::PAWN_TABLE[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 25, 25, 10,  5,  5},
    { 0,  0,  0, 20, 20,  0,  0,  0},
    { 5, -5,-10,  0,  0,-10, -5,  5},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

const int Board::KNIGHT_TABLE[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int Board::BISHOP_TABLE[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int Board::KING_TABLE[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};

Board::Board() {
    board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
    setupBoard();
}

void Board::setupBoard() {
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
                    default:     break;
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
    int kingX = kingPos.first, kingY = kingPos.second;
    return isSquareUnderAttack(kingX, kingY, color == WHITE ? BLACK : WHITE);
}

void Board::makeMove(const Move& m) {
    Piece movingPiece = board[m.fromX][m.fromY];
    
    if (m.isEnPassant) {
        board[m.fromX][m.toY] = Piece();
    }
    
    if (m.isCastle) {
        if (m.toY == 6) {
            board[m.fromX][5] = board[m.fromX][7];
            board[m.fromX][7] = Piece();
        } else if (m.toY == 2) {
            board[m.fromX][3] = board[m.fromX][0];
            board[m.fromX][0] = Piece();
        }
    }
    
    board[m.toX][m.toY] = movingPiece;
    board[m.fromX][m.fromY] = Piece();
    
    if (m.promotion != EMPTY) {
        board[m.toX][m.toY] = Piece(m.promotion, movingPiece.color);
    }
    
    updateGameState(m, movingPiece);
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

bool Board::isDraw() {
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

int Board::evaluateMobility() {
    int whiteMobility = generateMoves(WHITE).size();
    int blackMobility = generateMoves(BLACK).size();
    return (whiteMobility - blackMobility) * 10;
}

int Board::evaluatePawnStructure() {
    int score = 0;
    
    for (int y = 0; y < 8; y++) {
        int whitePawns = 0, blackPawns = 0;
        
        for (int x = 0; x < 8; x++) {
            if (board[x][y].type == PAWN) {
                if (board[x][y].color == WHITE) whitePawns++;
                else blackPawns++;
            }
        }
        
        if (whitePawns > 1) score -= (whitePawns - 1) * 50;
        if (blackPawns > 1) score += (blackPawns - 1) * 50;
        
        if (whitePawns == 1) {
            bool isolated = true;
            if (y > 0) {
                for (int x = 0; x < 8; x++) {
                    if (board[x][y-1].type == PAWN && board[x][y-1].color == WHITE) {
                        isolated = false; break;
                    }
                }
            }
            if (y < 7 && isolated) {
                for (int x = 0; x < 8; x++) {
                    if (board[x][y+1].type == PAWN && board[x][y+1].color == WHITE) {
                        isolated = false; break;
                    }
                }
            }
            if (isolated) score -= 20;
        }
    }
    
    return score;
}

int Board::evaluate() {
    int score = 0;
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece p = board[x][y];
            if (p.type == EMPTY) continue;
            
            int materialValue = 0;
            int positionalValue = 0;
            
            switch (p.type) {
                case PAWN:   
                    materialValue = 100; 
                    positionalValue = PAWN_TABLE[p.color == WHITE ? x : 7-x][y];
                    break;
                case KNIGHT: 
                    materialValue = 320; 
                    positionalValue = KNIGHT_TABLE[p.color == WHITE ? x : 7-x][y];
                    break;
                case BISHOP: 
                    materialValue = 330; 
                    positionalValue = BISHOP_TABLE[p.color == WHITE ? x : 7-x][y];
                    break;
                case ROOK:   
                    materialValue = 500; 
                    break;
                case QUEEN:  
                    materialValue = 900; 
                    break;
                case KING:   
                    materialValue = 20000; 
                    positionalValue = KING_TABLE[p.color == WHITE ? x : 7-x][y];
                    break;
            }
            
            int totalValue = materialValue + positionalValue;
            score += (p.color == WHITE) ? totalValue : -totalValue;
        }
    }
    
    score += evaluateMobility();
    score += evaluatePawnStructure();
    
    return score;
}