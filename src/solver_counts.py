#!/usr/bin/env python3
"""Accepted-solution (solver) counts across the archive.

For each problem we have the number of distinct users who solved it (from
analysis/ac_counts.json). This is an engagement/popularity signal: it partly
reflects difficulty, but it is confounded by a problem's position in the
archive (people work front-to-back and drop off) and by how approachable its
statement looks. This script joins the counts with statement/editorial/solution
sizes and the thematic clusters, writes a 3x2 figure
(analysis/solver_counts_grid.pdf), and dumps summary numbers
(analysis/solver_counts.json), including rank correlations.
"""
import json
import re
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import tiktoken

ROOT = Path(__file__).resolve().parent.parent
DATASET = ROOT / "dataset"
PROBLEMS = ROOT / "problems"
ANALYSIS = ROOT / "analysis"
ENC = tiktoken.get_encoding("cl100k_base")

BLUE, ORANGE, GREEN = "#3b6ea5", "#d4843a", "#4a8a5c"
plt.rcParams.update({"font.size": 9, "axes.spines.top": False,
                     "axes.spines.right": False, "figure.dpi": 200})


def spearman(x, y):
    """Spearman rank correlation (Pearson on ranks), no scipy dependency."""
    x, y = np.asarray(x, float), np.asarray(y, float)
    rx = np.argsort(np.argsort(x))
    ry = np.argsort(np.argsort(y))
    return float(np.corrcoef(rx, ry)[0, 1])


def primary_solution(pdir):
    cpp = sorted(pdir.glob("*.cpp"))
    if cpp:
        return cpp[0]
    py = sorted(pdir.glob("*.py"))
    return py[0] if py else None


# --- join AC counts with editorial/solution sizes ---------------------------
ac = {int(k): v for k, v in json.loads((ANALYSIS / "ac_counts.json").read_text()).items()}

rows = []  # (id, ac, ed_tokens, sol_lines)
for p in sorted(DATASET.glob("p*.txt")):
    if p.name.endswith("_raw.txt") or p.name.endswith("_finetune.txt"):
        continue
    num = int(re.match(r"p(\d+)", p.name).group(1))
    if not ac.get(num):           # skip unsolved / missing counts
        continue
    ed_tok = len(ENC.encode(p.read_text(encoding="utf-8", errors="ignore")))
    sol = primary_solution(PROBLEMS / f"p{num}")
    sol_lines = sol.read_text(encoding="utf-8", errors="ignore").count("\n") + 1 if sol else 0
    stmt = PROBLEMS / f"p{num}" / "statement.txt"
    stmt_tok = len(ENC.encode(stmt.read_text(encoding="utf-8", errors="ignore"))) if stmt.exists() else 0
    rows.append((num, ac[num], ed_tok, sol_lines, stmt_tok))

ids = np.array([r[0] for r in rows])
acv = np.array([r[1] for r in rows])
edt = np.array([r[2] for r in rows])
soll = np.array([r[3] for r in rows])
stmtt = np.array([r[4] for r in rows])

# --- per-cluster median difficulty ------------------------------------------
clusters = json.loads((ANALYSIS / "clusters.json").read_text())
ac_by_id = dict(zip(ids.tolist(), acv.tolist()))
cluster_rows = []
for ci, info in clusters.items():
    vals = [ac_by_id[p["id"]] for p in info["problems"] if p["id"] in ac_by_id]
    if vals:
        cluster_rows.append((info.get("tag", f"C{ci}"), int(np.median(vals)), len(vals)))
cluster_rows.sort(key=lambda r: r[1])  # hardest (lowest median) first

# Rank correlations of solver count with each size/order signal (we report
# these simply as "correlation" in the paper).
corr_stmt = round(spearman(acv, stmtt), 3)
corr_ed = round(spearman(acv, edt), 3)
corr_sol = round(spearman(acv, soll), 3)
corr_num = round(spearman(acv, ids), 3)

