# CPU availability check

A full benchmark run on a restricted CPU isn't uploaded. Restricted means the process may run on fewer CPUs than are
online, or has a CPU time quota below that. Such a result is lower than the CPU can do, and the uploaded topology
shows only the allowed cores. hwloc builds the topology from the allowed CPUs, because Elpida doesn't ask it to
include the others. So under `taskset -c 0-3`, a 16-core CPU would upload as a 4-core one and pull down that CPU's
statistics. Decided 2026-11-14: block, as Android already did, and not flag as with
[VMs](virtualization-detection.md): a restricted run is never useful for the statistics.

The messages say what is restricted and why, and ask users who think it's wrong to report it at
`https://gitlab.com/dev-hood/elpida/elpida/-/issues` (`Elpida::Application::ReportIssueUrl`; the Android strings in
`Resources.resx` repeat it).

## When it's checked

- **Before the run:** Qt shows the reason with Continue/Cancel (`FullBenchmarkView`). The CLI prints it to stderr
  and runs anyway. Android shows its own dialog (`BenchmarkViewModel`).
- **Before and after every benchmark** (`FullBenchmarkController`, from the thread that starts the benchmark
  processes, which inherit its affinity). If it was restricted at any point, the results aren't uploaded and a
  warning says why. The CLI prints it to stderr, also with `--quiet`, so stdout stays only the results. A run with upload off still gets the warning before it, not after.
- Unknown (the values can't be read) doesn't block.

## What is checked

`Elpida::CpuAvailabilityLoader` (`include/Elpida/Platform/CpuAvailabilityLoader.hpp`):

| | Linux | Android | Windows |
|---|---|---|---|
| Allowed CPUs | `Cpus_allowed_list` of `/proc/self/task/<tid>/status` (falls back to `sched_getaffinity`) | same | `GetProcessAffinityMask`, process mask within the system mask |
| Online CPUs | `/sys/devices/system/cpu/online` | same | system affinity mask |
| CPU quota | cgroup v2 `cpu.max`, v1 `cpu.cfs_quota_us`/`cpu.cfs_period_us`, smallest on the way to the root | not checked | job object hard CPU rate cap (`JobObjectCpuRateControlInformation`, Windows 8+) |

- **Offline CPUs don't count.** On Android power management parks cores. On desktop it's SMT or cores turned off in
  the BIOS or by the user, which is a real configuration and the uploaded topology shows it.
- **Linux/Android allowed CPUs:** not `sched_getaffinity()`, which on recent Android kernels also leaves out the
  CPUs the kernel has paused (idle big cores that stay online). That made 6 of 8 cores look restricted on an idle
  phone.
- **Android has no quota check:** apps get CPU shares and uclamp there, not quotas, and a false positive would block
  every upload from that device. Android's cpusets show up as allowed CPUs.
- **Windows processor groups:** the masks cover one group of up to 64 logical processors. A process starting in one
  group of a larger machine (Windows 10) is not restricted by the user, so the comparison stays within the group.
  When the process' threads run in several groups (the Windows 11 default), both masks are 0. That counts as
  unknown, not restricted.
- **Windows job quotas:** the rate is a share of all processors (`GetActiveProcessorCount(ALL_PROCESSOR_GROUPS)`,
  Windows 7+). Weight-based CPU rate control is not a cap and isn't counted. A job's affinity limit shows in the
  process affinity.
- **A quota counts** when it's below the allowed CPUs (by more than 0.01).

## Tested (2026-11-14)

Linux x86_64 (7950X3D, 32 logical processors), CLI with one benchmark group, uploading to `localhost`:

| Case | Result |
|---|---|
| No restriction | no warning |
| `taskset -c 0-3` | warning before (4 of 32), not uploaded |
| `taskset -a -p -c 0-7` on the running CLI, after 4 benchmarks | not uploaded (8 of 32), no warning before |
| Docker `--cpus=2.5` (cgroup v2 `cpu.max` 250000 100000) | quota 2.5 detected |
| Docker without limits | no warning |
| Qt (Xvfb), `taskset -c 0-3` | Continue/Cancel dialog, then the not-uploaded message |
| Android emulator (API 21), shared loader | allowed 4, online 4, not restricted |
| Windows (user), affinity changed on the running CLI | not uploaded |
| Android phone (user, 8 cores), normal run | uploaded, no warning |
| Android phone, notification shade opened during the run | not uploaded (7 of 8) |

The notification shade takes the app out of the top-app state: Android moves it to the `foreground` cpuset, which on
many phones leaves out the fastest core. The app stays visible, so `Window.Stopped` (the "left the screen" check)
doesn't fire; only this check catches it.

Not tested: Windows job quota, cgroup v1.
