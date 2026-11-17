#!/bin/bash
# Where are the mispredicted branches? Samples the mispredict event with precise attribution (:pp), so each sample
# lands on (or next to) the branch that mispredicted, and prints the share per function and per instruction.
# See docs/development/base64-decode-spread.md ("How it was profiled").
#
#   bash mispredicts.sh BINARY ARGS...        e.g. bash mispredicts.sh ./web-find --index=3 --config=2048
#
# Then disassemble the top function: objdump -d --no-show-raw-insn BINARY --disassemble=<symbol>
# Needs a binary with symbols (the Linux builds; under Wine the PE's symbols are not seen), perf and
# kernel.perf_event_paranoid <= 1. CPU (default 2), EVENT (default br_misp_retired.all_branches:pp, Intel) and
# PERIOD (default 20011) can be overridden; on AMD precise sampling of this event works differently (IBS), and
# plain EVENT=branch-misses is only approximate.
BIN=$1; shift
CPU=${CPU:-2}; EVENT=${EVENT:-br_misp_retired.all_branches:pp}; PERIOD=${PERIOD:-20011}
D=$(mktemp -d)
taskset -c "$CPU" perf record -q -e "$EVENT" -c "$PERIOD" -o "$D/perf.data" "$BIN" "$@" --format=json 2>/dev/null \
	| grep -o '"result":[0-9.e+]*'
echo "== per function"
perf report -i "$D/perf.data" --stdio -q --sort dso,sym 2>/dev/null | grep -v '^$' | head -10
echo "== per instruction ($(perf script -i "$D/perf.data" -F ip 2>/dev/null | wc -l) samples)"
perf script -i "$D/perf.data" -F ip,sym,symoff 2>/dev/null | awk '{print $2}' | sort | uniq -c | sort -rn | head -10
rm -rf "$D"
