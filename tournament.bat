@echo off
echo Starting Gauntlet vs Vice 1.1 (~2100 Elo)...
cutechess\cutechess-1.5.1-win64\cutechess-cli.exe -engine cmd=Chess-Player-AI.exe name=HarshChess proto=uci -engine cmd=vice.exe name=Vice proto=uci -each tc=15+0.1 -games 1000 -repeat -concurrency 5 -resign movecount=3 score=400 -draw movenumber=40 movecount=8 score=10 -sprt elo0=0 elo1=50 alpha=0.05 beta=0.05
pause
