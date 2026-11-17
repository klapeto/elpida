#!/bin/bash
# Runs a benchmark N times under perf stat, pinned to one CPU, and prints one CSV line per run:
#   score,event=count,event=count,...
# Feed the output to correlate.py to see which counters follow the score.
#
#   bash perf-stat.sh N BINARY ARGS...        e.g. bash perf-stat.sh 24 ./web-find --index=3 --config=2048
#
# Needs perf and kernel.perf_event_paranoid <= 1. CPU (default 2) and EVENTS can be overridden.
# The default events are Intel Skylake-family names (the i9-10900K); on other CPUs use EVENTS=cycles,instructions,branch-misses
# or the names from `perf list`.
N=$1; BIN=$2; shift 2
CPU=${CPU:-2}
EVENTS=${EVENTS:-cycles,instructions,br_misp_retired.all_branches,mem_load_retired.l1_miss,ld_blocks.store_forward,ld_blocks_partial.address_alias,machine_clears.count,machine_clears.memory_ordering,idq.dsb_uops,idq.mite_uops,dsb2mite_switches.penalty_cycles,frontend_retired.dsb_miss,baclears.any,resource_stalls.sb}
T=$(mktemp)
for i in $(seq "$N"); do
	r=$(taskset -c "$CPU" perf stat -x, -e "$EVENTS" -o "$T" "$BIN" "$@" --format=json 2>/dev/null | grep -o '"result":[0-9.e+]*' | cut -d: -f2)
	echo -n "$r"; awk -F, '!/^#/ && NF>2 {printf ",%s=%s", $3, $1}' "$T"; echo
done
rm -f "$T"
