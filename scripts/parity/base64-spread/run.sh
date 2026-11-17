#!/bin/bash
# Base64 decode / web benchmark spread test. Run on an idle machine: bash run.sh [ROUNDS]
# Writes results-<host>-<date>.txt next to this script. Takes about 15 minutes with the default 15 rounds.
cd "$(dirname "$0")"; N=${1:-15}
OUT=$PWD/results-$(hostname)-$(date +%Y%m%d-%H%M%S).txt; RAW=$(mktemp)
BIN=bin/$(uname -m)
[ -d "$BIN" ] || { echo "no binaries for $(uname -m) (have: $(ls bin))"; exit 1; }
chmod +x $BIN/*
temp() { for z in /sys/class/thermal/thermal_zone*/temp; do [ -r "$z" ] && printf "%s=%sC " "$(cat ${z%/temp}/type)" "$(( $(cat $z) / 1000 ))"; done; command -v vcgencmd >/dev/null && vcgencmd get_throttled; }
log() { echo "$@" | tee -a "$OUT"; }
summ() { python3 - "$RAW" <<'PY' | tee -a "$OUT"
import sys,statistics as s,collections as c
d=c.defaultdict(list); order=[]
for l in open(sys.argv[1]):
    p=l.split()
    if p[0] not in order: order.append(p[0])
    if len(p)>=2 and p[1]: d[p[0]].append([float(x) for x in p[1:]])
base=None
for k in order:
    v=d.get(k)
    if not v: print(f"  {k:12} NO RESULT"); continue
    sc=[x[0] for x in v]; m=s.median(sc); base=base or m
    line=f"  {k:12} n={len(sc):2} median {m:12.5g}  spread {100*(max(sc)-min(sc))/m:5.1f}%  vs first {m/base:6.3f}  min {min(sc):.4g} max {max(sc):.4g}"
    if len(v[0])==4:
        mp=[1000*x[3]/x[2] for x in v]; ipc=[x[2]/x[1] for x in v]
        line+=f"  IPC {s.median(ipc):.2f}  misp/1k instr {min(mp):.2f}-{max(mp):.2f}"
    print(line)
print("  raw:", ";".join(f"{k}=" + ",".join(f"{x[0]:.4g}" for x in d.get(k,[])) for k in order))
PY
}
run() { "$@" 2>/dev/null | grep -o '"result":[0-9.e+]*' | cut -d: -f2; }

log "== machine"
log "$(lscpu 2>/dev/null | grep -m1 'Model name' | tr -s ' ' || grep -m1 'model name' /proc/cpuinfo) $(tr -d '\0' 2>/dev/null < /proc/device-tree/model) | $(nproc) threads | $(uname -sr)"
log "governor: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null) | driver: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver 2>/dev/null) | load: $(cut -d' ' -f1-3 /proc/loadavg)"
log "temperature at start: $(temp)"
log "aslr: $(cat /proc/sys/kernel/randomize_va_space) | perf_event_paranoid: $(cat /proc/sys/kernel/perf_event_paranoid)"

log ""; log "== Base64 decode, $N interleaved rounds, not pinned"
: > $RAW
for i in $(seq $N); do for b in find findindex lut; do echo "$b $(run ./$BIN/web-$b --index=3 --config=2048 --format=json)" >> $RAW; done; echo -n "." >&2; done; echo >&2
summ

CPU=$(( $(nproc) > 2 ? 2 : 0 ))
if command -v perf >/dev/null && perf stat -e cycles,instructions,branch-misses true >/dev/null 2>&1; then
  log ""; log "== Base64 decode, $N interleaved rounds, pinned to CPU $CPU, perf stat"
  : > $RAW
  for i in $(seq $N); do for b in find findindex lut; do
    r=$(run taskset -c $CPU perf stat -x, -e cycles,instructions,branch-misses -o /tmp/ps.$$ ./$BIN/web-$b --index=3 --config=2048 --format=json)
    echo "$b $r $(awk -F, '!/^#/&&NF>2{printf "%s ",$1}' /tmp/ps.$$)" >> $RAW; done; echo -n "." >&2; done; echo >&2
  rm -f /tmp/ps.$$; summ
else
  log ""; log "== perf not available (install perf, or: sudo sysctl kernel.perf_event_paranoid=1); pinned runs without counters"
  : > $RAW
  for i in $(seq $N); do for b in find findindex lut; do echo "$b $(run taskset -c $CPU ./$BIN/web-$b --index=3 --config=2048 --format=json)" >> $RAW; done; echo -n "." >&2; done; echo >&2
  summ
fi

log ""; log "== other web benchmarks (find build), $N rounds each, not pinned"
for i in 0 1 2 4; do : > $RAW; name=$(./$BIN/web-find --dump-info | python3 -c "import sys,json;print(json.load(sys.stdin)['benchmarks'][$i]['name'].replace(' ','_'))")
  for r in $(seq $N); do echo "$name $(run ./$BIN/web-find --index=$i --config=2048 --format=json)" >> $RAW; done; echo -n "." >&2; summ; done; echo >&2
rm -f $RAW; log ""; log "temperature at end: $(temp)"; log "done: $OUT"
