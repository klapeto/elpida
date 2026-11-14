# Single-thread benchmarks and processor affinity

## What the full benchmark does

Since 2026-11-12 the full benchmark (CLI, Qt, Android) runs the single-thread benchmarks **without an affinity**: the
benchmark process may run on every processor and the OS picks one (`FullBenchmarkInstance::RunSingleThread` passes
`{}`). The multi-thread benchmarks never had one.

Before, single-thread benchmarks were started with `--affinity=<fastest processor>`, so the benchmark executable
restricted its process to that one processor (`PinProcessToProcessors`, `src/Elpida/EntryPoint/Main.cpp`). The fastest
processor came from `elpida-info-dumper`, which pins itself to each core in turn and times a short loop.

This is not thread pinning (`--pin-threads`, which pins every thread to its own processor): that is off by default and
the full benchmark never used it.

### Decision (final, 2026-11-14)

Single-thread benchmarks run **without an affinity on every platform** (Linux, Windows, Android). This was decided
after measuring pinned vs OS-scheduled runs on six machines (see [Results](#results-2026-11-12-to-2026-11-14)).

Why:

- **A wrong pin costs far more than OS scheduling.** OS scheduling was at most ~3% behind the best pinned core on any
  machine (i9-10900K on Linux) and within ±0.3% on the 7950X3D. Pinning to the wrong core cost ~10% (7950X3D, V-Cache
  CCD).
- **The fastest-processor search is unreliable.** It is a short measurement on an idle machine, where clocks change all
  the time; it picks a different processor from run to run, and on the 7950X3D it picked processors 12 and 14
  although the OS ranks 10 and 13 highest.
- **Comparability between machines.** Pinning only where the platform allows it (OS ranking available, cores in the
  app's cpuset, …) would measure machines differently and bias scores by up to a few percent depending on the OS. Same
  reasoning as identical benchmark code on every platform ([code-parity.md](code-parity.md)).
- **It measures what programs get.** Real programs are not pinned; the score includes how well the OS places a busy
  thread, which is a property of the platform.
- **Android can't pin reliably.** Apps may only use the processors of their cpuset and the kernel pauses idle big
  cores. The info dumper's search failed there ("failed to pin the thread"), and with the fallback (processor 0, a
  little core on phones) single-thread scores dropped from 3.2 to 0.5. The in-process Android code before ignored the
  affinity. Without affinity the phone scored 3.2 again: EAS moves a busy thread to the big/prime cluster within
  milliseconds, and each benchmark runs for seconds.
- **Heat.** Pinned, the benchmark can't move off a core that gets hot (laptops, phones).
- **One code path**, no per-OS ranking logic or fallbacks.

Costs: occasional migrations add some run-to-run noise (repeat runs to average it out), and on a hybrid CPU with an
OS that doesn't know its topology the thread may land on an efficiency core (which is what any program gets on that
system).

`elpida-info-dumper` still searches for the fastest processor (shown in the topology). Explicit `--affinity` and
`--pin-threads` remain available for custom benchmark runs and experiments.

### Alternatives considered and rejected

- **Pin to the info dumper's fastest processor** (the old behaviour): unreliable pick, see above.
- **Pin only to a core the OS ranks fastest**, with no affinity as the fallback. Sources: Linux
  `acpi_cppc/highest_perf` / `amd_pstate_prefcore_ranking` (x86 with CPPC), `cpu_capacity` (ARM), per-core
  `cpuinfo_max_freq`; Windows `GetSystemCpuSetInformation` `EfficiencyClass`/`SchedulingClass` (Windows 10+, nothing
  on 7/8/XP); hwloc CPU kinds (already used in `TopologyLoader.cpp`) only tell hybrid cores apart, not the 7950X3D's
  CCDs or the 10900K's favored cores. Gain at most a few percent, available only on some machines → breaks
  comparability.
- **Android: wake the big cores with a busy loop, then pin to the highest-capacity core.** Depends on undocumented
  vendor pause/placement logic (Qualcomm, MediaTek, Exynos, Tensor all differ), the core can be paused again while
  pinned, the prime core may not be in the app's cpuset, and a pinned prime core throttles instead of migrating. The
  benchmark's own load already wakes the big cores.
- **Android: record per-core residency / thermal status, pin to the top cluster, performance hints (ADPF), sustained
  performance mode.** Not pursued; the hint APIs would also change clocks and placement in a way desktop runs don't
  get.

## Measuring the difference: `elpida-affinity-test`

`apps/AffinityTest` builds `elpida-affinity-test`, installed next to `elpida-cli` (with `ELPIDA_ENABLE_CLI`, `ELPIDA_ENABLE_QT`
or `ELPIDA_ENABLE_BENCHMARK_TOOLS`). It loads the same full benchmark instances as the CLI (same configurations, same
benchmark executables) and runs every single-thread benchmark in two modes:

- **pinned**: `--affinity=<processor>`, the old behaviour (default: the info dumper's fastest processor);
- **OS-scheduled**: no affinity, the current behaviour.

Each run takes about 6 seconds; the modes alternate (and which goes first alternates) so that warm-up and heat affect
both the same way. The whole set (18 benchmarks, 3 runs per mode) takes about 11 minutes.

```sh
# Linux (from the install's bin directory)
./elpida-affinity-test --repeats=3 --csv=affinity-linux.csv
# Windows
elpida-affinity-test.exe --repeats=3 --csv=affinity-windows.csv
```

Options: `--repeats=N`, `--processor=N` (pin to this processor instead of the detected one), `--no-detect` (skip the
info dumper's search, which pins to every core; needs `--processor`), `--filter=TEXT` (only benchmarks whose name
contains it), `--benchmarks-directory=DIR`, `--csv=FILE` (every run).

Output: per benchmark the mean and CV% (run-to-run spread) of both modes and the difference, then the single-thread
score of both modes. `Diff% > 0` means faster when the OS schedules. A difference smaller than the CV% is noise.

For meaningful numbers: close other programs, run on power (laptops), use a performance power plan/governor where
possible, and run the same machine on Linux and Windows to compare the two schedulers. On hybrid or multi-CCD CPUs also
try `--processor` with a core of each kind.

## Results (2026-11-12 to 2026-11-14)

[`affinity-results/`](affinity-results/)`<machine>/Run<N>.txt` (next to this file), 4 runs of `--repeats=3` each. Score difference = OS-scheduled vs pinned
(negative: pinned is faster). The pinned processor is the info dumper's pick in each run.

| Machine | OS | Pinned processor(s) | Score difference | Notes |
|---|---|---|---|---|
| Ryzen 9 7950X3D (desktop) | Linux | | +1.6% (1 run) | OS faster in all 18 |
| Ryzen 9 7950X3D (desktop, 2026-11-14) | Linux, `amd_x3d_mode=frequency` | info dumper's pick | −0.5% | `affinity-results/x3d-default.csv`; 8/18 beyond CV |
| Ryzen 9 7950X3D (desktop, 2026-11-14) | Linux | 0 (V-Cache CCD) | +11.2% | `x3d-cpu0.csv`; OS faster in 18/18 |
| Ryzen 9 7950X3D (desktop, 2026-11-14) | Linux | 10 (frequency CCD, top prefcore rank 236) | +0.3% | `x3d-cpu10.csv`; noise (5/18 beyond CV) |
| Core i9-10900K (desktop) | Ubuntu 26.04 | 6, 7 | −2.0 … −2.9% | pinned faster in 18/18 every run; 6/7 are probably the Turbo Boost Max 3.0 cores |
| Core i9-10900K (desktop) | Windows 10 | 12, 14 (= cores 6, 7) | 0 … −1.6% | OS-scheduled Matrix Inverse CV ~12% |
| Core i7-1065G7 (laptop, AC) | Kubuntu 24.04 | 1, 2, 3 | +0.2 … −1.8% | |
| Core i7-1065G7 (laptop, battery) | Kubuntu 24.04 | 0, 1, 3 | +0.3 … −0.8% | noise |
| Ryzen 7 PRO 8840HS (laptop, AC) | Windows 11 | 6, 10, 14 | −0.2 … −1.3% | |
| Ryzen 7 PRO 8840HS (laptop, battery) | Windows 11 | 2, 4, 14 | +1.4 … −3.9% | single outliers (one OS-scheduled FFT run at 5.5 instead of 32) |
| Pentium Dual-Core E5200 | Ubuntu 22.04 | 0 | unusable | every benchmark randomly runs at full speed or exactly half (2.5 vs 1.2 GHz) in both modes: clock/thermal problem of the machine, not affinity |

Observations:

- The differences are small (≤ 3%) and their sign depends on the CPU: pinned wins on the 10900K (favored cores) and
  slightly on most laptops on AC, the OS is equal to the best core on the 7950X3D, battery runs are noise.
- The info dumper picks a different processor from run to run on every machine whose cores are equal (1065G7,
  8840HS, 10900K on Windows between its two favored cores), so the small gain on those machines comes from not
  migrating, not from finding a faster core.
- 7950X3D re-run (2026-11-14, three `--repeats=3` runs, compared against pinned-to-10): OS-scheduled lands within
  ±0.3% of the best core in all three runs, pinning to the best core gains nothing, pinning to a V-Cache core (0) loses
  ~10%. The OS ranking (`/sys/devices/system/cpu/cpu*/cpufreq/amd_pstate_prefcore_ranking`, same values as
  `acpi_cppc/highest_perf`) puts 10 and 13 on top (236); the info dumper picked 12 and 14 (226, 206) in earlier runs.
  So a wrong pick costs far more than OS scheduling ever did on any machine.
