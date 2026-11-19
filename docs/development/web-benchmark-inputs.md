# Real-world inputs for the Json, XML and Regex benchmarks

Until 2026-11-19 the three benchmarks processed 2048 bytes of generated input: the Json and XML documents were one short
element repeated (`JsonGenerationTask`, `XmlGenerationTask`), and the Regex text was generated lorem ipsum. After a few
elements the branch predictor knows the whole input, so the parsers ran far more predictably than on any real document.
They now process real documents, embedded at build time like the SVG and Compilation inputs (`EmbedFile.cmake`) and
copied into the task data by `EmbeddedDocumentTask`. Sources and licenses: `src/Benchmarks/Web/Assets/README.md`.

| Benchmark | Input | Size |
|---|---|---|
| Json Parsing | Wikidata entity Q2034599 (CC0): labels in 33 languages, 12 claims with references, `\u` escapes | 9,759 B |
| XML Parsing | U.S. House bill H.R. 2225 (public domain): document XML, DOCTYPE, processing instructions, mixed content | 8,020 B |
| Regex (Boost) | *Pride and Prejudice*, chapters 1-3 (public domain), UTF-8 punctuation | 11,726 B |

The benchmarks no longer have a size configuration. The Regex benchmark still writes its match target over the last
bytes of the text (`RegexTask`), so the whole text is searched.

## Keeping the footprint in cache

Full-run benchmarks must stay in the CPU caches (DRAM speed must not affect the score), on small CPUs too. The budget
that matters is the cache per core under the multi-thread run, where every thread parses its own copy: 256 KB on a
Raspberry Pi 4 (1 MB L2 for 4 cores), 128 KB on quad-core Cortex-A53 devices (512 KB L2).

The parsed result is several times the input: about 5.5x for the nlohmann Json tree and 6-8x for `XmlElement`. Both
parse tasks assigned the new result over the previous one, so two trees were alive during each parse. They now release
the previous result first (`ParseJsonTask`, `ParseXmlTask`): the same work per iteration (one release, one parse), half
the peak.

Peak live heap per parse (host build with a counting `operator new`, input included):

| Input | Before (two trees) | Now (release first) |
|---|---|---|
| Json, 9.8 KB | 123 KB | 66 KB |
| XML, 12 KB (first candidate, H.R. 3383) | 160 KB | 86 KB |
| XML, 8 KB (H.R. 2225) | 132 KB | 70 KB |

Simulated last-level cache misses per 1000 instructions of the parse task (callgrind, D1 32 KB 8-way, one last-level
cache of the given size, 16-way; the simulation is independent of the machine running it):

| Benchmark | Old input | Real input, two trees | Real input, release first (final) |
|---|---|---|---|
| Json, LL 128 KB | 0.024 | 0.456 | 0.037 |
| Json, LL 256 KB | 0.012 | 0.031 | 0.013 |
| XML, LL 128 KB | 0.010 | 0.668 (12 KB) | 0.133 |
| XML, LL 256 KB | 0.007 | 0.045 (12 KB) | 0.031 |
| Regex, LL 64 KB | 0.002 | 0.003 | 0.003 |

Json fits 128 KB per core like the old input. XML fits 256 KB but not quite 128 KB: the misses scale with the run
length (0.139 in a 1 s run, 0.127 in a 3 s run), so they happen on every parse, and shrinking the document from 12 to
8 KB barely changed them. Most are write misses in `memcpy`/`memmove` (string copies) and in the growth of the children
vectors. On a quad-core Cortex-A53 under the multi-thread run this is estimated at 1-2% of the XML result; see Open.

## Measurements on the i9-10900K

Ubuntu 26.04, performance governor, turbo on. `old` = the 2048-byte generated inputs, `new` = real inputs (12 KB XML),
`final` = real inputs (8 KB XML) and release-first. Interleaved rounds, single thread on CPU 2 (8 rounds) and 10 threads
on CPUs 0-9 (5 rounds), whole process under `perf stat`. Base64 Encode, which did not change, is the control.

Single thread, median and spread ((max - min) / median); events per 1000 instructions:

| Benchmark | Build | Result | Spread | Branch mispredicts | L1 misses | L2 misses | L3 misses |
|---|---|---|---|---|---|---|---|
| XML | old | 194.4 Mchars/s | 3.8% | 0.265 | 1.61 | 0.0080 | 0.00015 |
| XML | final | 100.9 Mchars/s | 3.0% | 1.453 | 0.88 | 0.0458 | 0.00013 |
| Json | old | 108.7 Mchars/s | 2.3% | 0.975 | 1.97 | 0.0202 | 0.00014 |
| Json | final | 144.4 Mchars/s | 2.1% | 1.321 | 1.70 | 0.0247 | 0.00015 |
| Regex | old | 103.3 Mchars/s | 2.0% | 2.377 | 0.014 | 0.0014 | 0.00014 |
| Regex | final | 168.3 Mchars/s | 2.7% | 2.900 | 0.030 | 0.0016 | 0.00017 |
| Base64 Encode (control) | old / final | 2199 / 2202 MB/s | 1.4% / 2.2% | 0.053 / 0.062 | 0.006 | 0.0007 | 0.00010 |

