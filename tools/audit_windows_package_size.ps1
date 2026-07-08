param(
    [string]$Folder,
    [string]$Zip
)

$ErrorActionPreference = 'Stop'

if (-not $Folder) { $Folder = Join-Path (Get-Location) 'dist\ThumbnailMe4-4b2q-win64' }
if (-not $Zip) { $Zip = Join-Path (Get-Location) 'dist\ThumbnailMe4-4b2q-win64.zip' }

if (-not (Test-Path -LiteralPath $Folder)) {
    throw "Folder does not exist: $Folder"
}

$files = Get-ChildItem -LiteralPath $Folder -Recurse -File
$total = ($files | Measure-Object Length -Sum).Sum
if (-not $total) { $total = 0 }

function MiB([long]$bytes) { [math]::Round($bytes / 1MB, 2) }

Write-Host ''
Write-Host 'Package size audit'
Write-Host '=================='
Write-Host "Folder: $Folder"
Write-Host "Files:  $($files.Count)"
Write-Host "Total:  $(MiB $total) MiB"
Write-Host ''
Write-Host 'Top 60 largest files:'
Write-Host ''
Write-Host '  MiB Path'
Write-Host '  --- ----'

$files |
    Sort-Object Length -Descending |
    Select-Object -First 60 |
    ForEach-Object {
        $rel = Resolve-Path -LiteralPath $_.FullName -Relative
        $rel = $rel -replace '^\.\\',''
        '{0,5} {1}' -f (MiB $_.Length), $rel
    }

$ff = $files | Where-Object { $_.Name -match '^(avcodec|avformat|avutil|swscale|swresample)-.*\.dll$' }
$ffTotal = ($ff | Measure-Object Length -Sum).Sum
if (-not $ffTotal) { $ffTotal = 0 }

Write-Host ''
Write-Host "Probable FFmpeg/runtime DLLs: $($ff.Count) files, $(MiB $ffTotal) MiB"
Write-Host ''
Write-Host ' MiB Name'
Write-Host ' --- ----'
$ff | Sort-Object Length -Descending | ForEach-Object {
    '{0,4} {1}' -f (MiB $_.Length), $_.Name
}

if (Test-Path -LiteralPath $Zip) {
    $zipItem = Get-Item -LiteralPath $Zip
    Write-Host ''
    Write-Host "ZIP:    $(MiB $zipItem.Length) MiB"
} else {
    Write-Host ''
    Write-Host "ZIP:    missing ($Zip)"
}
