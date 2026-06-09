#!/usr/bin/env python3
"""Semantic redundancy and diversity of the editorials, from their embeddings.

Standard hygiene/coverage checks for an LLM dataset, all derived from the
cached editorial embeddings (no new API calls):

  * Redundancy / near-duplicates: nearest-neighbour cosine similarity per
    problem, and the most-similar problem pairs.
  * Diversity: the distribution of all pairwise cosine similarities and the
    effective dimensionality (participation ratio) of the embedding cloud.
  * Cluster cohesion: mean intra- vs inter-cluster similarity, a sanity check
    on the thematic clusters.

Outputs analysis/embedding_analysis.json, analysis/embedding_grid.pdf, and
analysis/duplicate_pairs.tex (a LaTeX table of the closest pairs).
Requires the embedding cache produced by cluster_editorials.py.
"""
import hashlib
import json
import re
import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

ROOT = Path(__file__).resolve().parent.parent
DATASET = ROOT / "dataset"
PROBLEMS = ROOT / "problems"
ANALYSIS = ROOT / "analysis"
CACHE = ANALYSIS / ".embed_cache.npz"
EMBED_MODEL = "text-embedding-3-large"
MAX_CHARS = 8000          # must match cluster_editorials.py for cache keys to hit

BLUE, ORANGE, GREEN = "#3b6ea5", "#d4843a", "#4a8a5c"
plt.rcParams.update({"font.size": 9, "axes.spines.top": False,
                     "axes.spines.right": False, "figure.dpi": 200})


def problem_title(num):
    st = PROBLEMS / f"p{num}" / "statement.txt"
    if not st.exists():
        return ""
    for line in st.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = line.strip()
        if line:
            m = re.match(r"\d+\.\s*(.+)", line)
            return m.group(1).strip() if m else line
    return ""


def load_embeddings():
    """Read editorial embeddings straight from the cache (no API)."""
    if not CACHE.exists():
        sys.exit("embedding cache not found; run cluster_editorials.py first.")
    cache = np.load(CACHE, allow_pickle=True)
    nums, titles, vecs = [], [], []
    for p in sorted(DATASET.glob("p*.txt")):
        if p.name.endswith("_raw.txt") or p.name.endswith("_finetune.txt"):
            continue
        num = int(re.match(r"p(\d+)", p.name).group(1))
        text = p.read_text(encoding="utf-8", errors="ignore")[:MAX_CHARS]
        key = f"{EMBED_MODEL}:{hashlib.md5(text.encode('utf-8')).hexdigest()}"
        if key not in cache.files:
            continue
        nums.append(num)
        titles.append(problem_title(num))
        vecs.append(cache[key])
    return np.array(nums), titles, np.vstack(vecs)


nums, titles, X = load_embeddings()
X = X / (np.linalg.norm(X, axis=1, keepdims=True) + 1e-9)
n = len(nums)
print(f"Loaded {n} editorial embeddings.", file=sys.stderr)

# Cosine similarity matrix; ignore the diagonal for neighbour statistics.
S = X @ X.T
np.fill_diagonal(S, -1.0)
nn_sim = S.max(axis=1)                       # closest other problem
iu = np.triu_indices(n, k=1)
pair_sim = S[iu]

# Effective dimensionality: participation ratio of covariance eigenvalues.
Xc = X - X.mean(axis=0, keepdims=True)
eig = np.linalg.eigvalsh(np.cov(Xc, rowvar=False))
eig = np.clip(eig, 0, None)
eff_dim = float((eig.sum() ** 2) / (np.square(eig).sum() + 1e-12))

# Most-similar pairs (candidate near-duplicates).
order = np.argsort(-pair_sim)[:15]
pairs = [(int(nums[iu[0][k]]), titles[iu[0][k]],
          int(nums[iu[1][k]]), titles[iu[1][k]], float(pair_sim[k]))
         for k in order]

# Cluster cohesion: intra- vs inter-cluster mean similarity.
intra = inter = None
cl_path = ANALYSIS / "clusters.json"
if cl_path.exists():
    clusters = json.loads(cl_path.read_text())
    lab = {int(p["id"]): int(ci) for ci, info in clusters.items() for p in info["problems"]}
    labels = np.array([lab.get(int(x), -1) for x in nums])
    same = labels[:, None] == labels[None, :]
    intra = float(S[iu][same[iu]].mean())
    inter = float(S[iu][~same[iu]].mean())

