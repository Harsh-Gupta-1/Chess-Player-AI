@echo off
echo Starting SPRT Gauntlet vs TSCP 1.81 (~1700 Elo)...
cutechess\cutechess-1.5.1-win64\cutechess-cli.exe -engine cmd=Chess-Player-AI.exe name=HarshChess proto=uci -engine cmd=tscp.exe name=TSCP proto=xboard -each tc=15+0.1 -games 1000 -repeat -concurrency 5 -resign movecount=3 score=400 -draw movenumber=40 movecount=8 score=10 -sprt elo0=0 elo1=50 alpha=0.05 beta=0.05
pause
