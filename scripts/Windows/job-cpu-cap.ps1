# Puts this PowerShell into a job object with a CPU rate limit, to test Elpida's CPU availability check
# (docs/development/cpu-availability.md). Programs started from this window inherit the job, so run elpida-cli from
# it. A job can't be removed from a process: close the window afterwards.
#
#   powershell -ExecutionPolicy Bypass -NoExit -File .\job-cpu-cap.ps1 -Percent 25
#   powershell -ExecutionPolicy Bypass -NoExit -File .\job-cpu-cap.ps1 -Percent 25 -MinMax
#
# -Percent is a share of all logical processors (25 on 16 logical processors = 4 CPUs' worth). The default is a hard
# cap (JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP); -MinMax sets the max rate instead (JOB_OBJECT_CPU_RATE_CONTROL_MIN_MAX_RATE),
# with -MinRate (percent, default 1) as the min rate: Windows refuses a min rate of 0 (error 87).
# Windows 8 and later (nested jobs, CPU rate control).

param(
    [ValidateRange(1, 100)][int]$Percent = 25,
    [switch]$MinMax,
    [ValidateRange(1, 100)][int]$MinRate = 1
)

Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class ElpidaJob {
    [StructLayout(LayoutKind.Sequential)]
    public struct CpuRate { public uint ControlFlags; public uint Rate; }
    [DllImport("kernel32.dll", SetLastError=true)]
    public static extern IntPtr CreateJobObject(IntPtr attributes, string name);
    [DllImport("kernel32.dll", SetLastError=true)]
    public static extern bool SetInformationJobObject(IntPtr job, int infoClass, ref CpuRate info, uint size);
    [DllImport("kernel32.dll", SetLastError=true)]
    public static extern bool AssignProcessToJobObject(IntPtr job, IntPtr process);
    [DllImport("kernel32.dll")]
    public static extern IntPtr GetCurrentProcess();
}
"@

$JobObjectCpuRateControlInformation = 15
$Enable = 0x1
$HardCap = 0x4
$MinMaxRate = 0x10

$job = [ElpidaJob]::CreateJobObject([IntPtr]::Zero, $null)
if ($job -eq [IntPtr]::Zero) { throw "CreateJobObject failed: $([Runtime.InteropServices.Marshal]::GetLastWin32Error())" }

# The rate is in 1/100 of a percent; with MIN_MAX_RATE the min rate is the low 16 bits and the max rate the high 16
$rate = [uint32]($Percent * 100)
$info = New-Object ElpidaJob+CpuRate
if ($MinMax) {
    $info.ControlFlags = $Enable -bor $MinMaxRate
    if ($MinRate -gt $Percent) { throw "-MinRate must not be above -Percent" }
    $info.Rate = [uint32](($rate -shl 16) -bor ($MinRate * 100))
} else {
    $info.ControlFlags = $Enable -bor $HardCap
    $info.Rate = $rate
}

if (-not [ElpidaJob]::SetInformationJobObject($job, $JobObjectCpuRateControlInformation, [ref]$info, 8)) {
    $err = [Runtime.InteropServices.Marshal]::GetLastWin32Error()
    throw ("SetInformationJobObject failed: {0} (ControlFlags=0x{1:X} Rate=0x{2:X8})" -f $err, $info.ControlFlags, $info.Rate)
}
if (-not [ElpidaJob]::AssignProcessToJobObject($job, [ElpidaJob]::GetCurrentProcess())) {
    throw "AssignProcessToJobObject failed: $([Runtime.InteropServices.Marshal]::GetLastWin32Error())"
}

$kind = if ($MinMax) { "max rate, min rate $MinRate%" } else { "hard cap" }
"This PowerShell and the programs started from it are limited to $Percent% of all logical processors ($kind)."
