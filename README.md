# Chess-Player-AI# Chess AI

A simple command-line chess AI implemented in C++ using the minimax algorithm with alpha-beta pruning. The program allows users to play against the AI or run performance benchmarks to evaluate the AI's search efficiency.

## Features
- **Playable Chess Game**: Play against the AI in a text-based interface.
- **Minimax with Alpha-Beta Pruning**: The AI uses a depth-limited minimax algorithm with alpha-beta pruning for move selection.
- **Board Evaluation**: Evaluates positions based on material, piece-square tables, mobility, and pawn structure.
- **Move Generation**: Supports all legal chess moves, including castling, en passant, and promotions.
- **Benchmarking**: Measures average time and nodes explored per move at various depths to assess performance.
- **Standard Chess Rules**: Fully implements chess rules, including check, checkmate, stalemate, and draw conditions.

## Installation
### Prerequisites
- C++ compiler (e.g., `g++`) supporting C++11 or later
- Make (optional, for using the provided Makefile)
- Operating system: Windows, Linux, or macOS

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/chess-ai.git
   cd chess-ai

Compile the code:g++ -std=c++11 main.cpp board.cpp chess_ai.cpp game.cpp benchmark.cpp -o chess


Run the executable:./chess



Usage
Playing a Game
Run the program to start a new game:
./chess


The game alternates between the human player (White by default) and the AI (Black).
Enter moves in algebraic notation (e.g., e2e4 for moving a pawn from e2 to e4).
For promotions, append the piece type (e.g., e7e8Q for promoting to a queen).
The board is displayed after each move, with standard chess notation (a-h, 1-8).
The game ends with checkmate, stalemate, or a draw.

Running the Benchmark
To evaluate the AI's performance, uncomment the benchmark(); call in main.cpp:
int main() {
    benchmark(); // Uncomment this line
    std::cout << "Starting a new game..." << std::endl;
    Game g;
    g.play();
    return 0;
}

Recompile and run:
make
./chess

The benchmark outputs average time and nodes explored per move for depths 1 to 4, helping assess search efficiency.
File Structure

piece.h: Defines Piece, Move, and GameState structs for chess pieces and moves.
board.h and board.cpp: Implements the Board class for game state, move generation, and evaluation.
chess_ai.h and chess_ai.cpp: Implements the ChessAI class with minimax and alpha-beta pruning.
game.h and game.cpp: Implements the Game class for managing gameplay and user interaction.
benchmark.h and benchmark.cpp: Implements the benchmark function for performance testing.
main.cpp: Entry point, initializes the game or benchmark.
Makefile: Simplifies compilation.

Extensibility

Enhance Evaluation: Modify Board::evaluate in board.cpp to include additional terms (e.g., king safety, control of center).
Optimize Search: Add transposition tables or iterative deepening to ChessAI::minimax in chess_ai.cpp.
Add Features: Implement puzzle evaluation, time controls, or a graphical interface.
Test Suites: Integrate standard test suites (e.g., Bratko-Kopec, WAC) for tactical accuracy.

Contributing
Contributions are welcome! Please:

Fork the repository.
Create a feature branch (git checkout -b feature-name).
Commit changes (git commit -m "Add feature").
Push to the branch (git push origin feature-name).
Open a pull request.

Inspired by chess programming tutorials and open-source engines.
Uses standard C++ libraries for portability.


Feel free to open issues or suggest improvements!```