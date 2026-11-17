#!/usr/bin/env python3
"""Summary of a spread sweep (sweep.sh): per benchmark, the run-to-run spread, the IPC, the spread of the
instructions per result (the work should be the same in every run), the mispredicts per 1000 instructions and the
three counters that follow the score most closely (correlation with the score; mean rate per 1000 instructions in
the slowest and the fastest third of the runs).

Runs whose cycle count is more than 0.5% below the median are left out (column n: kept-dropped): they lost time to
another process on the same CPU, which perf stat does not count for us.

    python3 summary.py OUT_DIR
"""
import glob, os, statistics as st, sys

def corr(a, b):
    ma, mb = st.mean(a), st.mean(b)
    n = sum((x - ma) * (y - mb) for x, y in zip(a, b))
    d = (sum((x - ma) ** 2 for x in a) * sum((y - mb) ** 2 for y in b)) ** .5
    return n / d if d else 0.0

def num(v):
    try: return float(v)
    except ValueError: return float('nan')

SHORT = {'br_misp_retired.all_branches': 'misp', 'mem_load_retired.l1_miss': 'L1miss',
         'ld_blocks.store_forward': 'stfwd', 'ld_blocks_partial.address_alias': '4Kalias',
         'machine_clears.count': 'clears', 'machine_clears.memory_ordering': 'memord',
         'idq.dsb_uops': 'dsb', 'idq.mite_uops': 'mite', 'dsb2mite_switches.penalty_cycles': 'dsb2mite',
         'frontend_retired.dsb_miss': 'dsbmiss', 'baclears.any': 'baclears', 'resource_stalls.sb': 'sbstall'}

print(f"{'benchmark':22} {'n':>5} {'spread':>7} {'IPC':>5} {'instr/res':>9} {'misp/1k':>8}  top counters (corr with score; slow->fast per 1k instr)")
for f in sorted(glob.glob(os.path.join(sys.argv[1], '*.csv'))):
    rows = []
    for l in open(f):
        p = l.strip().split(',')
        if not p[0]: continue
        r = {'score': float(p[0])}
        for kv in p[1:]:
            k, v = kv.split('=', 1); r[k] = num(v)
        rows.append(r)
    mc = st.median(r['cycles'] for r in rows)
    dropped = sum(1 for r in rows if r['cycles'] < 0.995 * mc)
    rows = [r for r in rows if r['cycles'] >= 0.995 * mc]   # runs that lost time to another process
    if len(rows) < 3: print(os.path.basename(f), 'too few rows'); continue
    sc = [r['score'] for r in rows]; m = st.median(sc)
    ipc = st.median(r['instructions'] / r['cycles'] for r in rows)
    ipr = [r['instructions'] / r['score'] for r in rows]
    ipr_spread = 100 * (max(ipr) - min(ipr)) / st.median(ipr)
    mk = 'br_misp_retired.all_branches' if 'br_misp_retired.all_branches' in rows[0] else 'branch-misses'
    misp = st.median(1000 * r[mk] / r['instructions'] for r in rows) if mk in rows[0] else float('nan')
    rows.sort(key=lambda r: r['score']); k3 = max(1, len(rows) // 3); slow, fast = rows[:k3], rows[-k3:]
    sc = [r['score'] for r in rows]   # same order as the counter lists below
    tops = []
    for k, s in list(SHORT.items()) + [('branch-misses', 'misp')]:
        if k not in rows[0]: continue
        v = [1000 * r[k] / r['instructions'] for r in rows]
        if any(x != x for x in v): continue
        c = corr(sc, v)
        a, b = st.mean(1000 * r[k] / r['instructions'] for r in slow), st.mean(1000 * r[k] / r['instructions'] for r in fast)
        tops.append((abs(c), c, s, a, b))
    tops.sort(reverse=True)
    t = '  '.join(f"{s} {c:+.2f} ({a:.3g}->{b:.3g})" for _, c, s, a, b in tops[:3])
    name = os.path.basename(f)[:-4].replace('_', ' ')
    print(f"{name:22} {len(rows):2}-{dropped} {100 * (max(sc) - min(sc)) / m:6.1f}% {ipc:5.2f} {ipr_spread:8.2f}% {misp:8.3f}  {t}")
