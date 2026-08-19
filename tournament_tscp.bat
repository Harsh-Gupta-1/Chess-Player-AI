@echo off
echo Starting SPRT Gauntlet vs TSCP 1.81 (~1700 Elo)...
powershell -Command "cutechess\cutechess-1.5.1-win64\cutechess-cli.exe -engine cmd=chess_engine.exe name=HarshChess proto=uci -engine cmd=tscp.exe name=TSCP proto=xboard -each tc=2+0.05 -rounds 500 -games 2 -repeat -concurrency 5 -resign movecount=3 score=400 -draw movenumber=40 movecount=8 score=10 -sprt elo0=0 elo1=50 alpha=0.05 beta=0.05 | Tee-Object -FilePath tournament_results_sprt.txt"
pause