summary = {
    "n": n,
    "mean_pairwise_cosine": round(float(pair_sim.mean()), 3),
    "median_pairwise_cosine": round(float(np.median(pair_sim)), 3),
    "median_nearest_neighbor_cosine": round(float(np.median(nn_sim)), 3),
    "frac_pairs_above_0.8": round(float((pair_sim > 0.8).mean()), 5),
    "effective_dimensions": round(eff_dim, 1),
    "embedding_dim": int(X.shape[1]),
    "intra_cluster_cosine": round(intra, 3) if intra is not None else None,
    "inter_cluster_cosine": round(inter, 3) if inter is not None else None,
    "most_similar_pairs": [
        {"a": a, "a_title": at, "b": b, "b_title": bt, "cosine": round(s, 3)}
        for a, at, b, bt, s in pairs
    ],
}
(ANALYSIS / "embedding_analysis.json").write_text(json.dumps(summary, indent=2))
print(json.dumps(summary, indent=2))

# LaTeX table of the closest pairs for the appendix.
def esc(s):
    return s.replace("&", "\\&").replace("#", "\\#").replace("_", "\\_")

rows = [f"p{a} & {esc(at)} & p{b} & {esc(bt)} & {s:.2f} \\\\"
        for a, at, b, bt, s in pairs[:10]]
# Emit the whole tabular so no row-ending "\\" sits at end-of-file (which would
# swallow the following rule when \input inside a table float).
tex = ["\\begin{tabular}{rlrlr}", "\\toprule",
       "\\multicolumn{2}{c}{Problem A} & \\multicolumn{2}{c}{Problem B} & Cos. \\\\",
       "\\midrule", *rows, "\\bottomrule", "\\end{tabular}"]
(ANALYSIS / "duplicate_pairs.tex").write_text("\n".join(tex) + "\n")

# --- figure: 2x2 embedding grid ---------------------------------------------
fig, axes = plt.subplots(2, 2, figsize=(7.0, 5.2), constrained_layout=True)

ax = axes[0, 0]
ax.hist(nn_sim, bins=30, color=BLUE, edgecolor="white", linewidth=0.4)
ax.axvline(np.median(nn_sim), color=ORANGE, linestyle="--", linewidth=1.2,
           label=f"median {np.median(nn_sim):.2f}")
ax.set_xlabel("Nearest-neighbour cosine similarity")
ax.set_ylabel("Number of problems")
ax.legend(frameon=False, fontsize=8)
ax.set_title("(a)  Redundancy", loc="left", fontweight="bold", fontsize=10)

ax = axes[0, 1]
ax.hist(pair_sim, bins=40, color=GREEN, edgecolor="white", linewidth=0.4)
ax.axvline(pair_sim.mean(), color=ORANGE, linestyle="--", linewidth=1.2,
           label=f"mean {pair_sim.mean():.2f}")
ax.set_xlabel("Pairwise cosine similarity")
ax.set_ylabel("Number of pairs")
ax.legend(frameon=False, fontsize=8)
ax.set_title("(b)  Diversity", loc="left", fontweight="bold", fontsize=10)

ax = axes[1, 0]
cum = np.cumsum(np.sort(eig)[::-1]) / eig.sum()
ax.plot(np.arange(1, len(cum) + 1), cum, color=BLUE, linewidth=1.5)
ax.axhline(0.9, color="0.6", linestyle=":", linewidth=1.0)
ax.set_xlabel("Number of principal components")
ax.set_ylabel("Cumulative variance")
ax.set_title(f"(c)  Effective dim $\\approx${eff_dim:.0f}", loc="left",
             fontweight="bold", fontsize=10)

ax = axes[1, 1]
if intra is not None:
    ax.bar([0, 1], [intra, inter], color=[BLUE, ORANGE], width=0.6,
           edgecolor="white")
    ax.set_xticks([0, 1])
    ax.set_xticklabels(["within\ncluster", "across\nclusters"])
    ax.set_ylabel("Mean cosine similarity")
ax.set_title("(d)  Cluster cohesion", loc="left", fontweight="bold", fontsize=10)

fig.savefig(ANALYSIS / "embedding_grid.pdf")
plt.close(fig)
print(f"\nWrote embedding_analysis.json, embedding_grid.pdf, duplicate_pairs.tex to {ANALYSIS}",
      file=sys.stderr)
