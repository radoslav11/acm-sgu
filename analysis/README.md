# analysis/

Generated data and figures for the SGU-Editorial paper. The scripts that
produce these live in `../src/`; install their deps with
`pip install -r ../src/requirements.txt`. Run the scripts from the repo root.

## Files

| File | Produced by | What it is |
|------|-------------|------------|
| `stats.json`, `stats_grid.pdf` | `src/dataset_stats.py` | token/length stats and the 2x2 stats figure |
| `clusters.json`, `clusters.md`, `clusters_tsne.pdf` | `src/cluster_editorials.py` | thematic clusters (per-problem titles) and the tagged t-SNE |
| `ac_counts_raw.txt` | hand-pasted from acmsguru | raw problem index: id, title, accepted-solution count |
| `ac_counts.json` | `src/parse_ac_counts.py` | parsed map: problem id -> solver count (null if unsolved) |
| `solver_counts.json`, `solver_counts_grid.pdf` | `src/solver_counts.py` | accepted-solution-count correlations and the 3x2 figure |
| `embedding_analysis.json`, `embedding_grid.pdf`, `duplicate_pairs.tex` | `src/embedding_analysis.py` | redundancy/diversity stats, figure, and the near-duplicate table |
| `.embed_cache.npz` | `src/cluster_editorials.py` | embedding cache (gitignored) |

## Regenerate

```bash
python3 src/dataset_stats.py          # stats + figure
python3 src/parse_ac_counts.py        # ac_counts.json from the raw dump
python3 src/solver_counts.py          # solver-count figure + correlations
python3 src/embedding_analysis.py     # redundancy/diversity (reuses the cache)
python3 src/cluster_editorials.py --k 8   # clusters (needs OPENAI_API_KEY)
```
