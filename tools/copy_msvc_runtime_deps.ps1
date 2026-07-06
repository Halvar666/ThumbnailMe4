param(
    [Parameter(Mandatory=$true)]
    [string]$DestDir
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $DestDir)) {
    throw "Destination folder does not exist: $DestDir"
}

$runtimeNames = @(
    "vcruntime140.dll",
    "vcruntime140_1.dll",
    "msvcp140.dll",
    "msvcp140_1.dll",
    "msvcp140_2.dll",
    "concrt140.dll"
)

$requiredNames = @(
    "vcruntime140.dll",
    "vcruntime140_1.dll",
    "msvcp140.dll"
)

$candidateDirs = New-Object System.Collections.Generic.List[string]

if ($env:VCToolsRedistDir) {
    Get-ChildItem -LiteralPath $env:VCToolsRedistDir -Directory -Recurse -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -match '\\x64\\Microsoft\.VC.*\.CRT$' } |
        ForEach-Object { $candidateDirs.Add($_.FullName) }
}

$vsRoots = @(
    "C:\Program Files\Microsoft Visual Studio\18",
    "C:\Program Files\Microsoft Visual Studio\2026",
    "C:\Program Files\Microsoft Visual Studio\2022"
)

foreach ($root in $vsRoots) {
    if (Test-Path -LiteralPath $root) {
        Get-ChildItem -LiteralPath $root -Directory -Recurse -ErrorAction SilentlyContinue |
            Where-Object { $_.FullName -match '\\VC\\Redist\\MSVC\\[^\\]+\\x64\\Microsoft\.VC.*\.CRT$' } |
            ForEach-Object { $candidateDirs.Add($_.FullName) }
    }
}

foreach ($dir in ($env:PATH -split ';')) {
    if ($dir -and (Test-Path -LiteralPath $dir)) {
        foreach ($name in $runtimeNames) {
            if (Test-Path -LiteralPath (Join-Path $dir $name)) {
                $candidateDirs.Add($dir)
                break
            }
        }
    }
}

$dirs = $candidateDirs | Select-Object -Unique

if (-not $dirs -or $dirs.Count -eq 0) {
    Write-Warning "Could not find Visual C++ runtime directory."
    Write-Warning "Install Microsoft Visual C++ Redistributable x64 or run from a Visual Studio Developer Command Prompt."
    exit 1
}

$copied = 0
$missingRequired = New-Object System.Collections.Generic.List[string]

Write-Host "Copying app-local MSVC runtime DLLs..."

foreach ($name in $runtimeNames) {
    $src = $null
    foreach ($dir in $dirs) {
        $candidate = Join-Path $dir $name
        if (Test-Path -LiteralPath $candidate) {
            $src = $candidate
            break
        }
    }

    if ($src) {
        Copy-Item -LiteralPath $src -Destination (Join-Path $DestDir $name) -Force
        $sizeMb = [math]::Round((Get-Item -LiteralPath $src).Length / 1MB, 2)
        Write-Host ("OK: {0} ({1} MiB)" -f $name, $sizeMb)
        $copied++
    } elseif ($requiredNames -contains $name) {
        $missingRequired.Add($name)
    } else {
        Write-Host "Note: optional runtime DLL not found: $name"
    }
}

if ($missingRequired.Count -gt 0) {
    Write-Error ("Missing required MSVC runtime DLL(s): " + ($missingRequired -join ", "))
    exit 1
}

Write-Host "MSVC runtime DLL copy finished. Files copied: $copied"
