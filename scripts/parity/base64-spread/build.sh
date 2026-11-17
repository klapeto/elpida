#!/bin/bash
# Builds the Base64 decode test variants and packs the two test packages.
# See docs/development/base64-decode-spread.md.
#
#   scripts/parity/base64-spread/build.sh OUT_DIR [REV]
#
# REV (default HEAD) is exported with `git archive`, plus the submodules at their current checkouts, so the working
# tree is never touched. The patches are made against 2ee5c5e8. Each variant is built for each target in its own
# build directory:
#   find         REV unchanged (base64_chars.find(), memchr)
#   findindex    patches/findindex.patch  (FindIndex() loop in the main decode loop)
#   lut          patches/lut.patch        (256-byte reverse lookup table)
#   fixed-start  patches/mimalloc-fixed-start.patch (no random start of mimalloc's aligned allocations; layout test
#                only, x86_64-linux-gnu only)
# Results:
#   OUT_DIR/elpida-b64-spread.tar.gz          run.sh + bin/x86_64 + bin/aarch64
#   OUT_DIR/elpida-b64-spread-windows.zip     run.ps1 + bin/*.exe
#   OUT_DIR/layout/web-{find,fixed-start}     for layout.sh
#
# Environment: LLVM_MINGW (default /mnt/Dev/toolchains/llvm-mingw), TARGETS, VARIANTS to build a subset.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(git -C "$HERE" rev-parse --show-toplevel)
OUT=$(realpath -m "${1:?usage: build.sh OUT_DIR [REV]}")
REV=${2:-HEAD}
TC=${LLVM_MINGW:-/mnt/Dev/toolchains/llvm-mingw}
TARGETS=${TARGETS:-"x86_64-linux-gnu aarch64-linux-gnu x86_64-w64-mingw32"}
VARIANTS=${VARIANTS:-"find findindex lut fixed-start"}
export PATH="$TC/bin:$PATH"

SRC="$OUT/src"
echo "== exporting $REV to $SRC"
rm -rf "$SRC"; mkdir -p "$SRC"
git -C "$REPO" archive "$REV" | tar -x -C "$SRC"
# git archive leaves the submodules out: export their current checkouts
(cd "$REPO" && SRC="$SRC" git submodule foreach --quiet 'mkdir -p "$SRC/$sm_path" && git archive HEAD | tar -x -C "$SRC/$sm_path"')

mkdir -p "$OUT/build"
PATCHED="src/Benchmarks/Web/Base64.cpp src/Mimalloc/prim/os.c"
for f in $PATCHED; do cp "$SRC/$f" "$SRC/$f.pristine"; done

patch_for() {
	case $1 in
		find) ;;
		findindex) echo "$HERE/patches/findindex.patch" ;;
		lut) echo "$HERE/patches/lut.patch" ;;
		fixed-start) echo "$HERE/patches/mimalloc-fixed-start.patch" ;;
		*) echo "unknown variant $1" >&2; exit 1 ;;
	esac
}

build() { # variant target
	local v=$1 t=$2 b="$OUT/build/$1-$2" p
	for f in $PATCHED; do cp "$SRC/$f.pristine" "$SRC/$f"; done
	p=$(patch_for "$v")
	[ -n "$p" ] && patch -s -p1 -d "$SRC" < "$p"
	echo "== $v for $t"
	cmake -S "$SRC" -B "$b" -G Ninja -DCMAKE_BUILD_TYPE=Release -DQT_HOST_PATH=/usr \
		-DCMAKE_TOOLCHAIN_FILE="$SRC/cmake-modules/llvm-cross.cmake" -DCMAKE_SYSROOT="$TC/$t" -DTARGET_PREFIX="$t-" \
		-DELPIDA_ENABLE_CLI=OFF -DELPIDA_ENABLE_QT=OFF -DELPIDA_ENABLE_BENCHMARK_TOOLS=ON > "$b.configure.log" 2>&1 \
		|| { tail -20 "$b.configure.log"; exit 1; }
	cmake --build "$b" --target web-benchmarks > "$b.build.log" 2>&1 || { tail -20 "$b.build.log"; exit 1; }
}

