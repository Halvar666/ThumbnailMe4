param(
    [Parameter(Mandatory=$true)][string]$FFmpegBin,
    [Parameter(Mandatory=$true)][string]$DestDir
)

$ErrorActionPreference = 'Stop'

function Resolve-ExistingPath([string]$PathValue) {
    $resolved = Resolve-Path -LiteralPath $PathValue -ErrorAction Stop
    return $resolved.Path
}

$FFmpegBin = Resolve-ExistingPath $FFmpegBin
$DestDir = Resolve-ExistingPath $DestDir

$dumpbin = Get-Command dumpbin.exe -ErrorAction SilentlyContinue
if (-not $dumpbin) {
    Write-Warning 'dumpbin.exe was not found. Falling back to copying all FFmpeg DLL files.'
    Copy-Item -LiteralPath (Join-Path $FFmpegBin '*.dll') -Destination $DestDir -Force
    exit 0
}

$seedPatterns = @(
    'avcodec-*.dll',
    'avformat-*.dll',
    'avutil-*.dll',
    'swscale-*.dll'
)

$allowedSystemDlls = @(
    'KERNEL32.dll','USER32.dll','GDI32.dll','ADVAPI32.dll','SHELL32.dll','OLE32.dll','OLEAUT32.dll',
    'COMDLG32.dll','COMCTL32.dll','WS2_32.dll','CRYPT32.dll','BCRYPT.dll','VERSION.dll','SHLWAPI.dll',
    'MSVCRT.dll','VCRUNTIME140.dll','VCRUNTIME140_1.dll','MSVCP140.dll','api-ms-win-crt-runtime-l1-1-0.dll',
    'api-ms-win-crt-heap-l1-1-0.dll','api-ms-win-crt-string-l1-1-0.dll','api-ms-win-crt-stdio-l1-1-0.dll',
    'api-ms-win-crt-convert-l1-1-0.dll','api-ms-win-crt-math-l1-1-0.dll','api-ms-win-crt-filesystem-l1-1-0.dll',
    'api-ms-win-crt-time-l1-1-0.dll','api-ms-win-crt-environment-l1-1-0.dll','api-ms-win-crt-utility-l1-1-0.dll',
    'api-ms-win-crt-locale-l1-1-0.dll','api-ms-win-crt-multibyte-l1-1-0.dll','api-ms-win-crt-private-l1-1-0.dll'
) | ForEach-Object { $_.ToLowerInvariant() }

function Get-Dependents([string]$FilePath) {
    $lines = & $dumpbin.Source /DEPENDENTS $FilePath 2>$null
    $deps = New-Object System.Collections.Generic.List[string]
    foreach ($line in $lines) {
        $trim = $line.Trim()
        if ($trim -match '^[A-Za-z0-9_.+\-]+\.dll$') {
            $deps.Add($trim) | Out-Null
        }
    }
    return $deps
}

function Copy-ByName([string]$Name, [System.Collections.Queue]$Queue, [hashtable]$Copied) {
    $lower = $Name.ToLowerInvariant()
    if ($Copied.ContainsKey($lower)) { return }

    $src = Join-Path $FFmpegBin $Name
    if (-not (Test-Path -LiteralPath $src)) {
        if (-not ($allowedSystemDlls -contains $lower)) {
            Write-Host "  external/system dependency: $Name"
        }
        return
    }

    Copy-Item -LiteralPath $src -Destination $DestDir -Force
    $dst = Join-Path $DestDir $Name
    $Copied[$lower] = $true
    $Queue.Enqueue($dst)
    Write-Host "  copied: $Name"
}

$queue = [System.Collections.Queue]::new()
$copied = @{}

Write-Host 'Copying minimal FFmpeg runtime dependency closure...'
foreach ($pattern in $seedPatterns) {
    $matches = Get-ChildItem -LiteralPath $FFmpegBin -Filter $pattern -File -ErrorAction SilentlyContinue
    if (-not $matches) {
        throw "Required FFmpeg DLL pattern not found: $pattern in $FFmpegBin"
    }
    foreach ($m in $matches) {
        Copy-ByName $m.Name $queue $copied
    }
}

while ($queue.Count -gt 0) {
    $current = [string]$queue.Dequeue()
    foreach ($dep in Get-Dependents $current) {
        Copy-ByName $dep $queue $copied
    }
}

$copiedFiles = Get-ChildItem -LiteralPath $DestDir -File | Where-Object { $_.Name.ToLowerInvariant().EndsWith('.dll') -and $copied.ContainsKey($_.Name.ToLowerInvariant()) }
$total = ($copiedFiles | Measure-Object Length -Sum).Sum
if ($null -eq $total) { $total = 0 }
Write-Host ''
Write-Host ("Copied FFmpeg runtime DLLs: {0}" -f $copiedFiles.Count)
Write-Host ("Copied FFmpeg runtime size: {0:N2} MiB" -f ($total / 1MB))
