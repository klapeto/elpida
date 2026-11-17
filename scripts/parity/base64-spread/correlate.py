#!/usr/bin/env python3
"""Which hardware counters follow the score? Reads the CSV of perf-stat.sh (score,event=count,...) and prints, per
event, its rate per 1000 instructions in the slowest and the fastest third of the runs and its correlation with the
score. A counter that explains the run-to-run spread has a correlation near -1 (or +1) and differs clearly between the
two thirds.

    python3 correlate.py runs.csv
"""
import statistics as st
import sys


def corr(a, b):
    ma, mb = st.mean(a), st.mean(b)
    num = sum((x - ma) * (y - mb) for x, y in zip(a, b))
    den = (sum((x - ma) ** 2 for x in a) * sum((y - mb) ** 2 for y in b)) ** .5
    return num / den if den else 0.0


def number(text):
    try:
        return float(text)
    except ValueError:  # "<not counted>", "<not supported>"
        return float('nan')


rows = []
for line in open(sys.argv[1]):
    parts = line.strip().split(',')
    if not parts[0]:
        continue
    row = {'score': float(parts[0])}
    for kv in parts[1:]:
        k, v = kv.split('=', 1)
        row[k] = number(v)
    rows.append(row)
rows.sort(key=lambda r: r['score'])

scores = [r['score'] for r in rows]
print(f"n={len(rows)} score min {min(scores):.4g} max {max(scores):.4g} "
      f"spread {100 * (max(scores) - min(scores)) / st.median(scores):.1f}%")
slow, fast = rows[:len(rows) // 3], rows[-(len(rows) // 3):]
metrics = {'IPC': lambda r: r['instructions'] / r['cycles'],
           'instr/score': lambda r: r['instructions'] / r['score']}
for k in rows[0]:
    if k not in ('score', 'instructions'):
        metrics[k] = lambda r, k=k: 1000 * r[k] / r['instructions']
print(f"{'metric (per 1k instr)':40} {'slow third':>12} {'fast third':>12} {'corr w/ score':>14}")
for name, f in metrics.items():
    values = [f(r) for r in rows]
    print(f"{name:40} {st.mean(f(r) for r in slow):12.5g} {st.mean(f(r) for r in fast):12.5g} {corr(scores, values):14.2f}")
