@echo off
g++ -std=c++11 -O3 -Wall -Wextra main.cpp board.cpp chess_ai.cpp game.cpp benchmark.cpp perft.cpp zobrist.cpp transposition_table.cpp uci.cpp -o Chess-Player-AI.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful!
) else (
    echo Compilation failed.
)
