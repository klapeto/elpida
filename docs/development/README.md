# Elpida development notes

Internal documentation for developers. The user documentation is the website in `docs/` (the `.html` files).

## Principles

1. **Benchmarks run the same code everywhere.** Results must be comparable across platforms and CPUs, so every build
   runs the same machine code in the benchmarked (timed) tasks wherever the ISA allows, not just the same source.
   This applies to every platform, Android included. Only the benchmarks are held to this; tools such as
   `elpida-info-dumper` and the app libraries may differ per platform when that helps.
2. **Run on as many machines as possible.** Old operating systems and devices matter (Windows XP, Android 5.0+,
   CPUs without SSE3). Raising a minimum (OS version, minSdk, CPU baseline) is a last resort. When it conflicts with
   principle 1, look for a design that keeps both (for example the static Android benchmarks).

## Documents

| Document | Contents |
|---|---|
| [code-parity.md](code-parity.md) | The parity policy, compiler flags, CPU baselines and a summary of every component |
| [parity-checks.md](parity-checks.md) | `scripts/parity/disasm-parity.py`: checking and comparing the code the timed tasks run |
| [third-party-libraries.md](third-party-libraries.md) | Audit of each library the benchmarks use, for runtime CPU dispatch and per-target code |
| [toolchain.md](toolchain.md) | The llvm-mingw toolchain, the LLVM libraries of the Compilation benchmark, rebuilding |
| [android.md](android.md) | How Android runs the benchmarks: static executables, TLS, ABIs, supported devices |
| [android-toolchain.md](android-toolchain.md) | Step by step: building the Android targets, their libraries and the app's native code |
| [mimalloc.md](mimalloc.md) | Replacing the system allocator with mimalloc on Windows (XP), Linux and Android |
| [single-thread-affinity.md](single-thread-affinity.md) | Final decision: single-thread benchmarks run without affinity everywhere; measurements; `elpida-affinity-test` |
| [virtualization-detection.md](virtualization-detection.md) | Detecting virtual machines: results from VMs are uploaded with a flag and left out of the statistics |
| [known-issues.md](known-issues.md) | Open problems and remaining divergences |
