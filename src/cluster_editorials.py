#!/usr/bin/env python3
"""Cluster SGU-Editorial problems into groups of related algorithmic ideas.

Pipeline (following the OpenAI embeddings-clustering recipe):
  1. Embed every editorial with an OpenAI embedding model (cached to disk).
  2. Cluster the embeddings with K-means; pick K by silhouette score if not given.
  3. Ask a chat model to assign each cluster a short, human-readable theme by
     showing it a sample of the editorials it contains.
  4. Write a 2-D t-SNE scatter (analysis/clusters_tsne.pdf) and a
     machine-readable assignment (analysis/clusters.json) plus a readable
     analysis/clusters.md summary. Each problem is listed with its SGU
     title so the clusters are skimmable.

This is intended to be run ONCE the full editorial set is finalized, so the
taxonomy reflects the whole archive. It requires OPENAI_API_KEY in the
environment.

Usage:
    python3 analysis/cluster_editorials.py [--k 16]
        [--embed-model text-embedding-3-large] [--label-model gpt-5.1] [--no-label]
"""
import argparse
import hashlib
import json
import os
import re
import sys
import time
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parent.parent
DATASET = ROOT / "dataset"
PROBLEMS = ROOT / "problems"
OUT = ROOT / "analysis"
CACHE = OUT / ".embed_cache.npz"
OUT.mkdir(parents=True, exist_ok=True)

# Each editorial is truncated to this many characters before embedding. The
# abridged statement + detailed editorial live at the top, which is the part
# that carries the algorithmic "idea", so the head is what we want.
MAX_CHARS = 8000


def problem_title(num):
    """The SGU problem title, e.g. 'A+B', read from the first line of its
    statement (format 'NNN. Title'). Returns '' if unavailable."""
    stmt = PROBLEMS / f"p{num}" / "statement.txt"
    if not stmt.exists():
        return ""
    for line in stmt.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = line.strip()
        if not line:
            continue
        m = re.match(r"\d+\.\s*(.+)", line)
        return m.group(1).strip() if m else line
    return ""


def load_editorials():
    """Return (numbers, texts, titles) for every editorial file."""
    numbers, texts, titles = [], [], []
    for p in sorted(DATASET.glob("p*.txt")):
        if p.name.endswith("_raw.txt") or p.name.endswith("_finetune.txt"):
            continue
        num = int(re.match(r"p(\d+)", p.name).group(1))
        text = p.read_text(encoding="utf-8", errors="ignore")[:MAX_CHARS]
        numbers.append(num)
        texts.append(text)
        titles.append(problem_title(num))
    return numbers, texts, titles


def embed(texts, model, batch_size=64):
    """Embed texts with OpenAI, caching by (model, text-hash) to avoid re-billing."""
    from openai import OpenAI

    cache = {}
    if CACHE.exists():
        data = np.load(CACHE, allow_pickle=True)
        cache = {k: data[k] for k in data.files}

    keys = [f"{model}:{hashlib.md5(t.encode('utf-8')).hexdigest()}" for t in texts]
    missing = [(i, t, k) for i, (t, k) in enumerate(zip(texts, keys)) if k not in cache]

    if missing:
        client = OpenAI()
        print(f"Embedding {len(missing)} editorials with {model} "
              f"({len(texts) - len(missing)} cached)...", file=sys.stderr)
        for start in range(0, len(missing), batch_size):
            chunk = missing[start:start + batch_size]
            for attempt in range(5):
                try:
                    resp = client.embeddings.create(
                        model=model, input=[t for _, t, _ in chunk])
                    break
                except Exception as e:  # transient API errors -> backoff
                    wait = 2 ** attempt
                    print(f"  retry in {wait}s ({e})", file=sys.stderr)
                    time.sleep(wait)
            else:
                raise RuntimeError("embedding failed after retries")
            for (_, _, k), item in zip(chunk, resp.data):
                cache[k] = np.array(item.embedding, dtype=np.float32)
        np.savez(CACHE, **cache)

    return np.vstack([cache[k] for k in keys])


def choose_k(X, k_range):
    """Pick K by best silhouette score over the given range."""
    from sklearn.cluster import KMeans
    from sklearn.metrics import silhouette_score

    best_k, best_score = k_range[0], -1.0
    for k in k_range:
        labels = KMeans(n_clusters=k, n_init=10, random_state=0).fit_predict(X)
        score = silhouette_score(X, labels)
        print(f"  k={k:2d}  silhouette={score:.4f}", file=sys.stderr)
        if score > best_score:
            best_k, best_score = k, score
    print(f"Selected k={best_k} (silhouette={best_score:.4f})", file=sys.stderr)
    return best_k


