# Puts this PowerShell into a job object with a CPU rate limit, to test Elpida's CPU availability check
# (docs/development/cpu-availability.md). Programs started from this window inherit the job, so run elpida-cli from
# it. A job can't be removed from a process: close the window afterwards.
#
#   powershell -ExecutionPolicy Bypass -NoExit -File .\job-cpu-cap.ps1 -Percent 25
#   powershell -ExecutionPolicy Bypass -NoExit -File .\job-cpu-cap.ps1 -Percent 25 -MinMax
#
# -Percent is a share of all logical processors (25 on 16 logical processors = 4 CPUs' worth). The default is a hard
# cap (JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP); -MinMax sets the max rate instead (JOB_OBJECT_CPU_RATE_CONTROL_MIN_MAX_RATE).
# Windows 8 and later (nested jobs, CPU rate control).

param(
    [ValidateRange(1, 100)][int]$Percent = 25,
    [switch]$MinMax
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

# The rate is in 1/100 of a percent; with MIN_MAX_RATE the max rate is the high 16 bits (min rate 0)
$rate = [uint32]($Percent * 100)
$info = New-Object ElpidaJob+CpuRate
if ($MinMax) {
    $info.ControlFlags = $Enable -bor $MinMaxRate
    $info.Rate = [uint32]($rate -shl 16)
} else {
    $info.ControlFlags = $Enable -bor $HardCap
    $info.Rate = $rate
}

if (-not [ElpidaJob]::SetInformationJobObject($job, $JobObjectCpuRateControlInformation, [ref]$info, 8)) {
    throw "SetInformationJobObject failed: $([Runtime.InteropServices.Marshal]::GetLastWin32Error())"
}
if (-not [ElpidaJob]::AssignProcessToJobObject($job, [ElpidaJob]::GetCurrentProcess())) {
    throw "AssignProcessToJobObject failed: $([Runtime.InteropServices.Marshal]::GetLastWin32Error())"
}

$kind = if ($MinMax) { "max rate" } else { "hard cap" }
"This PowerShell and the programs started from it are limited to $Percent% of all logical processors ($kind)."
