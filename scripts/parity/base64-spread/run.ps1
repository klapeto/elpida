# Base64 decode / web benchmark spread test (Windows PowerShell 5.1+).
# Run on an idle machine:  powershell -ExecutionPolicy Bypass -File run.ps1 [-Rounds 15] [-Cpu 2]
# Writes results-<host>-<date>.txt next to this script.
param([int]$Rounds = 15, [int]$Cpu = 2)
$ErrorActionPreference = 'Continue'
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
Get-ChildItem -Path $here -Recurse -Filter *.exe | Unblock-File
$inv = [System.Globalization.CultureInfo]::InvariantCulture
$out = Join-Path $here ("results-{0}-{1}.txt" -f $env:COMPUTERNAME, (Get-Date -Format 'yyyyMMdd-HHmmss'))

function Log([string]$s) { Write-Host $s; Add-Content -Path $out -Value $s }

function Get-Score([string]$text) {
    $m = [regex]::Match($text, '"result":([0-9.eE+-]+)')
    if ($m.Success) { return [double]::Parse($m.Groups[1].Value, $inv) }
    return $null
}

function Run-Free([string]$exe, [string[]]$a) {
    $text = (& $exe @a 2>&1 | ForEach-Object { "$_" }) -join "`n"
    return Get-Score $text
}

function Run-Pinned([string]$exe, [string[]]$a) {
    $tmp = [System.IO.Path]::GetTempFileName()
    $err = [System.IO.Path]::GetTempFileName()
    $p = Start-Process -FilePath $exe -ArgumentList $a -NoNewWindow -PassThru -RedirectStandardOutput $tmp -RedirectStandardError $err
    try { $p.ProcessorAffinity = [IntPtr](1 -shl $script:Cpu) } catch { }
    $p.WaitForExit()
    $text = Get-Content -Raw $tmp
    Remove-Item $tmp, $err -ErrorAction SilentlyContinue
    return Get-Score $text
}

function Summary($data, [string[]]$order) {
    $base = $null; $raw = @()
    foreach ($k in $order) {
        $v = @($data[$k] | Where-Object { $_ -ne $null })
        if ($v.Count -eq 0) { Log ("  {0,-14} NO RESULT" -f $k); continue }
        $s = @($v | Sort-Object)
        $n = $s.Count
        if ($n % 2) { $med = $s[($n - 1) / 2] } else { $med = ($s[$n / 2 - 1] + $s[$n / 2]) / 2 }
        if ($base -eq $null) { $base = $med }
        $spread = 100 * ($s[-1] - $s[0]) / $med
        Log ([string]::Format($inv, "  {0,-14} n={1,2} median {2,12:G5}  spread {3,5:F1}%  vs first {4,6:F3}  min {5:G4} max {6:G4}", $k, $n, $med, $spread, $med / $base, $s[0], $s[-1]))
        $raw += $k + "=" + (($v | ForEach-Object { $_.ToString('G4', $inv) }) -join ',')
    }
    Log ("  raw: " + ($raw -join ';'))
}

$cpuName = (Get-CimInstance Win32_Processor | Select-Object -First 1).Name.Trim()
$os = (Get-CimInstance Win32_OperatingSystem)
$plan = (powercfg /getactivescheme) -join ' '
Log "== machine"
Log ("{0} | {1} logical processors | {2} {3}" -f $cpuName, [Environment]::ProcessorCount, $os.Caption, $os.Version)
Log ("power plan: {0}" -f $plan)
Log ("load: {0}%" -f (Get-CimInstance Win32_Processor | Measure-Object -Property LoadPercentage -Average).Average)

$args3 = @('--index=3', '--config=2048', '--format=json')
$names = @('find', 'findindex', 'lut')

Log ""; Log "== Base64 decode, $Rounds interleaved rounds, not pinned"
$d = @{}; foreach ($n in $names) { $d[$n] = @() }
for ($i = 1; $i -le $Rounds; $i++) {
    foreach ($n in $names) { $d[$n] += Run-Free (Join-Path $here "bin\web-$n.exe") $args3 }
    Write-Host -NoNewline "."
}
Write-Host ""; Summary $d $names

Log ""; Log "== Base64 decode, $Rounds interleaved rounds, pinned to CPU $Cpu"
$d = @{}; foreach ($n in $names) { $d[$n] = @() }
for ($i = 1; $i -le $Rounds; $i++) {
    foreach ($n in $names) { $d[$n] += Run-Pinned (Join-Path $here "bin\web-$n.exe") $args3 }
    Write-Host -NoNewline "."
}
Write-Host ""; Summary $d $names

Log ""; Log "== other web benchmarks (find build), $Rounds rounds each, not pinned"
$labels = @{ 0 = 'XML_Parsing'; 1 = 'Json_Parsing'; 2 = 'Base64_Encode'; 4 = 'Regex' }
foreach ($idx in 0, 1, 2, 4) {
    $l = $labels[$idx]; $d = @{ $l = @() }
    for ($i = 1; $i -le $Rounds; $i++) {
        $d[$l] += Run-Free (Join-Path $here "bin\web-find.exe") @("--index=$idx", '--config=2048', '--format=json')
        Write-Host -NoNewline "."
    }
    Write-Host ""; Summary $d @($l)
}
Log ""; Log "done: $out"
