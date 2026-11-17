# The C++ Compilation benchmark

The benchmark compiles one C++ file with the LLVM/Clang 19.1.7 libraries linked into the executable
(`src/Benchmarks/Compilation`): `GenerateCppCodeTask` copies the embedded source, `CompileWithClangTask` (timed) runs
clang's `EmitObjAction` on it for `x86_64-none-none` at `-O3` and discards the object. The libraries are built by
`scripts/docker/build-llvm.sh` (see [toolchain.md](toolchain.md#llvm-libraries-for-the-compilation-benchmark) and
[third-party-libraries.md](third-party-libraries.md#llvmclang-1917-compilation-benchmark)).

This document records the review of 2026-11-16/17: two configuration problems that made the benchmark measure
something other than a compile, and the new source.

## It was not compiling like clang does

`CompilerInvocation::CreateFromArgs()` takes **cc1** arguments, not driver arguments. The task passed only
`{"-O3", "test.cpp"}` and then called `resetNonModularOptions()`, which resets every codegen option that is not marked
"affecting" (`CODEGENOPT` in `clang/Basic/CodeGenOptions.def`) to its default. Only `-O3` survived. The compile ran
with:

| Option | Benchmark (before) | `clang++ -O3 -c` |
|---|---|---|
| `VerifyEach` (IR verifier after every pass) | on (`.def` default is 1) | off |
| `VerifyModule` | on | off (`-disable-llvm-verifier`) |
| `DiscardValueNames` | off: every IR value kept a string name | on (`-discard-value-names`) |
| `VectorizeLoop`, `VectorizeSLP` | off | on (`-vectorize-loops -vectorize-slp`) |
| `UnrollLoops` | off | on (on by default at `-O2`+) |

Profile on the 7950X3D (old source): 18% IR verifier, about 30% hashing and string work for value names
(`hash_short`, `memcpy`, `strlen`, `basic_string::insert`), 4% optimizer passes, 1% code generation.

`resetNonModularOptions()` also sets `PreprocessorOptions::RetainRemappedFileBuffers = true`. The task remaps the
source buffer once and reuses the `CompilerInstance` for every run; without the flag the source manager frees the
buffer after the first compile and the second one crashes (`MemoryBuffer::getMemBufferRef` in
`Preprocessor::EnterMainSourceFile`). That is presumably why the call was there.

**Fix (`CompileWithClangTask.cpp`):** the task passes the cc1 arguments the driver produces for
`clang++ -O3 -c test.cpp -target x86_64-none-none` (from `clang++ -###`), minus paths, debug info and
`-disable-free` (it would leak every run), sets `RetainRemappedFileBuffers` explicitly, and no longer calls
`resetNonModularOptions()`. Old source on the 7950X3D: 1.55 → 6.26 files/s.

## `-fno-builtin` in the LLVM libraries

After the fix, `memcpy` was still 17% of the cycles and 35% of the instructions (callgrind: 35 million calls per
compile, about 32 instructions each); in a real clang it is not visible. `-fno-builtin` was set for all Elpida code
(root `CMakeLists.txt`) and for the LLVM libraries (`build-llvm.sh`). Without builtins every fixed-size copy is a call,
for example the 8-byte `memcpy` LLVM uses as a bit cast in `QualType::hasLocalQualifiers()`:

```
mov    $0x8, %edx
mov    %rbx, %rsi
call   memcpy
```

The flag in `build-llvm.sh` had the comment "on arm64 windows the compilation benchmark crashes". `long` was not the
cause (musl-lite's headers are LLP64-correct on `WIN32`, which clang predefines for mingw targets). The first arm64
Windows run without the flag (2026-11-18) crashed with `0xC0000005` in LLVM's X86 `FixupBWInstPass::
getSuperRegDestIfDead`: with builtins, the compiler turned its loop over a 0-terminated list of 16-bit register
numbers into `wcslen`, which is right for Windows' 16-bit `wchar_t`, but musl-lite's aarch64 `alltypes.h.in` declared
`wchar_t` 32-bit on Windows too (x86_64 and i386 had the `WIN32` case), so its `wcslen` (which replaces msvcrt's)
scanned 4-byte units past the end of the list. `-fno-builtin` only hid it by stopping the `wcslen` call. Fixed:
`wchar_t` is `unsigned short` on aarch64 Windows as on x86_64; every wide-character function of musl-lite was wrong
there before.

**Change (2026-11-16):** `-fno-builtin` removed from the root `CMakeLists.txt` and from `build-llvm.sh`. Only the code
that implements or measures libc functions keeps it: musl-lite (`-ffreestanding`) and the StdLib benchmarks. Old
source, LLVM libraries rebuilt both ways from the same tree, 7950X3D: 6.15 → 10.23 files/s, `memcpy` 17% → 0.8%.
Parity checks and all benchmarks: [parity-checks.md](parity-checks.md#builtins-on-2026-11-16).

## The new source

The old source (`TestSource.cpp`, 1760 lines, no `#include`) was musl's math functions plus a small ray tracer: C-like
code with a few templates, about 0.1 s per compile on the 7950X3D.

Typical C++ spends most of its compile time in the frontend, because of large headers. A small file using libstdc++
(`vector`, `string`, `unordered_map`, `map`, `algorithm`, `memory`, `functional`, `optional`, `variant`, about 60
lines of own code, 69K lines preprocessed) on the 7950X3D at `-O3`: 0.74 s, 56% frontend. With the toolchain's libc++:
`#include <vector>` alone is 56K lines and 0.40 s of frontend; the same small file 0.84 s.

Embedding preprocessed libc++ was rejected: the headers alone cost 0.3-0.4 s on the 7950X3D, about 2 s on an E5200 and
4-6 s on a Pentium 4. The budget is **about 2.5 s per compile on a Pentium 4 Northwood**. From the old full-run results
the E5200 compiles 4.7x slower than the 7950X3D and the 10900K 3.3x slower; Northwood is estimated at another 1.8x
below the E5200, so the budget is about 0.25-0.3 s on the 7950X3D.

`src/Benchmarks/Compilation/Assets/CompilationBenchmarkSource.cpp` (3843 lines, 122 KB, no `#include`, GPL like the
rest of Elpida) has two parts:

- **A standard-library-like core** (`namespace ebs`): type traits (with the clang builtins libc++ uses), `pair`,
  `tuple`, `initializer_list`, hashing, function objects, `allocator`, `unique_ptr`, `shared_ptr`/`weak_ptr`,
  iterators, `span`, `array`, `vector`, `small_vector`, `string` with small-string optimization, `string_view`,
  `optional`, `variant` with `visit`, `function`, `function_ref`, algorithms (introsort, heap sort, merge sort,
  `nth_element`, binary search, ...), an open-addressing hash map, an AVL tree map, a ring buffer, a priority queue,
  `to_chars`/`from_chars`, a `{}`-style `format`, `expected`, a signal and an arena.
- **Application code** that instantiates it: template geometry (`vec<T, N>`, `mat<T, R, C>`, Gauss-Jordan inverse,
  transforms, `aabb`, ray/sphere and ray/box intersection) and text processing (splitting, word counting in a
  `hash_map<string, size_t>`, `stable_sort`, formatting). Entry points are `extern "C"` so the code is generated.

It is embedded at build time with `cmake-modules/EmbedFile.cmake` (like the SVG document); the array has a
terminating NUL that clang's memory buffer needs.

Cost of the parts (toolchain clang 21, `-O3`, 7950X3D, `-ftime-trace`), each together with the full core:

| Part | Total | Frontend | Backend |
|---|---|---|---|
| Core alone | 0.035 s | 0.034 s | 0.001 s |
| + geometry | 0.080 s | 0.046 s | 0.034 s |
| + text | 0.194 s | 0.042 s | 0.152 s |
| + JSON (parser and writer) | 0.298-0.386 s | 0.07-0.08 s | 0.23-0.30 s |
| + expression language | 0.747 s | 0.131 s | 0.616 s |
| **geometry + text (chosen)** | **0.257 s** | 0.062 s (24%) | 0.195 s |
| everything (6 modules) | 2.4 s | 0.39 s (16%) | 2.03 s |

Backend time follows the amount of generated code (about 1.5-3 ms per function, evenly spread), and `-O2` instead
of `-O3` saved under 10%. Within the budget the frontend share stays near 25%: parsing the core costs little; the
frontend time is mostly template instantiation driven by the application code. The full version with JSON (typed
struct mapping included), an expression language (lexer, Pratt parser, AST with visitors, evaluator, constant
folding), an entity-component system, a BVH and graph algorithms was written too (5900 lines); it is not in the repo.

## Results (2026-11-17)

| Machine | Old source, before the fixes | New source |
|---|---|---|
| 7950X3D, Linux | 1.55 files/s | 4.79 files/s (0.21 s) |
| i9-10900K, Linux (powersave) | | 3.62 files/s (0.276 s), spread 0.5% over 7 runs |
| i9-10900K, Wine | | 3.48 files/s, spread 1.7%; Windows/Linux 0.961 |

Profile of the new source in the benchmark (7950X3D): about 30% IR optimizer, 14% instruction selection, register
allocation and scheduling, 13% clang frontend, 4% allocator, 2% `mem*`/`str*`, the rest LLVM/clang code not
attributed to one phase.

Windows/Linux is 0.96 with both the old and the new source (0.972/0.961 with the old source before/after removing
`-fno-builtin`), not analyzed further.

**TODO:** a run on the E5200 (base score) and, if possible, a Pentium 4 to confirm the 2.5 s budget.
