# Base64: run-to-run spread, the lookup table and production-style code

Base64 decode was the noisiest benchmark: 11-12% between runs of the same binary on an i9-10900K. The cause was
the decoder looking each character up with `base64_chars.find()`. Its exit is a data-dependent branch, and how well
it is predicted depended on the address layout of the process. The decoder was changed to a 256-byte lookup table,
which takes the spread to the level of the other benchmarks and keeps comparisons between CPUs and operating
systems sound (about 4.5-7 times the old score, depending on the CPU).

Both benchmarks then still spent most of their time in libc++'s per-character `push_back`, so encode and decode
were rewritten the way production Base64 code is written ([below](#production-style-encode-and-decode)). The
measurements on six machines are of the lookup-table build with `push_back`; the final code is to be measured
again.

Measured on 2026-11-15, against 2ee5c5e8. Raw results: [base64-spread-results/](base64-spread-results/). Test
code: [scripts/parity/base64-spread/](../../scripts/parity/base64-spread/).

## The three versions

| Version | Lookup | Code |
|---|---|---|
| `find()` | `base64_chars.find(c)`: musl's `memchr`, a byte loop that stops at the character's position in the alphabet | the code until 2ee5c5e8 |
| `FindIndex()` | a loop over the 64 characters; Clang sees the constant alphabet and turns it into range checks (`A-Z`, `a-z`, `0-9`, `+`, `/`) on x86-64 and aarch64 | `patches/findindex.patch` |
| lookup table | `values[c]`, a `constexpr` 256-byte table | `patches/lut.patch` |

The input is the same in every run (2048 bytes, fixed seed, about 2700 Base64 characters), decoded again and again
for the whole run.

## Cause: the address layout and the branch predictor

1. **Address layout.** On the 10900K, only turning off both ASLR (`setarch -R`) and mimalloc's random start of its
   aligned allocations (`_mi_os_get_aligned_hint`, 1 MiB steps, in every non-debug build;
   `patches/mimalloc-fixed-start.patch`) made the runs repeat: Base64 decode 11.6% to 0.7%, ASLR alone 9.1%,
   mimalloc alone 10.8% (`layout.sh`). A fixed layout is not a fix: each binary gets one lucky or unlucky layout
   (Base64 decode +4.8%, XML -2.9% against the normal median), a constant offset per build and platform instead of
   noise around the same mean.
2. **The branch predictor.** 24 runs under `perf stat` (`perf-stat.sh`, `correlate.py`): the instructions per
   result are the same in every run. The slow runs have more mispredicted branches (8.5 against 7.0 per 1000
   instructions, correlation with the score -0.98), and these account for the extra cycles. 89% of the mispredicts
   are the exit of `memchr`'s byte loop. The loop runs as many steps as the character's position in the alphabet,
   so the exit depends on the data. The predictor indexes by address, so how well it learns the pattern depends on
   where the code and data are. No other counter followed the score: L1 misses, store forwarding, 4K aliasing,
   machine clears, DSB/MITE.
3. **Governor.** Part of the spread measured earlier was the `powersave` governor. With `performance` the other web
   benchmarks were 0.4-2.8% on the 10900K, and Base64 decode stayed at 11-12%.

## Results

Same binaries on every machine (the Pi 4 runs an aarch64 build of the same source), performance governor unless
noted, 15 interleaved rounds unpinned and 15 pinned (`run.sh`, `run.ps1`).

| Machine | Spread find / FindIndex / LUT, unpinned | Spread, pinned | Mispredicts per 1000 instructions, find / FindIndex / LUT |
|---|---|---|---|
| Pentium E5200 (Core 2, 2008), Ubuntu 22.04 | 0.1% / 2.8% / 2.1% | 0.4% / 2.4% / 1.5% | 6.6 / 7.4-7.9 / 2.6 |
| Core i3 330M (Westmere, 2010), Ubuntu | 0.5% / 0.5% / 0.5% | 0.2% / 0.6% / 0.2% | 6.2 / 5.8 / 2.5 |
| Raspberry Pi 4 (Cortex-A72), Ubuntu 24.04 | 1.1% / 1.2% / 2.4% | 2.1% / 2.3% / 4.3% | 5.0 / 5.8 / 2.9 |
| Core i7-1065G7 (Ice Lake, laptop), Kubuntu 24.04 | 2.1% / 7.6% / 6.2% | 3.2% / 3.4% / 1.6% | 5.2-5.4 / 0.01-0.03 / 0.07-0.12 |
| Core i9-10900K (Comet Lake), Windows 10 | 6.9% / 17.3% / 2.6% | 10.7% / 2.5% / 1.5% | - |
| Core i9-10900K (Comet Lake), Ubuntu 26.04 | 11.0% / 19.4% / 3.0% | 12.1% / 2.7% / 4.0% | 7.0-8.5 / 2.0 / 1.5 |
| Ryzen 9 7950X3D (Zen 4), KDE neon; powersave, load ~1.3 | 4.9% / 2.1% / 3.0% | 2.7% / 2.2% / 4.2% | 0.33-0.39 / 0.03 / 0.03 |

Disturbed runs:

- The Pi 4 throttled: 70°C at the start, 82°C at the end, `throttled=0xe0008`. Its later runs, and its other web
  benchmarks (7-17%), are not clean.
- The laptop's unpinned FindIndex and LUT scores fell steadily over the rounds (package 47°C to 72°C).
- The 10900K's FindIndex 17.3% on Windows and 19.4% on Linux are single rounds (2.06e8 against 2.35-2.49e8 on
  Windows, 1.93e8 against 2.35-2.39e8 on Linux).
- The 10900K's pinned `find()` on Windows has two levels: 5.93e7, or 5.30-5.39e7.

Score relative to the E5200 (unpinned medians):

| Machine | find | FindIndex | LUT |
|---|---|---|---|
| E5200 | 1.00 | 1.00 | 1.00 |
| i3 330M | 0.84 | 0.94 | 0.87 |
| Pi 4 | 0.64 | 0.95 | 0.83 |
| i9-10900K Ubuntu | 2.55 | 3.95 | 3.38 |
| i9-10900K Windows | 2.83 | 4.12 | 3.31 |
| i7-1065G7 | 2.26 | 4.17 | 3.54 |
| 7950X3D | 4.99 | 7.23 | 5.56 |

## Findings

- **The `find()` noise needs a predictor that half-learns the pattern.** The old cores (Core 2, Westmere, A72) do
  not learn it at all: 5-6.6 mispredicts per 1000 instructions in every run, so they are steady to 0.1-1%. Zen 4
  learns it almost fully (0.33) and is also steady. Comet Lake is in between and swings with the layout. Ice Lake
  learns `find()` no better than Westmere, but learns the range checks of `FindIndex()` perfectly.
- **`find()` breaks comparisons.** On the same 10900K, Windows scored 1.109 times Linux with `find()`, against 0.978
  with the LUT (the full benchmark run is 1.005). The layout luck of each OS's binary becomes an OS difference.
  `find()` also reverses the order of two CPUs: the 10900K beats the i7-1065G7 by 13% with `find()` and loses by
  4.5% with the LUT.
- **`find()` rewards memorizing, not prediction.** Zen 4's lead over the 10900K is 1.96 with `find()` against 1.65
  with the LUT, because it memorizes a pattern that repeats every ~2700 characters. Real data does not repeat like
  that.
- **`FindIndex()` depends on the compiler.** Its speed comes from Clang removing the loop, which other compilers or
  versions may not do. On the E5200 the range checks mispredict more than `find()`.
- **The LUT still branches on the data**, in `is_base64()` (letter, digit or `+/`): 2.5-2.9 mispredicts per 1000
  instructions on the old cores, 1.5 on Comet Lake, 0.03-0.12 on Ice Lake and Zen 4. It still rewards a good
  predictor, but far less than `find()`, and it is the same code everywhere.

## Ice Lake against Comet Lake

The i7-1065G7 boosts to 3.9 GHz and the i9-10900K to 5.1-5.3 GHz, yet the laptop is faster with the LUT (pinned:
3.55e8 against 3.23e8). Both run the same x86-64 binary, so the instructions per character are the same, and the
score is proportional to IPC times clock:

| Version | 10900K IPC | i7-1065G7 IPC | IPC ratio | Score ratio i7 / 10900K | Implied i7 clock |
|---|---|---|---|---|---|
| find | 2.18 | 2.62 | 1.20 | 0.919 | 3.87 GHz |
| FindIndex | 3.12 | 4.52 | 1.45 | 1.105 | 3.87 GHz |
| LUT | 3.51 | 5.06 | 1.44 | 1.100 | 3.87 GHz |

The 10900K ran at 5.07 GHz: 25.4e9 cycles in each 5.0 s run. All three versions give the same implied laptop
clock, 3.87 GHz, which is its single-core boost, so the counters are consistent. Per clock, Ice Lake does 1.44
times the work on the LUT and 1.20 times on `find()`.

- **Width.** Comet Lake is a Skylake core (2015): 4 µops per cycle allocated and retired, 8 ports, 1 store per cycle.
  Ice Lake's Sunny Cove (2019) allocates 5 per cycle, has 10 ports, 2 stores per cycle, a 352-entry ROB (against
  224) and a 48 KB L1D (against 32 KB). The LUT loop runs at 3.51 instructions per cycle on the 10900K, near its
  4-wide limit. The decoder is loads, table
  lookups, shifts and byte stores into `char_array_4` and `ret`, plenty of independent work for a wider core. This
  explains most of the 1.44. It is the expected cause, not measured: port and store counters were not recorded.
