# Search Ablation Results

This document tracks the telemetry results of disabling specific search heuristics to validate their effectiveness.

| Configuration | Nodes | QNodes | BetaCutoffs | TTCutoffs | LMR Reductions | Null Cutoffs | Killer Hits | Hist Hits |
|---|---|---|---|---|---|---|---|---|
| Baseline | 70,476 | 55,015 | 3,456 | 569 | 153 | 27 | 758 | 20 |
| No Null Move | 78,102 | 60,021 | 4,265 | 699 | 153 | 0 | 1,061 | 29 |
| No LMR | 77,459 | 58,554 | 3,605 | 505 | 0 | 27 | 840 | 21 |
| No Killer | 70,636 | 54,943 | 3,529 | 565 | 122 | 27 | 793 | 28 |
| No History | 71,638 | 56,141 | 3,456 | 569 | 153 | 27 | 757 | 17 |
