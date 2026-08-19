@echo off
echo Compiling TT Mate Regression Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_mate_tt.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_mate_tt.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running tests...
    .\test_mate_tt.exe
) else (
    echo Compilation failed.
)