exe() { # variant target -> path of the built benchmark
	local b="$OUT/build/$1-$2/apps/Benchmarks/web-benchmarks"
	case $2 in *mingw32) echo "$b.exe" ;; *) echo "$b" ;; esac
}

for v in $VARIANTS; do
	for t in $TARGETS; do
		[ "$v" = fixed-start ] && [ "$t" != x86_64-linux-gnu ] && continue
		build "$v" "$t"
	done
done
for f in $PATCHED; do cp "$SRC/$f.pristine" "$SRC/$f"; done

has() { [ -f "$(exe "$1" "$2")" ]; }

# Linux package (x86_64 + aarch64)
L="$OUT/pkg/elpida-b64-spread"
rm -rf "$OUT/pkg"; mkdir -p "$L/bin"
for t in x86_64-linux-gnu aarch64-linux-gnu; do
	a=${t%%-*}
	for v in find findindex lut; do
		if has $v $t; then mkdir -p "$L/bin/$a"; cp "$(exe $v $t)" "$L/bin/$a/web-$v"; fi
	done
done
if [ -d "$L/bin/x86_64" ] || [ -d "$L/bin/aarch64" ]; then
	cp "$HERE/run.sh" "$L/run.sh"
	cat > "$L/README.txt" <<'EOF'
Base64 decode spread test. Linux x86-64 or aarch64 (glibc 2.17+), python3; perf optional.
Three builds of the same benchmarks, differing only in how Base64 decode looks up a character:
  web-find       base64_chars.find()  (memchr byte loop)
  web-findindex  FindIndex()          (clang turns it into range checks)
  web-lut        256-byte lookup table
bin/x86_64 and bin/aarch64; run.sh picks the right one.
Run on an idle machine (laptop on AC power), preferably with the performance governor:
  bash run.sh            (15 rounds, ~15 min on a fast CPU, longer on slow ones)
Result: results-<host>-<date>.txt next to run.sh.
For mispredict counts: install perf (Ubuntu: linux-tools-$(uname -r), on a Pi linux-tools-raspi)
and allow it: sudo sysctl kernel.perf_event_paranoid=1
EOF
	tar -czf "$OUT/elpida-b64-spread.tar.gz" -C "$OUT/pkg" elpida-b64-spread
	echo "== $OUT/elpida-b64-spread.tar.gz"
fi

# Windows package
W="$OUT/pkg/elpida-b64-spread-windows"
if has find x86_64-w64-mingw32 || has lut x86_64-w64-mingw32; then
	mkdir -p "$W/bin"
	for v in find findindex lut; do has $v x86_64-w64-mingw32 && cp "$(exe $v x86_64-w64-mingw32)" "$W/bin/web-$v.exe"; done
	cat > "$W/README.txt" <<'EOF'
Base64 decode spread test for Windows x64 (same three variants as the Linux package):
  bin\web-find.exe       base64_chars.find()  (memchr byte loop)
  bin\web-findindex.exe  FindIndex()
  bin\web-lut.exe        256-byte lookup table
Run on an idle machine (High performance power plan if available):
  powershell -ExecutionPolicy Bypass -File run.ps1            (15 rounds, about 15 minutes)
Options: -Rounds N, -Cpu N (the processor for the pinned runs, default 2).
Result: results-<COMPUTERNAME>-<date>.txt next to run.ps1.
EOF
	# CRLF for Notepad and Windows PowerShell
	sed 's/$/\r/' "$HERE/run.ps1" > "$W/run.ps1"
	sed -i 's/$/\r/' "$W/README.txt"
	rm -f "$OUT/elpida-b64-spread-windows.zip"
	(cd "$OUT/pkg" && zip -qr "$OUT/elpida-b64-spread-windows.zip" elpida-b64-spread-windows)
	echo "== $OUT/elpida-b64-spread-windows.zip"
fi

# Layout test binaries (layout.sh)
if has find x86_64-linux-gnu && has fixed-start x86_64-linux-gnu; then
	mkdir -p "$OUT/layout"
	cp "$(exe find x86_64-linux-gnu)" "$OUT/layout/web-find"
	cp "$(exe fixed-start x86_64-linux-gnu)" "$OUT/layout/web-fixed-start"
	cp "$HERE/layout.sh" "$OUT/layout/"
	echo "== $OUT/layout (copy to the test machine, run layout.sh there)"
fi
