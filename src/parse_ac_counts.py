#!/usr/bin/env python3
"""Parse the SGU problem list (problem -> number of accepted solutions).

Input is analysis/ac_counts_raw.txt, copied from the acmsguru problem index,
where each row looks like:

    100    A+B*special problem    Submit Add to favourites     x13646

The trailing "xN" is the number of users with an accepted solution; problems
with no accepted solution (e.g. 145) have no count. We use this count as a
coarse difficulty proxy: fewer solvers => harder. Output is a sorted
analysis/ac_counts.json mapping problem id -> count (null if unsolved).
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RAW = ROOT / "analysis" / "ac_counts_raw.txt"
OUT = ROOT / "analysis" / "ac_counts.json"

# Each record: leading id, a title, the "Submit Add to favourites" marker, and
# an optional "xN" solver count. Records may be run together by stray "then"
# connectors from the page dump, so we scan globally rather than line by line.
RECORD = re.compile(
    r"(\d+)\s+(.*?)\s+Submit Add to favourites\s*(?:x(\d+))?(?=\s*(?:then\s+)?\d+\s|\s*$)",
    re.IGNORECASE,
)


def parse(text):
    # Drop comment/provenance lines (e.g. the leading "# ... as of <date>")
    # so their digits cannot leak into the first record.
    text = "\n".join(ln for ln in text.splitlines() if not ln.lstrip().startswith("#"))
    counts = {}
    for m in RECORD.finditer(text):
        pid = int(m.group(1))
        ac = int(m.group(3)) if m.group(3) else None
        counts[pid] = ac
    return counts


def main():
    text = RAW.read_text(encoding="utf-8")
    counts = parse(text)
    ordered = {str(k): counts[k] for k in sorted(counts)}
    OUT.write_text(json.dumps(ordered, indent=2))

    solved = {k: v for k, v in counts.items() if v}
    unsolved = sorted(k for k, v in counts.items() if not v)
    print(f"parsed {len(counts)} problems "
          f"({len(solved)} solved, unsolved: {unsolved})")
    hardest = sorted(solved.items(), key=lambda kv: kv[1])[:5]
    easiest = sorted(solved.items(), key=lambda kv: -kv[1])[:5]
    print("fewest solvers:", [f"p{p}={c}" for p, c in hardest])
    print("most solvers:  ", [f"p{p}={c}" for p, c in easiest])
    print(f"wrote {OUT}")


if __name__ == "__main__":
    main()
