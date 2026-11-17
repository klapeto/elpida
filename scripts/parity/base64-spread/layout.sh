#!/bin/bash
# Address-layout test: does the run-to-run spread go away when the layout is fixed?
# See docs/development/base64-decode-spread.md ("Address layout").
#
#   bash layout.sh [ROUNDS] [INDEXES]       (in the directory build.sh made: OUT_DIR/layout)
#
# Runs each web benchmark index interleaved, pinned with Elpida's own --affinity=2, as
#   normal   web-find                          (ASLR + mimalloc random start)
#   norand   web-fixed-start                   (ASLR only)
#   noaslr   setarch -R web-find               (mimalloc random start only)
#   fixed    setarch -R web-fixed-start        (neither)
# and prints the spread of each. Defaults: 10 rounds, indexes 0-4 (XML, JSON, Base64 encode, Base64 decode, regex).
cd "$(dirname "$0")"
N=${1:-10}; IDX=${2:-"0 1 2 3 4"}
RAW=raw-layout-$(hostname)-$(date +%Y%m%d-%H%M%S).txt
r() { "$@" --affinity=2 --format=json --config=2048 2>/dev/null | grep -o '"result":[0-9.e+]*' | cut -d: -f2; }
for i in $(seq "$N"); do
	for b in $IDX; do
		echo "$b normal $(r ./web-find --index=$b)"
		echo "$b norand $(r ./web-fixed-start --index=$b)"
		echo "$b noaslr $(r setarch -R ./web-find --index=$b)"
		echo "$b fixed $(r setarch -R ./web-fixed-start --index=$b)"
	done
	echo -n "." >&2
done > "$RAW"
echo >&2
python3 - "$RAW" <<'EOF'
import collections, statistics as st, sys
names = {0: 'XML parsing', 1: 'JSON parsing', 2: 'Base64 encode', 3: 'Base64 decode', 4: 'Regex'}
d = collections.defaultdict(list)
for l in open(sys.argv[1]):
    p = l.split()
    if len(p) == 3: d[(int(p[0]), p[1])].append(float(p[2]))
sp = lambda v: 100 * (max(v) - min(v)) / st.median(v)
kinds = ['normal', 'norand', 'noaslr', 'fixed']
print(f"{'':14}" + "".join(f"{k:>9}" for k in kinds) + "   median fixed/normal")
for b in sorted({k[0] for k in d}):
    v = [d[(b, k)] for k in kinds]
    if not all(v): continue
    print(f"{names.get(b, b):14}" + "".join(f"{sp(x):8.1f}%" for x in v) + f"   {st.median(v[3]) / st.median(v[0]):.3f}")
print("raw:", sys.argv[1])
EOF
