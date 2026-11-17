#!/bin/bash
# Runs every benchmark of the full run, single-threaded, with the Linux build natively and the Windows build under
# Wine, interleaved, on one core, and prints the median scores and the Windows/Linux ratio.
# See docs/development/parity-checks.md ("Linux vs Windows on one machine").
#
#   linux-vs-windows.sh LINUX_BENCHMARKS_DIR WINDOWS_BENCHMARKS_DIR [ROUNDS] [CORE]
#
# The directories hold the *-benchmarks executables (build/apps/Benchmarks of each target). WINE is the wine64 to use
# (default: wine64 on PATH; Proton's files/bin/wine64 works too), WINEPREFIX the prefix (default ~/.wine-elpida).
# The timed code makes no Windows API calls, so under Wine it runs natively; multi-thread results under Wine are not
# meaningful (Linux schedules the threads).
set -u
LIN=$(realpath "$1"); WIN=$(realpath "$2"); ROUNDS=${3:-7}; CORE=${4:-2}
WINE=${WINE:-wine64}
export WINEPREFIX=${WINEPREFIX:-$HOME/.wine-elpida} WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml="

# name|group|index|configuration (the values apps/Application/FullBenchmarkInstances sets)
LIST=(
"AES Decryption|encryption|3|--config=131072"
"AES Encryption|encryption|2|--config=131072"
"Base64 Decode|web|3|--config=2048"
"Base64 Encode|web|2|--config=2048"
"C++ Compilation|compilation|0|"
"FFT|math|0|--config=2048"
"Json Parsing|web|1|--config=2048"
"Matrix inverse 4x4|math|3|"
"Matrix mult 32x32|math|2|"
"NBody|math|4|"
"Ray Tracing|image|5|--config=32"
"Regex|web|4|--config=2048"
"RSA Decryption|encryption|1|"
"RSA Encryption|encryption|0|"
"SVG Rasterization|image|2|--config=1.0"
"XML Parsing|web|0|--config=2048"
"Zlib compression|compression|0|--config=16384"
"Zlib decompression|compression|1|--config=16384"
)

"$WINE" wineboot -i >/dev/null 2>&1
result() { "$@" 2>/dev/null | grep -o '"result":[0-9.e+-]*' | cut -d: -f2; }
out=$(mktemp)
for i in $(seq "$ROUNDS"); do
	for e in "${LIST[@]}"; do
		IFS='|' read -r name group index config <<< "$e"
		# shellcheck disable=SC2086
		args="--index=$index --affinity=$CORE --format=json $config"
		l=$(cd "$LIN" && result ./$group-benchmarks $args)
		w=$(cd "$WIN" && result "$WINE" ./$group-benchmarks.exe $args)
		echo "$name|$l|$w" >> "$out"
	done
done

python3 - "$out" <<'PY'
import sys, statistics, collections
lin, win, order = collections.defaultdict(list), collections.defaultdict(list), []
for line in open(sys.argv[1]):
    name, l, w = line.rstrip("\n").split("|")
    if name not in order:
        order.append(name)
    if l: lin[name].append(float(l))
    if w: win[name].append(float(w))
print(f"{'benchmark':22} {'linux':>12} {'windows':>12} {'win/lin':>8}  spread linux / windows")
for name in order:
    if lin[name] and win[name]:
        ml, mw = statistics.median(lin[name]), statistics.median(win[name])
        sl = 100 * (max(lin[name]) - min(lin[name])) / ml
        sw = 100 * (max(win[name]) - min(win[name])) / mw
        print(f"{name:22} {ml:12.5g} {mw:12.5g} {mw / ml:8.3f}  {sl:4.1f}% / {sw:4.1f}%")
    else:
        print(f"{name:22} missing results: linux {len(lin[name])}, windows {len(win[name])}")
PY
rm -f "$out"
