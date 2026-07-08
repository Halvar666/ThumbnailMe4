param(
    [Parameter(Mandatory=$true)]
    [string]$OutDir
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $OutDir)) {
    throw "Output directory does not exist: $OutDir"
}

$copied = 0

function Copy-RuntimeDllsFromDir {
    param([string]$Dir)
    if (-not (Test-Path -LiteralPath $Dir)) { return }
    Get-ChildItem -LiteralPath $Dir -Filter '*.dll' -File -ErrorAction SilentlyContinue | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination $OutDir -Force
        $script:copied++
    }
}

if ($env:VCToolsRedistDir) {
    Get-ChildItem -LiteralPath $env:VCToolsRedistDir -Recurse -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -match '\\x64\\Microsoft\.VC.*\.CRT$' } |
        ForEach-Object { Copy-RuntimeDllsFromDir $_.FullName }
}

$vsRoots = @(
    'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Redist\MSVC',
    'C:\Program Files\Microsoft Visual Studio\18\BuildTools\VC\Redist\MSVC',
    'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC',
    'C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Redist\MSVC'
)

foreach ($root in $vsRoots) {
    if (Test-Path -LiteralPath $root) {
        Get-ChildItem -LiteralPath $root -Recurse -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.FullName -match '\\x64\\Microsoft\.VC.*\.CRT$' } |
            Sort-Object FullName -Descending |
            Select-Object -First 1 |
            ForEach-Object { Copy-RuntimeDllsFromDir $_.FullName }
    }
}

if ($copied -eq 0) {
    Write-Host 'MSVC runtime DLLs were not found by helper. windeployqt may still have copied them.'
} else {
    Write-Host "Copied MSVC runtime DLLs: $copied"
}
