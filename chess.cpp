#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <chrono> 
using namespace std;

// ----- Constants -----
enum Color { WHITE, BLACK };
enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
const int BOARD_SIZE = 8;
// Piece-square tables (from white's perspective)
const int PAWN_TABLE[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 25, 25, 10,  5,  5},
    { 0,  0,  0, 20, 20,  0,  0,  0},
    { 5, -5,-10,  0,  0,-10, -5,  5},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

const int KNIGHT_TABLE[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int BISHOP_TABLE[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int KING_TABLE[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};
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

class Board {
private:
    vector<vector<Piece>> board;
    int evaluateMobility() {
    int whiteMobility = generateMoves(WHITE).size();
    int blackMobility = generateMoves(BLACK).size();
    return (whiteMobility - blackMobility) * 10;
}

int evaluatePawnStructure() {
    int score = 0;
    
    for (int y = 0; y < 8; y++) {
        int whitePawns = 0, blackPawns = 0;
        
        // Count pawns in each file
        for (int x = 0; x < 8; x++) {
            if (board[x][y].type == PAWN) {
                if (board[x][y].color == WHITE) whitePawns++;
                else blackPawns++;
            }
        }
        
        // Penalize doubled pawns
        if (whitePawns > 1) score -= (whitePawns - 1) * 50;
        if (blackPawns > 1) score += (blackPawns - 1) * 50;
        
        // Penalize isolated pawns
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

public:
    GameState gameState;

    Board() {
        board = vector<vector<Piece>>(8, vector<Piece>(8, Piece()));
        setupBoard();
    }
    
    void setupBoard() {
        // Pawns
        for (int i = 0; i < 8; ++i) {
            board[1][i] = Piece(PAWN, WHITE);
            board[6][i] = Piece(PAWN, BLACK);
        }
        // Rooks
        board[0][0] = board[0][7] = Piece(ROOK, WHITE);
        board[7][0] = board[7][7] = Piece(ROOK, BLACK);
        // Knights
        board[0][1] = board[0][6] = Piece(KNIGHT, WHITE);
        board[7][1] = board[7][6] = Piece(KNIGHT, BLACK);
        // Bishops
        board[0][2] = board[0][5] = Piece(BISHOP, WHITE);
        board[7][2] = board[7][5] = Piece(BISHOP, BLACK);
        // Queens
        board[0][3] = Piece(QUEEN, WHITE);
        board[7][3] = Piece(QUEEN, BLACK);
        // Kings
        board[0][4] = Piece(KING, WHITE);
        board[7][4] = Piece(KING, BLACK);
    }

    void printBoard() {
        for (int i = 7; i >= 0; --i) {
            cout << i + 1 << " ";
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
                cout << symbol << " ";
            }
            cout << endl;
        }
        cout << "  a b c d e f g h" << endl;
    }

    bool isInBounds(int x, int y) const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    Piece getPiece(int x, int y) const {
        return board[x][y];
    }

    // Find king position
    pair<int, int> findKing(Color color) const {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                if (board[x][y].type == KING && board[x][y].color == color) {
                    return {x, y};
                }
            }
        }
        return {-1, -1}; // Should never happen
    }

    // Check if a square is under attack by the opponent
    bool isSquareUnderAttack(int x, int y, Color byColor) const {
        // Check pawn attacks
        int pawnDir = (byColor == WHITE) ? 1 : -1;
        if (isInBounds(x - pawnDir, y - 1) && board[x - pawnDir][y - 1].type == PAWN && board[x - pawnDir][y - 1].color == byColor) return true;
        if (isInBounds(x - pawnDir, y + 1) && board[x - pawnDir][y + 1].type == PAWN && board[x - pawnDir][y + 1].color == byColor) return true;

        // Check knight attacks
        int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (int i = 0; i < 8; i++) {
            int nx = x + knightMoves[i][0], ny = y + knightMoves[i][1];
            if (isInBounds(nx, ny) && board[nx][ny].type == KNIGHT && board[nx][ny].color == byColor) return true;
        }

        // Check bishop/queen diagonal attacks
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

        // Check rook/queen straight attacks
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

        // Check king attacks
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (isInBounds(nx, ny) && board[nx][ny].type == KING && board[nx][ny].color == byColor) return true;
            }
        }

        return false;
    }

    bool isInCheck(Color color) const {
        pair<int, int> kingPos = findKing(color);
        int kingX = kingPos.first, kingY = kingPos.second;
        return isSquareUnderAttack(kingX, kingY, color == WHITE ? BLACK : WHITE);
    }

    void makeMove(const Move& m) {
        Piece movingPiece = board[m.fromX][m.fromY];
        
        // Handle en passant capture
        if (m.isEnPassant) {
            board[m.fromX][m.toY] = Piece(); // Remove captured pawn
        }
        
        // Handle castling
        if (m.isCastle) {
            if (m.toY == 6) { // Kingside castle
                board[m.fromX][5] = board[m.fromX][7]; // Move rook
                board[m.fromX][7] = Piece();
            } else if (m.toY == 2) { // Queenside castle
                board[m.fromX][3] = board[m.fromX][0]; // Move rook
                board[m.fromX][0] = Piece();
            }
        }
        
        // Make the move
        board[m.toX][m.toY] = movingPiece;
        board[m.fromX][m.fromY] = Piece();
        
        // Handle pawn promotion
        if (m.promotion != EMPTY) {
            board[m.toX][m.toY] = Piece(m.promotion, movingPiece.color);
        }
        
        // Update game state
        updateGameState(m, movingPiece);
    }

    void undoMove(const Move& m, const Piece& captured, const GameState& prevState) {
        Piece movingPiece = board[m.toX][m.toY];
        
        // Restore original piece (handle promotion)
        if (m.promotion != EMPTY) {
            movingPiece = Piece(PAWN, movingPiece.color);
        }
        
        // Undo the basic move
        board[m.fromX][m.fromY] = movingPiece;
        board[m.toX][m.toY] = captured;
        
        // Undo en passant
        if (m.isEnPassant) {
            board[m.fromX][m.toY] = Piece(PAWN, movingPiece.color == WHITE ? BLACK : WHITE);
            board[m.toX][m.toY] = Piece(); // En passant square should be empty
        }
        
        // Undo castling
        if (m.isCastle) {
            if (m.toY == 6) { // Kingside castle
                board[m.fromX][7] = board[m.fromX][5]; // Restore rook
                board[m.fromX][5] = Piece();
            } else if (m.toY == 2) { // Queenside castle
                board[m.fromX][0] = board[m.fromX][3]; // Restore rook
                board[m.fromX][3] = Piece();
            }
        }
        
        // Restore game state
        gameState = prevState;
    }

    void updateGameState(const Move& m, const Piece& movingPiece) {
        // Clear en passant
        gameState.hasEnPassant = false;
        
        // Set en passant if pawn moved two squares
        if (movingPiece.type == PAWN && abs(m.toX - m.fromX) == 2) {
            gameState.enPassantX = (m.fromX + m.toX) / 2;
            gameState.enPassantY = m.fromY;
            gameState.hasEnPassant = true;
        }
        
        // Update castling rights
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
        
        // If rook is captured, update castling rights
        if (m.toX == 0 && m.toY == 0) gameState.whiteCanCastleQueenside = false;
        if (m.toX == 0 && m.toY == 7) gameState.whiteCanCastleKingside = false;
        if (m.toX == 7 && m.toY == 0) gameState.blackCanCastleQueenside = false;
        if (m.toX == 7 && m.toY == 7) gameState.blackCanCastleKingside = false;
    }

    vector<Move> generateMoves(Color color) {
        vector<Move> moves;
        
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

    void generatePawnMoves(int x, int y, Color color, vector<Move>& moves) {
        int dir = (color == WHITE) ? 1 : -1;
        int startRow = (color == WHITE) ? 1 : 6;
        int promotionRow = (color == WHITE) ? 7 : 0;
        
        // Forward move
        if (isInBounds(x + dir, y) && board[x + dir][y].type == EMPTY) {
            if (x + dir == promotionRow) {
                // Promotion
                moves.emplace_back(x, y, x + dir, y, QUEEN);
                moves.emplace_back(x, y, x + dir, y, ROOK);
                moves.emplace_back(x, y, x + dir, y, BISHOP);
                moves.emplace_back(x, y, x + dir, y, KNIGHT);
            } else {
                moves.emplace_back(x, y, x + dir, y);
            }
            
            // Double forward move from starting position
            if (x == startRow && board[x + 2 * dir][y].type == EMPTY) {
                moves.emplace_back(x, y, x + 2 * dir, y);
            }
        }
        
        // Captures
        for (int dy = -1; dy <= 1; dy += 2) {
            if (isInBounds(x + dir, y + dy)) {
                Piece target = board[x + dir][y + dy];
                if (target.type != EMPTY && target.color != color) {
                    if (x + dir == promotionRow) {
                        // Promotion capture
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
        
        // En passant
        if (gameState.hasEnPassant && x + dir == gameState.enPassantX) {
            if (y + 1 == gameState.enPassantY || y - 1 == gameState.enPassantY) {
                moves.emplace_back(x, y, gameState.enPassantX, gameState.enPassantY, EMPTY, true);
            }
        }
    }

    void generateKnightMoves(int x, int y, Color color, vector<Move>& moves) {
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

    void generateBishopMoves(int x, int y, Color color, vector<Move>& moves) {
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

    void generateRookMoves(int x, int y, Color color, vector<Move>& moves) {
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

    void generateQueenMoves(int x, int y, Color color, vector<Move>& moves) {
        generateBishopMoves(x, y, color, moves);
        generateRookMoves(x, y, color, moves);
    }

    void generateKingMoves(int x, int y, Color color, vector<Move>& moves) {
        // Regular king moves
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
        
        // Castling
        if (!isInCheck(color)) {
            if (color == WHITE) {
                // Kingside castle
                if (gameState.whiteCanCastleKingside && 
                    board[0][5].type == EMPTY && board[0][6].type == EMPTY &&
                    !isSquareUnderAttack(0, 5, BLACK) && !isSquareUnderAttack(0, 6, BLACK)) {
                    moves.emplace_back(x, y, 0, 6, EMPTY, false, true);
                }
                // Queenside castle
                if (gameState.whiteCanCastleQueenside && 
                    board[0][1].type == EMPTY && board[0][2].type == EMPTY && board[0][3].type == EMPTY &&
                    !isSquareUnderAttack(0, 2, BLACK) && !isSquareUnderAttack(0, 3, BLACK)) {
                    moves.emplace_back(x, y, 0, 2, EMPTY, false, true);
                }
            } else {
                // Kingside castle
                if (gameState.blackCanCastleKingside && 
                    board[7][5].type == EMPTY && board[7][6].type == EMPTY &&
                    !isSquareUnderAttack(7, 5, WHITE) && !isSquareUnderAttack(7, 6, WHITE)) {
                    moves.emplace_back(x, y, 7, 6, EMPTY, false, true);
                }
                // Queenside castle
                if (gameState.blackCanCastleQueenside && 
                    board[7][1].type == EMPTY && board[7][2].type == EMPTY && board[7][3].type == EMPTY &&
                    !isSquareUnderAttack(7, 2, WHITE) && !isSquareUnderAttack(7, 3, WHITE)) {
                    moves.emplace_back(x, y, 7, 2, EMPTY, false, true);
                }
            }
        }
    }

    vector<Move> generateLegalMoves(Color color) {
        vector<Move> allMoves = generateMoves(color);
        vector<Move> legalMoves;
        
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

    bool isCheckmate(Color color) {
        return isInCheck(color) && generateLegalMoves(color).empty();
    }

    bool isStalemate(Color color) {
        return !isInCheck(color) && generateLegalMoves(color).empty();
    }

    bool isDraw() {
        // Check for insufficient material
        vector<PieceType> whitePieces, blackPieces;
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
        
        // King vs King
        if (whitePieces.empty() && blackPieces.empty()) return true;
        
        // King + Bishop vs King or King + Knight vs King
        if ((whitePieces.size() == 1 && blackPieces.empty() && 
             (whitePieces[0] == BISHOP || whitePieces[0] == KNIGHT)) ||
            (blackPieces.size() == 1 && whitePieces.empty() && 
             (blackPieces[0] == BISHOP || blackPieces[0] == KNIGHT))) {
            return true;
        }
        
        return false;
    }

    int evaluate() {
    int score = 0;
    
    // Material and positional evaluation
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
    
    // Add mobility bonus
    score += evaluateMobility();
    
    // Add pawn structure evaluation
    score += evaluatePawnStructure();
    
    return score;
}
};

class ChessAI {
public:
    long long nodesExplored = 0;
    Move getBestMove(Board& board, Color aiColor, int depth) {
        nodesExplored = 0;
        int bestScore = (aiColor == WHITE) ? numeric_limits<int>::min() : numeric_limits<int>::max();
        Move bestMove(0, 0, 0, 0);
        
        vector<Move> legalMoves = board.generateLegalMoves(aiColor);
        if (legalMoves.empty()) return bestMove;

        for (const auto& move : legalMoves) {
            GameState prevState = board.gameState;
            Piece captured = board.getPiece(move.toX, move.toY);
            board.makeMove(move);
            
            int score = minimax(board, depth - 1, numeric_limits<int>::min(), 
                               numeric_limits<int>::max(), aiColor == WHITE ? BLACK : WHITE, aiColor == BLACK);
            
            board.undoMove(move, captured, prevState);

            if ((aiColor == WHITE && score > bestScore) || (aiColor == BLACK && score < bestScore)) {
                bestScore = score;
                bestMove = move;
            }
        }
          cout << "Nodes explored: " << nodesExplored << endl;
        return bestMove;
    }

private:
    int minimax(Board& board, int depth, int alpha, int beta, Color currentTurn, bool maximizing) {
        nodesExplored++;
        if (depth == 0) return board.evaluate();
        
        if (board.isCheckmate(currentTurn)) {
            return maximizing ? -10000 : 10000;
        }
        
        if (board.isStalemate(currentTurn) || board.isDraw()) {
            return 0;
        }

        vector<Move> moves = board.generateLegalMoves(currentTurn);
        
        if (maximizing) {
            int maxEval = numeric_limits<int>::min();
            for (const auto& move : moves) {
                GameState prevState = board.gameState;
                Piece captured = board.getPiece(move.toX, move.toY);
                board.makeMove(move);
                
                int eval = minimax(board, depth - 1, alpha, beta, 
                                 currentTurn == WHITE ? BLACK : WHITE, false);
                
                board.undoMove(move, captured, prevState);
                
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break;
            }
            return maxEval;
        } else {
            int minEval = numeric_limits<int>::max();
            for (const auto& move : moves) {
                GameState prevState = board.gameState;
                Piece captured = board.getPiece(move.toX, move.toY);
                board.makeMove(move);
                
                int eval = minimax(board, depth - 1, alpha, beta, 
                                 currentTurn == WHITE ? BLACK : WHITE, true);
                
                board.undoMove(move, captured, prevState);
                
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break;
            }
            return minEval;
        }
    }
};

class Game {
private:
    Board board;
    ChessAI ai;
    Color humanColor = WHITE;

public:
    void play() {
        while (true) {
            board.printBoard();
            
            // Check game end conditions
            if (board.isCheckmate(WHITE)) {
                cout << "Black wins by checkmate!" << endl;
                break;
            } else if (board.isCheckmate(BLACK)) {
                cout << "White wins by checkmate!" << endl;
                break;
            } else if (board.isStalemate(WHITE) || board.isStalemate(BLACK)) {
                cout << "Draw by stalemate!" << endl;
                break;
            } else if (board.isDraw()) {
                cout << "Draw by insufficient material!" << endl;
                break;
            }
            
            if (board.isInCheck(WHITE)) cout << "White is in check!" << endl;
            if (board.isInCheck(BLACK)) cout << "Black is in check!" << endl;
            
            if (humanColor == WHITE) {
                playerTurn(WHITE);
                board.printBoard();
                aiTurn(BLACK);
            } else {
                aiTurn(WHITE);
                board.printBoard();
                playerTurn(BLACK);
            }
        }
    }

    void playerTurn(Color color) {
        while (true) {
            string from, to, promotion = "";
            cout << "Your move (e.g. e2 e4, or e7 e8 Q for promotion): ";
            cin >> from >> to;
            
            // Check for promotion input
            if (cin.peek() == ' ') {
                cin >> promotion;
            }

            if (from.length() != 2 || to.length() != 2 ||
                from[0] < 'a' || from[0] > 'h' || to[0] < 'a' || to[0] > 'h' ||
                from[1] < '1' || from[1] > '8' || to[1] < '1' || to[1] > '8') {
                cout << "Invalid input format. Try again.\n";
                continue;
            }

            int fx = from[1] - '1';
            int fy = from[0] - 'a';
            int tx = to[1] - '1';
            int ty = to[0] - 'a';

            // Handle promotion
            PieceType promoPiece = EMPTY;
            if (!promotion.empty()) {
                switch (toupper(promotion[0])) {
                    case 'Q': promoPiece = QUEEN; break;
                    case 'R': promoPiece = ROOK; break;
                    case 'B': promoPiece = BISHOP; break;
                    case 'N': promoPiece = KNIGHT; break;
                    default:
                        cout << "Invalid promotion piece. Use Q, R, B, or N.\n";
                        continue;
                }
            }

            vector<Move> legalMoves = board.generateLegalMoves(color);
            bool found = false;

            for (const auto& move : legalMoves) {
                if (move.fromX == fx && move.fromY == fy &&
                    move.toX == tx && move.toY == ty &&
                    move.promotion == promoPiece) {
                    board.makeMove(move);
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Invalid move. Try again.\n";
            } else {
                break;
            }
        }
    }

    void aiTurn(Color color) {
        cout << "AI is thinking..." << endl;
        auto start = chrono::high_resolution_clock::now();
        Move best = ai.getBestMove(board, color, 4); // depth 4
        auto end = chrono::high_resolution_clock::now();
         double moveTime = chrono::duration<double>(end - start).count();
    cout << "AI took " << moveTime << " seconds to decide the move.\n";
        if (best.fromX == 0 && best.fromY == 0 && best.toX == 0 && best.toY == 0) {
            cout << "AI has no legal moves!" << endl;
            return;
        }
        
        board.makeMove(best);
        
        cout << "AI played: " << char('a' + best.fromY) << best.fromX + 1 
             << " to " << char('a' + best.toY) << best.toX + 1;
        
        if (best.promotion != EMPTY) {
            char promoChar = 'Q';
            switch (best.promotion) {
                case ROOK: promoChar = 'R'; break;
                case BISHOP: promoChar = 'B'; break;
                case KNIGHT: promoChar = 'N'; break;
                default: break;
            }
            cout << " (" << promoChar << ")";
        }
        
        if (best.isCastle) {
            cout << " (Castle)";
        }
        
        if (best.isEnPassant) {
            cout << " (En Passant)";
        }
        
        cout << endl;
    }
};

#include <chrono>
#include <iomanip> // for setw formatting

void benchmark() {
    ChessAI ai;
    vector<int> depths = {2, 3, 4, 5};
    int moveCount = 10;

    cout << left << setw(10) << "Depth"
         << setw(20) << "Avg Time (s)"
         << setw(25) << "Avg Nodes Searched" << endl;

    for (int depth : depths) {
        double totalTime = 0.0;
        long long totalNodes = 0;

        for (int i = 0; i < moveCount; i++) {
            Board board;
            Color aiColor = WHITE;

            auto start = chrono::high_resolution_clock::now();
            Move best = ai.getBestMove(board, aiColor, depth);
            auto end = chrono::high_resolution_clock::now();

            double duration = chrono::duration<double>(end - start).count();
            totalTime += duration;
            totalNodes += ai.nodesExplored;
        }

        cout << left << setw(10) << depth
             << setw(20) << totalTime / moveCount
             << setw(25) << totalNodes / moveCount << endl;
    }
}


int main() {
    benchmark();
    cout << "Starting a new game..." << endl;
    Game g;
    g.play();
    return 0;
}