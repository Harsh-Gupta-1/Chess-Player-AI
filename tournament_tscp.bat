@echo off
echo Starting SPRT Gauntlet vs TSCP 1.81 (~1700 Elo)...
cutechess\cutechess-1.5.1-win64\cutechess-cli.exe -engine cmd=chess_engine.exe name=HarshChess proto=uci -engine cmd=tscp.exe name=TSCP proto=xboard -each tc=2+0.05 -games 20 -repeat -concurrency 5 -resign movecount=3 score=400 -draw movenumber=40 movecount=8 score=10 > tournament_results.txt
pause
