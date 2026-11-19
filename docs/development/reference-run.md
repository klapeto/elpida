# Reference run: measuring the base scores

Every full-run benchmark has a single-thread and a multi-thread base score in
`apps/Application/BenchmarkBaseScores.hpp`. A result's score is the geometric mean of result / base score over the
benchmarks (`apps/Shared/ScoreCalculator.cpp`), and the total score is the square root of single-thread times
multi-thread. The base scores come from one machine, the reference machine, measured with the procedure below.

## What the reference does and does not decide

With a geometric mean, the ratio of two machines' scores is the geometric mean of their per-benchmark ratios: the base
scores cancel out. **The reference machine only sets the scale** (what a score of 1.0 means). It cannot change how two
CPUs compare or how much each benchmark weighs. Uploaded results keep the raw per-benchmark results, so scores can be
recomputed if the reference ever changes.

What matters in a reference machine is therefore:

1. **Low noise.** Each base score carries the reference run's measurement error into every result.
2. **Availability.** Base scores must be measured again whenever a benchmark changes, on the same machine and setup.
3. **A typical, untuned system.** Stock firmware and the build CI ships.

## The reference machine

HP Z2 Tower G5 Workstation with an Intel Core i9-10900K (10 cores, 20 threads), chosen on 2026-11-19. Stock HP firmware:
Intel's default power limits, no multi-core enhancement or overclocking options, memory at its JEDEC speed (no XMP).
Its run-to-run spread in full runs is at most 0.3% single-thread and about 1.5% multi-thread (three full runs of CI build 1246, 2026-11-19).

Record of the setup (keep it current; a change here means measuring the base scores again):

| Item | Value |
|---|---|
| Board / firmware | HP 8750, BIOS S50 Ver. 01.04.17 (2024-06-26) |
| CPU microcode | 0x100 |
| Memory | 64 GB (speed and channels: fill in from `sudo dmidecode -t memory`) |
| OS | Ubuntu 26.04.1 LTS, kernel 7.0.0-43-generic, `intel_pstate` driver |
| Turbo | off for reference runs |

Keep the machine frozen: no BIOS or microcode updates, and hold the kernel
(`sudo apt-mark hold linux-image-generic linux-headers-generic` and the installed `linux-image-7.0.0-43-generic`),
unless the base scores are measured again afterwards. Microcode updates have changed CPU performance before (for example
the Downfall mitigation on this CPU generation).

The benchmarks are statically linked with our own C library, allocator and C++ library, so the OS only reaches the
measurement through the kernel (scheduler, frequency driver, mitigations). Another Ubuntu version on this machine
should give the same base scores within the spread; check that with one set of runs before switching.

## When to measure again

- A full-run benchmark changes: its code, its input, its configuration in `apps/Application/FullBenchmarkInstances/`,
  or the code it calls (musl-lite, mimalloc, libc++, third-party libraries).
- The compiler, its flags or the toolchain changes.
- Anything in the setup record above changes.

Measure all benchmarks in the same session even if only one changed, so all base scores share one setup.
While Elpida is in beta, changed base scores do not need a benchmarks version bump (`ELPIDA_BENCHMARKS_VERSION_*` in
`cmake-modules/GetElpidaVersion.cmake`).

## Procedure

1. **Build.** Use the CI package of the commit (`Package Cli (x86_64-linux-gnu)`), not a local build, so the base
   scores come from the binaries users run.
2. **Turn turbo off.** In the BIOS, or until the next reboot:
   `echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo`. Check that
   `/sys/devices/system/cpu/intel_pstate/no_turbo` reads 1.
3. **Set the performance governor** (until the next reboot): `sudo cpupower frequency-set -g performance`.
4. **Make the machine idle.** Log out of the desktop and stop the display manager (`sudo systemctl stop gdm`): the
   GNOME greeter's `gnome-shell` disturbs runs. Run over ssh or on a text console. No other users or jobs; check `uptime`
   shows a load near 0 before starting.
5. **Run** 10 full runs in one CLI invocation:

   ```
   ./elpida-cli --iterations=10 --format=json --quiet --output=reference-$(date +%Y%m%d).json
   ```

   Leave everything else at its default (no affinity, default concurrency): the base scores must come from the same
   settings users run with.
6. **Check the spread.**
   `python3 scripts/reference/base-scores.py reference-YYYYMMDD.json` prints the median and the spread
   ((max - min) / median) of every benchmark. Expect at most about 1% single-thread and 2% multi-thread. If a benchmark is
   far above that, look for the cause (a background job, thermal limits) and run again rather than accept it.
7. **Write the base scores.**
   `python3 scripts/reference/base-scores.py --write reference-YYYYMMDD.json` replaces the 36 numbers in
   `apps/Application/BenchmarkBaseScores.hpp` with the medians. This is the only place to change: the Android app and
   the website get the base scores from the info dumpers (`apps/BackEndInfoDumper` writes `baseScore`), which read this
   header.
8. **Keep the evidence.** Commit the result file and the script's table under `docs/development/reference-runs/` with the
   date, together with the setup record above.
9. **Restore the machine** (turbo on, default governor) or reboot.

## Open

- **Scale.** With the base scores as measured, the reference machine scores 1.0 and older CPUs score below 1 (an E5200
  roughly 0.15). Multiplying by a constant (for example 1000) in `ScoreCalculator` would give whole numbers; it changes no
  comparison. Not decided.
