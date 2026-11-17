# Run-to-run spread of every benchmark (i9-10900K)

After Base64 decode ([base64-decode-spread.md](base64-decode-spread.md)), every benchmark was checked for the same
kind of problem: a score that moves between runs of the same binary because a CPU mechanism depends on the address
layout of the process (ASLR, mimalloc's random start), and especially such behaviour caused by our own code doing
something real implementations do not.

Measured on 2026-11-16 on the i9-10900K (Ubuntu 26.04, `performance` governor, `kernel.perf_event_paranoid=1`),
with the 2ee5c5e8 binaries of the parity package plus the final Base64 build. Raw data and summary:
[spread-sweep-results/](spread-sweep-results/core-i9-10900k-ubuntu-26.04/). Scripts:
[scripts/parity/spread-sweep/](../../scripts/parity/spread-sweep/). Not decided yet: see [Open](#open).

## Method

`sweep.sh`: 15 runs of each benchmark pinned to CPU 2, each under `perf stat` (`perf-stat.sh`, Skylake-family
events: cycles, instructions, mispredicts, L1 misses, store-forwarding blocks, 4K aliasing, machine clears, µop
cache (DSB/MITE) delivery and switches, store buffer stalls). `summary.py` gives, per benchmark, the spread
((max - min) / median), the IPC, the spread of the instructions per result, the mispredicts per 1000 instructions,
and the three counters whose rate per 1000 instructions follows the score most closely.

- **Disturbed runs are left out.** The login screen's `gnome-shell` (`gdm-greeter`, about 9% CPU at times) woke up
  during some runs: those lost about 1.6% of their cycles (perf stat only counts our process) and some cache
  contents, and scored 2-3% lower. Runs with a cycle count more than 0.5% below the median are dropped: 2-4 of 15 per
  benchmark. With them, every benchmark spread at least 2%, AES included (0.1% earlier).
  This filter assumes a steady clock (`performance` governor, as here); with a governor that changes the clock,
  undisturbed runs vary in cycles too and get dropped.
- **The instructions per result are constant** (0.03-0.22% spread) in every benchmark except compilation (13.9%):
  2 of its 15 runs did 14% more instructions outside the timed part, at the same score. In the timed part the work
  is the same in every run, so the spread is how fast the same instructions run (IPC).
- A first summary had a bug (scores and counters paired from different runs). `summary.py` is the fixed one, and
  it reproduces `summary.txt` from the CSVs.

## Results

| Benchmark | Spread | IPC | Mispredicts / 1k instr | Counter that follows the score (correlation; slow -> fast per 1k instr) |
|---|---|---|---|---|
| Base64 decode (2ee5c5e8) | 9.1% | 2.25 | 7.7 | mispredicts -1.00 (8.43 -> 6.95), DSB µops -1.00 |
| **Base64 decode (final)** | **0.6%** | 4.05 | 0.08 | none clearly |
| Regex | 4.9% | 2.51 | 2.3 | DSB µops -0.99, DSB-to-MITE switches -0.98, mispredicts -0.96 (2.52 -> 2.33) |
| XML parsing | 3.8% | 2.79 | 0.58 | DSB-to-MITE switches -0.97, DSB µops -0.93, mispredicts -0.91 (0.72 -> 0.54) |
| C++ compilation | 3.2% | 1.90 | 2.9 | DSB misses -0.86, DSB-to-MITE -0.72, mispredicts -0.63 (3.0 -> 2.8) |
| Matrix inverse 4x4 | 3.0% | 1.16 | 0.005 | store-forwarding blocks -1.00 (43.8 -> 43.1) |
| JSON parsing | 2.2% | 2.87 | 0.87 | memory-ordering machine clears -0.83 (0.087 -> 0.073), store buffer stalls -0.83 (11 -> 8.5) |
| RSA encryption | 2.0% | 2.00 | 0.04 | 4K aliasing -0.74 (1.75 -> 0.40) |
| Base64 encode (2ee5c5e8) | 1.7% | 3.58 | 0.03 | 4K aliasing -0.93 (2.25 -> 0.76) |
| **Base64 encode (final)** | **0.3%** | 4.08 | 0.05 | none clearly |
| Matrix mult 32x32 | 1.6% | 1.77 | 0.10 | L1 misses -0.52 |
| SVG rasterization | 1.2% | 3.59 | 0.25 | 4K aliasing -0.95 (7.5 -> 1.5) |
| FFT | 1.0% | 2.81 | 0.85 | mispredicts -0.89 (0.90 -> 0.82) |
| Zlib compression | 0.8% | 1.30 | 13.5 | - |
| Zlib decompression | 0.8% | 2.01 | 8.2 | - |
| RSA decryption | 0.7% | 2.02 | 0.10 | - |
| AES decryption | 0.5% | 3.80 | 0.002 | - |
| Ray tracing | 0.5% | 2.16 | 2.1 | - |
| AES encryption | 0.3% | 3.85 | 0.002 | - |
| NBody | 0.3% | 2.72 | 0.002 | - |

A correlation means little when the spread is under about 1%: the counters then follow noise.

## Mechanisms

Three ways the address layout changes the speed of the same instructions:

1. **Branch prediction.** The predictor is indexed by addresses, so how well it learns a data-dependent pattern
   depends on where the code and data are. The DSB (µop cache) counters move with it: every mispredict refills the
   front end down the wrong path. Base64 decode (fixed), regex, XML, compilation.
2. **4K aliasing.** A load whose address has the same low 12 bits as an earlier store's is first treated as
   depending on it. Whether two buffers alias depends on where they are allocated. RSA encryption, SVG, Base64 encode
   (fixed).
3. **Store-forwarding stalls.** A load that cannot take its data from the store buffer waits for the stores. How long
   depends on timing and alignment. Matrix inverse.

JSON's memory-ordering machine clears and store buffer stalls do not fit these clearly; not investigated further.

## Per benchmark

- **Base64 decode and encode:** fixed by the production-style rewrite. On this machine the final builds spread 0.6%
  and 0.3%, and no counter clearly follows the score.
- **Matrix inverse 4x4: our code, measuring something real code avoids.** `Matrix<T,4,4>::CalculateInverse`
  (`include/Elpida/Math/Matrix.hpp`) expands cofactors: it builds each 3x3 submatrix in a temporary on the stack with
  single-float stores (`movss`) and reads it back with 8- and 16-byte loads (`movlps`, `movaps`). A wide load that
  covers several narrow stores cannot be forwarded and waits for them: 43 times per 1000 instructions, about every 23
  instructions, at an IPC of 1.16. The stall length follows the stack alignment, which ASLR changes per run.
  Libraries (glm, Eigen) compute a 4x4 inverse with a closed formula (2x2 sub-determinants) in registers.
- **XML parsing: our parser, but realistic.** 73% of the mispredicts are in `ParseElement`, at the exits of the
  loops that scan until `<` (text) and until `/`, `>` or a space (names, attributes): the exit is the length of each
  token. Real parsers (pugixml, RapidXML) scan the same way.
- **Regex:** all mispredicts are in Boost's `perl_matcher`, the engine's own work.
- **C++ compilation:** LLVM, branchy and with a large code footprint (DSB misses). 7.4% of the time (7950X3D
  profile) was in musl-lite's `strlen`, then a byte loop, where real C libraries read a word or vector at a time
  (now upstream's word-at-a-time version, see Decisions).
- **JSON (nlohmann), RSA (OpenSSL):** library code doing its work, small spreads.
- Under 1%: zlib, RSA decryption, AES, ray tracing, NBody.

Two observations from the profile on the 7950X3D (all 18 benchmarks, `perf record`):

- `std::string::push_back` is never inlined (libc++ compiles `basic_string<char>` into its library): 68% of Base64
  encode, 8% of JSON. Fixed for Base64 by the rewrite.
- musl-lite's `strlen` and `memchr` were byte loops (since its first commit; upstream musl reads a word at a time).
  They were the same code everywhere, but slower than any real C library. Restored, see Decisions.

## Decisions (2026-11-16)

1. **Matrix inverse 4x4:** kept as it is.
2. **musl-lite string functions:** upstream musl's word-at-a-time (`__GNUC__`) code, removed when musl-lite was
   imported, is restored in `memchr`, `strlen`, `memccpy`, `memcpy`, `memmove`, `stpcpy`, `stpncpy` and
   `strchrnul` (pure C, the same code everywhere). `memset` keeps upstream's plain C loop: clang vectorizes it (SSE2
   stores), while upstream's `__GNUC__` version uses 8-byte scalar stores and cost Base64 encode 3.4%, Base64 decode
   2.2% and matrix multiplication 1.0% (10900K, 11 rounds; with the old `memset` all three were at 1.001).
   Differential test old vs new: 4.59M checks on x86_64, i386, aarch64 and armv7 (qemu), no difference.
   `disasm-parity.py`: only these functions change, Linux vs Windows verdicts unchanged.
   Full A/B, 10900K, 9 rounds, new/old with the `__GNUC__` `memset` (Linux / Windows under Wine): XML 1.028 / 1.015,
   zlib compression 1.024 / 1.016, regex 1.022 / 0.999, compilation 1.008 / 1.017, others within ±1% except the
   `memset` losses above. Windows/Linux ratios unchanged (all within 2%). The zlib numbers move ±2% with code placement
   alone (see [parity-checks.md](parity-checks.md)).
3. **XML, regex, compilation, JSON, RSA, SVG:** left as they are.
4. **JSON's memory-ordering clears:** not investigated further for now.

## Reproducing

```
bash scripts/parity/spread-sweep/sweep.sh BENCHMARKS_DIR OUT_DIR [N] ["Name|binary|args"...]
python3 scripts/parity/spread-sweep/summary.py OUT_DIR
bash scripts/parity/base64-spread/mispredicts.sh ./web-benchmarks --index=0 --config=2048   # where they are
```

On a CPU that is not Skylake-family, set `EVENTS` (e.g. `cycles,instructions,branch-misses`); `summary.py` uses
whatever events the CSVs have.