- **The inputs are less predictable, as intended.** XML mispredicts 5.5 times as often per instruction, Json 1.35
  times, Regex 1.2 times. The repeated XML element had made the XML parser almost branch-free in practice.
- **Nothing reaches DRAM.** L3 misses per 1000 instructions are the same for every build and the same as the control,
  single-thread and with 10 threads (0.00008 to 0.00010).
- **Release-first works.** Json's L2 misses go from 0.117 (real input, two trees) back to 0.025, the old input's level;
  XML's halve (0.099 to 0.046) and stay in the 10900K's L3. It costs nothing: Json `new` and `final` both 144 Mchars/s.
- **The results change**, so the base scores of these three benchmarks must be measured again
  ([reference-run.md](reference-run.md)): XML -48%, Json +33%, Regex +63% in characters per second. Results from
  before and after are not comparable.
- **Spread is unchanged** (2-4% single-thread for old and final alike).

Multi-thread scaling (10 threads / 1 thread): XML 8.70x old, 8.80x final; Json 8.64x, 8.42x; Regex 8.50x, 8.47x;
control 8.41x, 8.49x. The real inputs scale like the control, so there is no memory limit on this machine.

## Measurements on the i3-330M

Westmere (2010), 2 cores with Hyper-Threading, 256 KB L2 per core, 3 MB L3 shared; Ubuntu 18.04 (kernel 5.4),
performance governor. Same builds and method as on the 10900K: single thread on CPU 2 (8 rounds), 2 threads on separate
cores (CPUs 0 and 2) and 4 threads (both siblings of both cores, so two threads share each L2), 5 rounds each. The
counters are multiplexed on this CPU (each counts about 80% of the time): good for comparing builds, not exact. Events
per 1000 instructions; "LLC" is `longest_lat_cache.miss`, a last-level miss that goes to memory.

| Benchmark | Build | 1 thread | Spread | Mispredicts | L2 misses | LLC misses | LLC misses, 4 threads |
|---|---|---|---|---|---|---|---|
| XML | old | 53.95 Mchars/s | 4.9% | 2.50 | 0.143 | 0.0038 | 0.0026 |
| XML | final | 27.93 Mchars/s | 4.6% | 3.04 | 0.317 | 0.0065 | 0.0164 |
| Json | old | 25.02 Mchars/s | 3.0% | 5.01 | 0.186 | 0.0054 | 0.0073 |
| Json | final | 34.39 Mchars/s | 3.7% | 5.71 | 0.227 | 0.0059 | 0.0120 |
| Regex | old | 23.77 Mchars/s | 6.7% | 7.40 | 0.044 | 0.0058 | 0.0051 |
| Regex | final | 43.52 Mchars/s | 5.3% | 7.00 | 0.048 | 0.0051 | 0.0044 |
| Base64 Encode (control) | final | 666 MB/s | 6.3% | 0.11 | 0.015 | 0.0020 | 0.0023 |

- **Memory traffic stays negligible.** Single-thread, the final inputs miss the last-level cache as rarely as the old
  ones (0.005-0.0065 per 1000 instructions, near the control). With 4 threads sharing two L2s, XML reaches 0.0164: at
  about 200 cycles each, roughly 3 cycles per 1000 instructions, well under 1% of the run time.
- **Release-first helps more here.** From `new` to `final`, 4-thread L2 misses drop from 1.26 to 0.89 (XML) and from
  1.16 to 0.59 (Json).
- **Scaling** (2 cores / 1 thread; 4 threads / 1 thread): XML 2.05x/2.16x old, 1.91x/2.07x final; Json 1.95x/2.36x,
  1.94x/2.33x; Regex 1.96x/2.34x, 2.03x/2.32x; control 1.93x/1.71x. XML's 2-core scaling is 7% lower with the final
  input, inside the 4-6% single-thread spread these short runs have on this machine; the 10900K shows no such loss.
- **The old inputs favoured the newer CPU's branch predictor.** Mispredicts rise only 1.1-1.2x here (XML 2.50 to 3.04)
  against 5.5x for XML on the 10900K: the 10900K had learned the repeated element, the 330M much less. How much faster
  the 10900K is (single thread) changes accordingly: XML 3.60x old, 3.61x final; Json 4.34x, 4.20x; Regex 4.35x, 3.87x
  (control 3.31x). The real inputs narrow the gap where the repetition had helped the new core.

## Measurements on the Raspberry Pi 4

Cortex-A72 at 1.8 GHz, 1 MB L2 shared by the 4 cores and no L3, so an L2 miss goes to memory; Ubuntu 24.04
(kernel 6.8), performance governor. The Pi also serves DNS and DHCP (Pi-hole, about 0.6% of one core). Same builds
(aarch64) and method: single thread on CPU 2 (8 rounds), 3 threads on CPUs 1-3 (leaving CPU 0 to Pi-hole) and 4 threads
(5 rounds each). Before each run the runner waits until the SoC is at 68 C or below, and it records the temperature, the
throttle flags and the lowest ARM clock during the run: the maximum was 78.3 C, the clock never left 1.8 GHz and no
throttling happened during the runs. "DRAM" is `l2d_cache_refill` per 1000 instructions.

