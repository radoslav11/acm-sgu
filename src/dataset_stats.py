#!/usr/bin/env python3
"""Compute dataset statistics and render figures for the SGU-Editorial paper.

Reads the editorials in dataset/ and the reference solutions in problems/,
tokenizes the editorials with tiktoken (cl100k_base), and writes figures to
analysis/ plus a machine-readable summary to analysis/stats.json.
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
OUT = ROOT / "analysis"
OUT.mkdir(parents=True, exist_ok=True)

ENC = tiktoken.get_encoding("cl100k_base")

# Muted, print-friendly palette so the figures read well in a two-column paper.
BLUE, ORANGE, GREEN = "#3b6ea5", "#d4843a", "#4a8a5c"
plt.rcParams.update({
    "font.size": 9,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "figure.dpi": 200,
})


def editorial_paths():
    for p in sorted(DATASET.glob("p*.txt")):
        if p.name.endswith("_raw.txt") or p.name.endswith("_finetune.txt"):
            continue
        yield p


def problem_number(name):
    return int(re.match(r"p(\d+)", name).group(1))


def primary_solution(pdir):
    """Return (path, language) for the reference solution, preferring C++."""
    cpp = sorted(pdir.glob("*.cpp"))
    if cpp:
        return cpp[0], "C++"
    py = sorted(pdir.glob("*.py"))
    if py:
        return py[0], "Python"
    return None, None


# --- gather editorial token counts -----------------------------------------
ed_numbers, ed_tokens = [], []
for p in editorial_paths():
    text = p.read_text(encoding="utf-8", errors="ignore")
    ed_numbers.append(problem_number(p.name))
    ed_tokens.append(len(ENC.encode(text)))
ed_numbers = np.array(ed_numbers)
ed_tokens = np.array(ed_tokens)

# --- gather solution stats ---------------------------------------------------
sol_lines, sol_tokens, lang_counts = [], [], {"C++": 0, "Python": 0}
for pdir in sorted(PROBLEMS.glob("p*")):
    if not pdir.is_dir():
        continue
    path, lang = primary_solution(pdir)
    if path is None:
        continue
    lang_counts[lang] += 1
    code = path.read_text(encoding="utf-8", errors="ignore")
    sol_lines.append(code.count("\n") + 1)
    sol_tokens.append(len(ENC.encode(code)))
sol_lines = np.array(sol_lines)
sol_tokens = np.array(sol_tokens)

summary = {
    "n_editorials": int(len(ed_tokens)),
    "n_solutions": int(lang_counts["C++"] + lang_counts["Python"]),
    "n_cpp": lang_counts["C++"],
    "n_python": lang_counts["Python"],
    "editorial_tokens_total": int(ed_tokens.sum()),
    "editorial_tokens_mean": float(ed_tokens.mean()),
    "editorial_tokens_median": float(np.median(ed_tokens)),
    "editorial_tokens_min": int(ed_tokens.min()),
    "editorial_tokens_max": int(ed_tokens.max()),
    "solution_tokens_total": int(sol_tokens.sum()),
    "solution_tokens_mean": float(sol_tokens.mean()),
    "solution_lines_mean": float(sol_lines.mean()),
    "solution_lines_median": float(np.median(sol_lines)),
    "dataset_tokens_total": int(ed_tokens.sum() + sol_tokens.sum()),
}
(OUT / "stats.json").write_text(json.dumps(summary, indent=2))
print(json.dumps(summary, indent=2))

# Pair editorial/solution tokens per problem (for panel d).
pairs = {"C++": ([], []), "Python": ([], [])}
ed_by_num = dict(zip(ed_numbers.tolist(), ed_tokens.tolist()))
for pdir in sorted(PROBLEMS.glob("p*")):
    if not pdir.is_dir():
        continue
    num = problem_number(pdir.name)
    if num not in ed_by_num:
        continue
    path, lang = primary_solution(pdir)
    if path is None:
        continue
    code = path.read_text(encoding="utf-8", errors="ignore")
    pairs[lang][0].append(len(ENC.encode(code)))
    pairs[lang][1].append(ed_by_num[num])


def draw_editorial_hist(ax):
    ax.hist(ed_tokens, bins=30, color=BLUE, edgecolor="white", linewidth=0.4)
    ax.axvline(ed_tokens.mean(), color=ORANGE, linestyle="--", linewidth=1.2,
               label=f"mean {ed_tokens.mean():.0f}")
    ax.set_xlabel("Tokens per editorial")
    ax.set_ylabel("Number of problems")
    ax.legend(frameon=False, fontsize=8)


def draw_solution_hist(ax):
    ax.hist(sol_lines, bins=30, color=GREEN, edgecolor="white", linewidth=0.4)
    ax.axvline(sol_lines.mean(), color=ORANGE, linestyle="--", linewidth=1.2,
               label=f"mean {sol_lines.mean():.0f}")
    ax.set_xlabel("Lines per reference solution")
    ax.set_ylabel("Number of problems")
    ax.legend(frameon=False, fontsize=8)


def draw_cumulative(ax):
    order = np.argsort(ed_numbers)
    ax.plot(ed_numbers[order], np.cumsum(ed_tokens[order]) / 1000.0,
            color=BLUE, linewidth=1.5)
    ax.set_xlabel("Problem number")
    ax.set_ylabel("Cumulative editorial\ntokens (thousands)")


def draw_scatter(ax):
    for lang, color in (("C++", BLUE), ("Python", ORANGE)):
        xs, ys = pairs[lang]
        ax.scatter(xs, ys, s=10, alpha=0.5, color=color,
                   label=f"{lang} ({len(xs)})", edgecolors="none")
    ax.set_xlabel("Solution tokens")
    ax.set_ylabel("Editorial tokens")
    ax.legend(frameon=False, fontsize=8)


# Single 2x2 grid with constrained_layout so matplotlib spaces the panels and
# never lets axis labels collide (the LaTeX-side stitching used to overlap).
panels = [("a", draw_editorial_hist), ("b", draw_solution_hist),
          ("c", draw_cumulative), ("d", draw_scatter)]
fig, axes = plt.subplots(2, 2, figsize=(7.2, 5.0), constrained_layout=True)
for (letter, draw), ax in zip(panels, axes.flat):
    draw(ax)
    ax.set_title(f"({letter})", loc="left", fontweight="bold", fontsize=11)
fig.savefig(OUT / "stats_grid.pdf")
plt.close(fig)

print(f"\nWrote stats_grid.pdf and stats.json to {OUT}")
