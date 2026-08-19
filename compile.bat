@echo off
g++ -std=c++11 main.cpp board.cpp chess_ai.cpp game.cpp benchmark.cpp perft.cpp zobrist.cpp transposition_table.cpp -o chess.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful!
) else (
    echo Compilation failed.
)
