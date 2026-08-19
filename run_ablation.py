import subprocess
import re
import os

configs = [
    {"name": "Baseline", "args": []},
    {"name": "No Null Move", "args": ["-no-null"]},
    {"name": "No LMR", "args": ["-no-lmr"]},
    {"name": "No Killer", "args": ["-no-killer"]},
    {"name": "No History", "args": ["-no-history"]}
]

engine_path = "./chess_engine.exe"

def run_benchmark(args):
    cmd = [engine_path, "bench"] + args
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    
    telemetry = {}
    in_telemetry = False
    
    for line in result.stdout.splitlines():
        line = line.strip()
        if line == "[TELEMETRY]":
            in_telemetry = True
            continue
        if line == "[/TELEMETRY]":
            in_telemetry = False
            continue
            
        if in_telemetry and ":" in line:
            key, val = line.split(":", 1)
            telemetry[key.strip()] = int(val.strip())
            
    return telemetry

results = []

for cfg in configs:
    print(f"Running {cfg['name']}...")
    telemetry = run_benchmark(cfg["args"])
    results.append((cfg["name"], telemetry))

markdown_content = "# Search Ablation Results\n\n"
markdown_content += "This document tracks the telemetry results of disabling specific search heuristics to validate their effectiveness.\n\n"

markdown_content += "| Configuration | Nodes | QNodes | BetaCutoffs | TTCutoffs | LMR Reductions | Null Cutoffs | Killer Hits | Hist Hits |\n"
markdown_content += "|---|---|---|---|---|---|---|---|---|\n"

for name, t in results:
    if not t:
        markdown_content += f"| {name} | FAILED | FAILED | FAILED | FAILED | FAILED | FAILED | FAILED | FAILED |\n"
        continue
        
    markdown_content += f"| {name} | {t.get('Nodes', 0):,} | {t.get('QNodes', 0):,} | {t.get('BetaCutoffs', 0):,} | {t.get('TTCutoffs', 0):,} | {t.get('LMRReductions', 0):,} | {t.get('NullCutoffs', 0):,} | {t.get('KillerHits', 0):,} | {t.get('HistoryHits', 0):,} |\n"

with open("ablation_results.md", "w") as f:
    f.write(markdown_content)

print("Ablation testing complete. Results written to ablation_results.md")
