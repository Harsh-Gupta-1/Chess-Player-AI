@echo off
echo Starting 100-game Gauntlet vs TSCP (Elo 1700)...
cutechess\cutechess-1.5.1-win64\cutechess-cli.exe -engine cmd=Chess-Player-AI.exe name=HarshChess proto=uci -engine cmd=tscp.exe name=TSCP proto=xboard -each tc=15+0.1 -games 20 -repeat -concurrency 10 -resign movecount=3 score=400 -draw movenumber=40 movecount=8 score=10
pause