| Benchmark | Build | 1 thread | DRAM, 1 thread | DRAM, 4 threads | Scaling, 4 threads |
|---|---|---|---|---|---|
| XML | old | 46.51 Mchars/s | 0.0064 | 0.0074 | 3.94x |
| XML | new (two trees, 12 KB) | 26.64 Mchars/s | 0.0163 | 0.5263 | 3.58x |
| XML | final | 25.04 Mchars/s | 0.0106 | 0.1377 | 3.86x |
| Json | old | 20.84 Mchars/s | 0.0088 | 0.0201 | 3.97x |
| Json | new (two trees) | 29.07 Mchars/s | 0.0174 | 0.2464 | 3.74x |
| Json | final | 29.31 Mchars/s | 0.0105 | 0.0431 | 3.91x |
| Regex | old | 25.65 Mchars/s | 0.0047 | 0.0029 | 3.97x |
| Regex | final | 39.24 Mchars/s | 0.0064 | 0.0038 | 3.97x |
| Base64 Encode (control) | final | 641 MB/s | 0.0029 | 0.0016 | 3.97x |

- **This is the machine where the footprint matters.** With all four cores parsing, the real inputs with two live trees
  reached memory 70 (XML) and 12 (Json) times as often as the old inputs, and scaling fell from 3.94x to 3.58x (XML) and
  3.97x to 3.74x (Json): DRAM speed was affecting the multi-thread score.
- **Release-first fixes most of it.** The final builds scale 3.86x (XML) and 3.91x (Json) against 3.97x for the
  control: XML loses about 2% and Json about 1.5% to memory in the 4-thread run. Single-thread and 3-thread are close to
  the control (3 threads: XML 2.98x, Json 2.98x, control 2.99x).
- **Regex is layout-sensitive on the A72.** The `new` and `final` builds run identical regex code on identical text, yet
  `final` is 9% slower with 70% more mispredicts (8.5 against 5.0 per 1000 instructions), in every round: the code
  around it moved (different XML and Json code), and the A72's branch predictor aliases differently. The same pair
  differs by 1% on the 10900K and 4% on the 330M. This is the code-placement effect of
  [run-to-run-spread.md](run-to-run-spread.md), not an effect of the input, but it means an unrelated change can move
  the Regex result on this core by several percent.

## XML parser: text nodes without `std::ostringstream` (2026-11-20)

`XmlParser` built every text and CDATA node through a `std::ostringstream` and copied the result out with `str()`. It
now appends to a `std::string` and moves it into the element. The parse tree is identical (checked node by node,
attributes and content, on the XML input, the SVG benchmark document, the old generated input and the 12 KB bill), so
the SVG benchmarks, which use the same parser, are unaffected in their output. Builds: `old` (2048-byte generated
input), `before` (real input, release-first, previous parser), `after` (with the parser change). XML and the Base64
control, interleaved rounds as above.

| Machine | old | before | after | after / before |
|---|---|---|---|---|
| i9-10900K, 1 thread | 194.0 Mchars/s | 101.7 | 180.0 | 1.77x |
| i3-330M, 1 thread | 55.1 Mchars/s | 28.4 | 50.9 | 1.79x |
| Raspberry Pi 4, 1 thread | 46.6 Mchars/s | 25.1 | 45.5 | 1.81x |

Multi-thread scaling (threads / 1 thread), XML old / before / after, and the control:

| Machine | XML old | XML before | XML after | Control |
|---|---|---|---|---|
| i9-10900K, 10 cores | 8.73x | 8.77x | 8.84x | 8.64x |
| i3-330M, 2 cores | 2.02x | 2.00x | 2.00x | 2.00x |
| i3-330M, 4 threads | 2.15x | 2.18x | 2.31x | 1.77x |
| Raspberry Pi 4, 3 cores | 2.97x | 2.98x | 3.00x | 2.99x |
| Raspberry Pi 4, 4 cores | 3.91x | 3.90x | 3.85x | 3.97x |

- **The parser is about 1.8 times as fast on every machine**, so the XML result changes again (base scores are measured
  after this change).
- **Memory traffic per parse falls to about a third** on the Pi with 4 cores: 0.069 memory refills per 1000
  instructions after, against 0.111 before, with roughly half the instructions per character.
- **On the Pi's 4-core run, XML stays 1.5-3% below the control**, as the old input does (3.91x); the 4-core runs
  spread 3-4%, so the remaining difference is at the edge of what these runs can resolve. The Pi stayed at 1.8 GHz
  (maximum 77.4 C).
- In the callgrind simulation, misses at a 128 KB cache halve (0.133 to 0.072 per 1000 instructions) and at 256 KB fall
  to 0.016 (old input 0.007).

## Open

- **Small shared caches.** After the parser change, XML on the Pi 4 with 4 cores is 1.5-3% below the control, within
  the run spread, like the old input; Json about 1.5%. Quad-core A53 devices (128 KB of L2 per core) not measured.
- **Regex code placement on the A72** (9% between two builds with identical regex code). Not investigated.
