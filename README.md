# Chess-Player-AI

A high-performance command-line chess engine implemented in C++ using the minimax algorithm with alpha-beta pruning. The engine utilizes a highly optimized **Bitboard** architecture to achieve blistering search speeds and positional understanding. The program allows users to play against the AI, run performance benchmarks, or connect it to GUI software via the UCI protocol.

## Features

- **Bitboard Architecture**: Uses 64-bit integers for core board representation, enabling ultra-fast, lock-free move generation and bitwise evaluation techniques.
- **Negamax with Alpha-Beta Pruning**: Uses a state-of-the-art Negamax formulation with Alpha-Beta pruning for move selection.
- **Iterative Deepening**: Searches progressively deeper (depth 1, then 2, then 3...) to ensure the best moves are found early, drastically improving pruning.
- **Principal Variation Search (PVS)**: Optimized Alpha-Beta pruning using zero-window searches for non-principal variation moves.
- **Move Ordering**: Implements MVV-LVA (Most Valuable Victim - Least Valuable Attacker), Killer Heuristic, and History Heuristic.
- **Quiescence Search**: Eliminates the "Horizon Effect" by continuing to search all tactical captures at the end of the main search depth. Uses a zero-allocation stack-based sorting approach for maximum throughput.
- **Selective Extensions**: Automatically extends the search depth when a king is in check, ensuring forced mate sequences are not overlooked.
- **UCI Protocol Support**: The engine is fully compatible with the Universal Chess Interface protocol, allowing it to be plugged into standard GUIs like Arena, CuteChess, and Lichess.
- **Incremental Zobrist Hashing**: State keys are XOR'd incrementally during `makeMove` and `undoMove`, feeding the Transposition Table (TT) with zero overhead.
- **Tapered Evaluation**: Sophisticated positional evaluation that seamlessly interpolates between midgame and endgame phases, including piece-square tables, bishop pair bonuses, pawn structure (isolated, doubled, passed), and rook open-file bonuses.
- **Benchmarking & Profiling**: Built-in ablation framework and `std::chrono` timers to measure search nodes, CPU time bottlenecks, and Nodes Per Second (NPS).

## Installation

### Prerequisites

- C++ compiler (e.g., `g++`) supporting C++11 or later
- Operating system: Windows, Linux, or macOS

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/Harsh-Gupta-1/Chess-Player-AI.git
   cd Chess-Player-AI
   ```

2. Compile the code (we recommend `-O3` and `-march=native` for maximum performance):
   ```bash
   g++ -std=c++11 -O3 -march=native main.cpp board.cpp chess_ai.cpp game.cpp benchmark.cpp perft.cpp transposition_table.cpp zobrist.cpp uci.cpp -o chess
   ```

3. Run the executable:
   ```bash
   ./chess
   ```

## Usage

### Playing a Game

1. Run the program to start a new game:
   ```bash
   ./chess play
   ```

2. Enter moves in algebraic notation (e.g., `e2e4` for moving a pawn from e2 to e4).
3. For promotions, append the piece type (e.g., `e7e8q` for promoting to a queen).

### Running Tests & Benchmarks

You can run the engine in different modes using command-line arguments:

1. **UCI Mode (Default)** (for plugging into Arena/CuteChess GUIs):
   ```bash
   ./chess
   # or
   ./chess uci
   ```

2. **PERFT Suite** (tests move generation correctness):
   ```bash
   ./chess perft
   ```

3. **Benchmark Suite** (tests search speed and efficiency):
   ```bash
   ./chess bench
   ```

## Engine Strength & Benchmarks

HarshChess has been formally tested using a **Sequential Probability Ratio Test (SPRT)** against established reference engines at a 15+0.1 time control. By heavily profiling the code, implementing Bitboards, and rewriting the innermost search loops to prevent dynamic memory allocation, the engine achieves a benchmark speed of **over 2.5 Million Nodes Per Second (NPS)**.

**SPRT Match Results:**

| Opponent | Reference Elo | Result | Score | SPRT Outcome |
| :--- | :--- | :--- | :--- | :--- |
| **TSCP 1.81** | ~1700 | **Crushing Win** | - | H1 Accepted |
| **Vice 1.1** | ~2100 | **Win** (41W - 18L - 9D) | 66.9% | H1 Accepted |

*Note: The migration to a bitboard representation and sophisticated tapered evaluation completely broke through the engine's previous 2100 Elo ceiling. It now comfortably defeats established ~2100 Elo engines in gauntlet matches.*

## Contributing

Contributions are welcome! Please:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit changes (`git commit -m "Add feature"`).
4. Push to the branch (`git push origin feature-name`).
5. Open a pull request.