- **Prediction.** On the 10900K the LUT mispredicts 1.52 times per 1000 instructions, about 135 million per run.
  At 16-20 cycles each that is 2.2-2.7e9 of the 25.4e9 cycles, 9-11%. Ice Lake (0.07-0.12) loses almost nothing
  there.
- **`find()`.** The `memchr` byte loop has one load, compare and branch per byte, and its time goes on mispredicted
  exits (5.3 per 1000 on Ice Lake, 8.3 on the 10900K). Width hardly helps such a loop, so Ice Lake's per-clock lead
  is only 1.20, less than the 10900K's 1.31 clock advantage. That is why `find()` reverses the order of the two CPUs.

## Production-style encode and decode

After the lookup table, the profile (7950X3D) still showed libc++'s `std::string::push_back` as a call per
character: 68% of the encode time, 10% of decode. libc++ compiles `basic_string<char>` into its library, so the
call is never inlined. `ret.append(char_array_3, 3)` per group helped decode by 17%, but `append` is a
116-instruction library function that copies the 3 bytes with `memcpy`/`memmove`, and 45% of the time stayed in
those three functions. Those benchmarks measured libc++'s string appending more than Base64.

`src/Benchmarks/Web/Base64.cpp` is now written the way production Base64 code is. No code was copied, but the
techniques are those of these implementations, credited in the source and in `Attribution.Base64.txt` (checked
against their sources on 2026-11-16):