def label_cluster(numbers, texts, titles, members, model):
    """Ask a chat model for a short theme describing a cluster of editorials."""
    from openai import OpenAI

    client = OpenAI()
    sample = members[:8]
    blurbs = []
    for idx in sample:
        head = texts[idx][:600].replace("\n", " ")
        blurbs.append(f"Problem {numbers[idx]} ({titles[idx]}): {head}")
    prompt = (
        "Below are abridged competitive-programming editorials that were "
        "grouped into one cluster. Provide: a very short tag (1-2 words, "
        "Title Case, suitable as an on-figure label), a longer label (3-6 "
        "words) naming the common algorithmic theme, and one sentence "
        "justifying it. Respond as JSON: "
        "{\"tag\": ..., \"label\": ..., \"reason\": ...}.\n\n"
        + "\n\n".join(blurbs)
    )
    resp = client.chat.completions.create(
        model=model,
        messages=[{"role": "user", "content": prompt}],
        response_format={"type": "json_object"},
    )
    try:
        return json.loads(resp.choices[0].message.content)
    except json.JSONDecodeError:
        return {"label": "unlabeled", "reason": resp.choices[0].message.content[:200]}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--k", type=int, default=0, help="cluster count (0 = auto)")
    ap.add_argument("--embed-model", default="text-embedding-3-large")
    ap.add_argument("--label-model", default="gpt-5.1")
    ap.add_argument("--no-label", action="store_true", help="skip LLM labeling")
    args = ap.parse_args()

    if not os.environ.get("OPENAI_API_KEY"):
        sys.exit("OPENAI_API_KEY is not set; this script needs the OpenAI API.")

    numbers, texts, titles = load_editorials()
    print(f"Loaded {len(texts)} editorials.", file=sys.stderr)

    X = embed(texts, args.embed_model)
    # Cosine geometry: normalize so K-means / t-SNE operate on direction.
    X = X / (np.linalg.norm(X, axis=1, keepdims=True) + 1e-9)

    from sklearn.cluster import KMeans
    k = args.k or choose_k(X, range(8, min(28, len(texts) // 8)))
    labels = KMeans(n_clusters=k, n_init=10, random_state=0).fit_predict(X)

    clusters = {}
    for ci in range(k):
        members = [i for i in range(len(texts)) if labels[i] == ci]
        members.sort(key=lambda i: numbers[i])
        info = {"size": len(members),
                "problems": [{"id": numbers[i], "title": titles[i]}
                             for i in members]}
        if not args.no_label:
            meta = label_cluster(numbers, texts, titles, members, args.label_model)
            info.update(meta)
        clusters[str(ci)] = info
        print(f"cluster {ci}: {info.get('label', '?')} ({len(members)} problems)",
              file=sys.stderr)

    (OUT / "clusters.json").write_text(json.dumps(clusters, indent=2))

    # Readable markdown summary: one bullet per problem, with its SGU title.
    lines = ["# SGU-Editorial thematic clusters\n"]
    for ci, info in sorted(clusters.items(), key=lambda kv: -kv[1]["size"]):
        lines.append(f"## {info.get('label', 'cluster ' + ci)} "
                     f"({info['size']} problems)")
        if info.get("reason"):
            lines.append(f"_{info['reason']}_\n")
        for pr in info["problems"]:
            title = pr["title"] or "(untitled)"
            lines.append(f"- p{pr['id']} — {title}")
        lines.append("")
    (OUT / "clusters.md").write_text("\n".join(lines))

    # LaTeX listing of the problem->cluster assignment for the paper appendix.
    tex = []
    for ci, info in sorted(clusters.items(), key=lambda kv: -kv[1]["size"]):
        tag = (info.get("tag") or f"C{ci}").replace("&", "\\&")
        ids = ", ".join(f"p{pr['id']}" for pr in info["problems"])
        tex.append(f"\\noindent\\textbf{{{tag}}} ({info['size']}): {ids}"
                   f"\\par\\smallskip")
    (OUT / "cluster_assignments.tex").write_text("\n".join(tex) + "\n")

    # 2-D t-SNE scatter for the paper.
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import matplotlib.patheffects as pe
    from sklearn.manifold import TSNE

    perp = max(5, min(30, len(texts) // 12))
    coords = TSNE(n_components=2, perplexity=perp, init="pca",
                  random_state=0).fit_transform(X)
    fig, ax = plt.subplots(figsize=(6.5, 5.0))
    cmap = plt.get_cmap("tab10" if k <= 10 else "tab20")
    ax.scatter(coords[:, 0], coords[:, 1], c=labels, cmap=cmap,
               s=16, alpha=0.6, edgecolors="none")
    # Overlay each cluster's short tag at the median position of its points
    # (median is robust to t-SNE outliers), in the cluster's own color.
    for ci in range(k):
        pts = coords[labels == ci]
        cx, cy = np.median(pts[:, 0]), np.median(pts[:, 1])
        tag = clusters[str(ci)].get("tag") or clusters[str(ci)].get("label", f"C{ci}")
        ax.text(cx, cy, tag, fontsize=9, fontweight="bold", ha="center",
                va="center", color="black",
                path_effects=[pe.withStroke(linewidth=2.6, foreground="white")])
    ax.set_xticks([]); ax.set_yticks([])
    for spine in ax.spines.values():
        spine.set_visible(False)
    fig.tight_layout()
    fig.savefig(OUT / "clusters_tsne.pdf")
    print(f"\nWrote clusters.json, clusters.md, clusters_tsne.pdf to {OUT}",
          file=sys.stderr)


if __name__ == "__main__":
    main()
