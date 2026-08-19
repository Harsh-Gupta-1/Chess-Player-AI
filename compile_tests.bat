@echo off
echo Compiling TT Mate Regression Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_mate_tt.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_mate_tt.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running TT Mate Tests...
    .\test_mate_tt.exe
) else (
    echo Compilation failed.
)

echo.
echo Compiling Draw Regression Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_draws.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_draws.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running Draw Tests...
    .\test_draws.exe
) else (
    echo Compilation failed.
)

echo.
echo Compiling Zobrist and Make/Undo Invariant Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_invariants.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_invariants.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running Invariant Tests...
    .\test_invariants.exe
) else (
    echo Compilation failed.
)

echo.
echo Compiling Search Correctness Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_search_correctness.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_search_correctness.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running Correctness Tests...
    .\test_search_correctness.exe
) else (
    echo Compilation failed.
)

echo.
echo Compiling UCI Robustness Tests...
g++ -std=c++11 -O3 -march=native -flto -Wall -Wextra test_uci.cpp board.cpp chess_ai.cpp game.cpp zobrist.cpp transposition_table.cpp uci.cpp -o test_uci.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running UCI Tests...
    .\test_uci.exe
) else (
    echo Compilation failed.
)