- OpenSSL (`crypto/evp/encode.c`, Apache-2.0): decoding table with `0xFF` (`B64_ERROR`) for invalid characters, and
  one `(a | b | c | d) & 0x80` check per group of 4, the same check as ours.
- Go (`encoding/base64`, BSD-style): a 256-entry `decodeMap` with `0xFF` for invalid characters, an OR check per
  group (`assemble32`/`assemble64`), and encoding into an output sized in advance (`EncodedLen`, `(n + 2) / 3 * 4`).
- modp_b64 (Nick Galbreath's stringencoders, MIT; vendored in Chromium): per-group table lookups with one
  bad-character check per group, and output written through a pointer.

The behaviour stays that of René Nyffenegger's original: decoding stops at the first `=` or invalid character and
returns what came before it, where OpenSSL and Go report an error. In the benchmark the input is always valid, so
this only adds the cost of the check.

What changed:

- The output is sized once (`resize`, exact for encode, an upper bound for decode, cut at the end) and written
  through a pointer. No library call in the loops.
- Whole groups: 3 bytes to 4 characters, 4 characters to 3 bytes, then the remainder.
- Decode validates through its table: `=` and every character outside the alphabet map to `0xFF`. The four values of
  a group are OR-ed and checked once (bit 7). This replaces `is_base64()`, a chain of branches per character that
  the old cores mispredicted 2.5-2.9 times per 1000 instructions and Ice Lake and Zen 4 learned from the repeating
  input.

Same output as the original code: 110,248 checks (encode for lengths 0-300; decode of valid input, without padding,
with one byte replaced by any value, of random bytes and of random alphabet characters with `=`) under ASan and
UBSan. Not auto-vectorized: the loops are scalar loads, ORs, shifts and byte stores, and the only SSE is in the
inlined string setup, outside the loops.

7950X3D, 8 interleaved rounds pinned to core 10 (load about 1):

| Version | Decode | Encode |
|---|---|---|
| lookup table, `push_back` per character (measured on six machines) | 1.00 | 1.00 |
| `append(char_array_3, 3)` | 1.16 | - |
| output through a pointer | 2.11 | 4.86 |
| final: pointer, whole groups, validation through the table | 7.34 | 9.89 |

The final build spends 95% (decode) and 94% (encode) of the time in `Decode`/`Encode`. The rest is the `memset` of
`resize`'s zero-fill (2.5-4.4%) and the allocation (under 1%); C++17 has no way to size a string without filling
it (`resize_and_overwrite` is C++23).

## How it was profiled

On the 10900K under Linux, with the native build (it has symbols, unlike the PE under Wine),
`kernel.perf_event_paranoid=1` and the `performance` governor. Count first, then locate, then read the code.

**1. What differs between fast and slow runs** (`perf-stat.sh`, `correlate.py`). 24 runs pinned to one core, each
under `perf stat` with the counters behind the usual layout-dependent slowdowns: mispredicted branches, L1 misses,
store-forwarding blocks, 4K aliasing (`ld_blocks_partial.address_alias`), memory-ordering machine clears, µop cache
(DSB) misses and switches, store buffer stalls. `correlate.py` sorts the runs by score and prints, per counter, the
rate per 1000 instructions in the slowest and the fastest third and the correlation with the score:

```
metric (per 1k instr)        slow third   fast third  corr w/ score
instr/score                      1057.8       1057.9           0.09   same work in every run
cycles                           464.19       427.45          -0.99
br_misp_retired.all_branches     8.4936       7.0081          -0.98   the cause
ld_blocks_partial.address_alias  0.17054      0.17649          -0.07   not it
```

Only the mispredicts follow the score. The µop cache counters correlate too (-0.97, -0.93), as a consequence:
every mispredict refills the front end down the wrong path.

**2. Which branches** (`mispredicts.sh`). `perf record -e br_misp_retired.all_branches:pp -c 20011`: precise
sampling puts each sample on the branch that mispredicted. Over 8 runs, the slowest and the fastest look the same,
the slow one with more samples:

```
per function                     per instruction
  89.19%  memchr                   11675 memchr+0x13
  10.78%  Base64::Decode            8213 memchr+0x10
                                    2404 Base64::Decode
```

**3. What they are** (`objdump -d --no-show-raw-insn web-find --disassemble=memchr`):

```
8a610:  cmp  %sil,(%rax)      byte == c ?
8a613:  je   8a61f            +0x13: found, exit      most of the mispredicts
8a615:  inc  %rax
8a618:  dec  %rdx
8a61b:  jne  8a610            loop
```

`memchr+0x13` is the exit of musl's byte loop. The `+0x10` samples are the same exit, attributed to the compare
that sets its flags. `base64_chars.find(c)` calls `memchr` for every input character, so the loop runs as many
steps as `c`'s position in the alphabet. On the 7950X3D only CPU time was profiled (`perf record`, default event),
because precise sampling of branch misses works differently on AMD (IBS).

## Reproducing

```
scripts/parity/base64-spread/build.sh OUT_DIR [REV]   # exports REV (default HEAD) + submodules, builds the variants,
                                                      # packs OUT_DIR/elpida-b64-spread.tar.gz (Linux x86-64 +
                                                      # aarch64) and elpida-b64-spread-windows.zip
bash elpida-b64-spread/run.sh [ROUNDS]                # on the test machine; results-<host>-<date>.txt
powershell -ExecutionPolicy Bypass -File run.ps1      # Windows; -Rounds N, -Cpu N
bash OUT_DIR/layout/layout.sh [ROUNDS]                # address-layout test (find and mimalloc-fixed-start builds)
bash perf-stat.sh 24 ./web-find --index=3 --config=2048 > runs.csv && python3 correlate.py runs.csv
bash mispredicts.sh ./web-find --index=3 --config=2048   # per-function and per-instruction mispredicts
```

The patches are made against 2ee5c5e8. Once the LUT change is committed, an unpatched export of HEAD is the LUT
and the patches no longer apply: build the measured versions with `build.sh OUT_DIR 2ee5c5e8`. `perf-stat.sh`'s default events are
Skylake-family names; set `EVENTS` on other CPUs. Under Linux, `perf` needs `kernel.perf_event_paranoid` set to 1
or lower.
