# Search Ablation Results

This document tracks the telemetry results of disabling specific search heuristics to validate their effectiveness.

| Configuration | Nodes | QNodes | BetaCutoffs | TTCutoffs | LMR Reductions | Null Cutoffs | Killer Hits | Hist Hits |
|---|---|---|---|---|---|---|---|---|
| Baseline | 71,076 | 55,460 | 3,461 | 560 | 146 | 27 | 757 | 21 |
| No Null Move | 78,781 | 60,543 | 4,272 | 688 | 146 | 0 | 1,061 | 31 |
| No LMR | 77,459 | 58,554 | 3,605 | 505 | 0 | 27 | 840 | 21 |
| No Killer | 70,503 | 54,853 | 3,492 | 563 | 122 | 27 | 783 | 28 |
| No History | 71,064 | 55,596 | 3,433 | 558 | 126 | 27 | 776 | 18 |
