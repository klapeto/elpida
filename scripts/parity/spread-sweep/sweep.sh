#!/bin/bash
# Run-to-run spread sweep: N runs of every benchmark under perf stat, pinned to one CPU, one CSV per benchmark.
# See docs/development/run-to-run-spread.md.
#
#   bash sweep.sh BENCHMARKS_DIR OUT_DIR [N] [EXTRA...]
#
# BENCHMARKS_DIR holds the *-benchmarks executables (a Linux build's apps/Benchmarks, or a parity package's
# Benchmarks). N defaults to 15. Each EXTRA is "Name|/path/to/binary|args" for builds to compare, e.g.
#   "Base64 Decode final|$HOME/web-final|--index=3 --config=2048"
# Run on an idle machine with the performance governor and kernel.perf_event_paranoid <= 1 (perf-stat.sh). Summary:
#   python3 summary.py OUT_DIR
# Takes about 75 s per benchmark with N=15 (5 s per run). CPU and EVENTS are passed through to perf-stat.sh.
HERE=$(cd "$(dirname "$0")" && pwd)
PERF_STAT=$HERE/../base64-spread/perf-stat.sh
BENCH=$(cd "${1:?usage: sweep.sh BENCHMARKS_DIR OUT_DIR [N] [EXTRA...]}" && pwd)
OUT=$(realpath -m "${2:?usage: sweep.sh BENCHMARKS_DIR OUT_DIR [N] [EXTRA...]}")
N=${3:-15}
shift 3 2>/dev/null || shift $#
mkdir -p "$OUT"
L=(
"AES Decryption|$BENCH/encryption-benchmarks|--index=3 --config=131072"
"AES Encryption|$BENCH/encryption-benchmarks|--index=2 --config=131072"
"Base64 Decode|$BENCH/web-benchmarks|--index=3 --config=2048"
"Base64 Encode|$BENCH/web-benchmarks|--index=2 --config=2048"
"C++ Compilation|$BENCH/compilation-benchmarks|--index=0"
"FFT|$BENCH/math-benchmarks|--index=0 --config=2048"
"Json Parsing|$BENCH/web-benchmarks|--index=1 --config=2048"
"Matrix inverse 4x4|$BENCH/math-benchmarks|--index=3"
"Matrix mult 32x32|$BENCH/math-benchmarks|--index=2"
"NBody|$BENCH/math-benchmarks|--index=4"
"Ray Tracing|$BENCH/image-benchmarks|--index=5 --config=32"
"Regex|$BENCH/web-benchmarks|--index=4 --config=2048"
"RSA Decryption|$BENCH/encryption-benchmarks|--index=1"
"RSA Encryption|$BENCH/encryption-benchmarks|--index=0"
"SVG Rasterization|$BENCH/image-benchmarks|--index=2 --config=1 --config=0.06 --config=32"
"XML Parsing|$BENCH/web-benchmarks|--index=0 --config=2048"
"Zlib compression|$BENCH/compression-benchmarks|--index=0 --config=16384"
"Zlib decompression|$BENCH/compression-benchmarks|--index=1 --config=16384"
"$@"
)
for e in "${L[@]}"; do
	[ -n "$e" ] || continue
	IFS='|' read -r name bin args <<< "$e"
	f="$OUT/$(echo "$name" | tr ' +/' '_pp').csv"
	echo "$(date +%T) $name"
	# shellcheck disable=SC2086
	bash "$PERF_STAT" "$N" "$bin" $args > "$f"
done
echo "$(date +%T) done"