summary = {
    "n": len(rows),
    "corr_ac_vs_statement_tokens": corr_stmt,
    "corr_ac_vs_editorial_tokens": corr_ed,
    "corr_ac_vs_solution_lines": corr_sol,
    "corr_ac_vs_problem_number": corr_num,
    "median_ac": int(np.median(acv)),
    "fewest_solvers": [(int(i), int(a)) for i, a in sorted(zip(ids, acv), key=lambda t: t[1])[:8]],
    "most_solvers": [(int(i), int(a)) for i, a in sorted(zip(ids, acv), key=lambda t: -t[1])[:8]],
    "cluster_median_ac": [{"tag": t, "median_ac": m, "n": n} for t, m, n in cluster_rows],
}
(ANALYSIS / "solver_counts.json").write_text(json.dumps(summary, indent=2))
print(json.dumps(summary, indent=2))

# --- figure: 3x2 difficulty grid (constrained layout, no overlap) -----------
def corr_box(ax, r):
    """Annotate a scatter panel with its correlation, in-panel (no overlap)."""
    ax.text(0.95, 0.95, f"correlation {r:+.2f}", transform=ax.transAxes,
            ha="right", va="top", fontsize=8.5,
            bbox=dict(boxstyle="round,pad=0.25", fc="white", ec="0.7", lw=0.5))


def scatter_vs_ac(ax, y, ylabel, color, r):
    ax.scatter(acv, y, s=9, alpha=0.4, color=color, edgecolors="none")
    ax.set_xscale("log")
    ax.set_xlabel("Accepted-solution count (log)")
    ax.set_ylabel(ylabel)
    corr_box(ax, r)


fig, axes = plt.subplots(3, 2, figsize=(7.0, 7.6), constrained_layout=True)

ax = axes[0, 0]
ax.hist(acv, bins=np.logspace(0, np.log10(acv.max()), 30), color=BLUE,
        edgecolor="white", linewidth=0.4)
ax.set_xscale("log")
ax.set_xlabel("Accepted-solution count (log)")
ax.set_ylabel("Number of problems")
ax.set_title("(a)  Solver-count distribution", loc="left", fontweight="bold", fontsize=10)

ax = axes[0, 1]
scatter_vs_ac(ax, stmtt, "Statement tokens", GREEN, corr_stmt)
ax.set_title("(b)  vs. statement length", loc="left", fontweight="bold", fontsize=10)

ax = axes[1, 0]
scatter_vs_ac(ax, edt, "Editorial tokens", BLUE, corr_ed)
ax.set_title("(c)  vs. editorial length", loc="left", fontweight="bold", fontsize=10)

ax = axes[1, 1]
scatter_vs_ac(ax, soll, "Solution lines", BLUE, corr_sol)
ax.set_title("(d)  vs. solution length", loc="left", fontweight="bold", fontsize=10)

ax = axes[2, 0]
ax.scatter(ids, acv, s=9, alpha=0.4, color=GREEN, edgecolors="none")
ax.set_yscale("log")
ax.set_xlabel("Problem number")
ax.set_ylabel("Accepted-solution count (log)")
corr_box(ax, corr_num)
ax.set_title("(e)  vs. problem number", loc="left", fontweight="bold", fontsize=10)

ax = axes[2, 1]
tags = [r[0] for r in cluster_rows]
meds = [r[1] for r in cluster_rows]
ypos = np.arange(len(tags))
ax.barh(ypos, meds, color=ORANGE, edgecolor="white", linewidth=0.4)
ax.set_yticks(ypos)
ax.set_yticklabels(tags, fontsize=7)
ax.invert_yaxis()
ax.set_xlabel("Median accepted-solution count")
ax.set_title("(f)  Median by cluster", loc="left", fontweight="bold", fontsize=10)

fig.savefig(ANALYSIS / "solver_counts_grid.pdf")
plt.close(fig)
print(f"\nWrote solver_counts_grid.pdf and solver_counts.json to {ANALYSIS}")
