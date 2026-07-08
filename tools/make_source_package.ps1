$ErrorActionPreference = 'Stop'

$Root = (Get-Location).Path
$Version = '4b2q'
$Dist = Join-Path $Root 'dist'
$StageRoot = Join-Path $Dist '_sourceziproot'
$SourceDir = Join-Path $StageRoot "ThumbnailMe4-$Version-source"
$ZipFile = Join-Path $Dist "ThumbnailMe4-$Version-source.zip"

Write-Host 'Preparing clean source tree:'
Write-Host "  `"$SourceDir`""
Write-Host ''

if (Test-Path -LiteralPath $StageRoot) {
    Remove-Item -LiteralPath $StageRoot -Recurse -Force
}
if (-not (Test-Path -LiteralPath $Dist)) {
    New-Item -ItemType Directory -Path $Dist | Out-Null
}
New-Item -ItemType Directory -Path $SourceDir -Force | Out-Null

$excludeDirs = @('.git', '.vs', '.vscode', 'build', 'build-linux', 'build-win', 'dist', '__pycache__')
$excludeFiles = @('*.user','*.suo','*.pdb','*.ilk','*.obj','*.pch','*.ncb','*.opensdf','*.sdf','*.ipch')

Write-Host 'Copying source files...'
& robocopy $Root $SourceDir /E /XD $excludeDirs /XF $excludeFiles | Write-Host
if ($LASTEXITCODE -ge 8) {
    throw "robocopy failed with exit code $LASTEXITCODE"
}

# Clean accidental root artifacts that appeared during earlier manual fixes.
Remove-Item -LiteralPath (Join-Path $SourceDir 'make_source_package.bat') -Force -ErrorAction SilentlyContinue
Get-ChildItem -LiteralPath $SourceDir -Force -File -ErrorAction SilentlyContinue |
    Where-Object { $_.Name -like '*ource tree*' } |
    Remove-Item -Force -ErrorAction SilentlyContinue

Write-Host ''
Write-Host 'Normalizing timestamps for ZIP compatibility...'
$min = [datetime]'1980-01-01 00:00:00'
$max = [datetime]'2107-12-31 23:59:58'
$safe = [datetime]'2026-01-01 12:00:00'

Get-ChildItem -LiteralPath $SourceDir -Recurse -Force | ForEach-Object {
    if ($_.LastWriteTime -lt $min -or $_.LastWriteTime -gt $max) { $_.LastWriteTime = $safe }
    if ($_.CreationTime -lt $min -or $_.CreationTime -gt $max) { $_.CreationTime = $safe }
    if ($_.LastAccessTime -lt $min -or $_.LastAccessTime -gt $max) { $_.LastAccessTime = $safe }
}

if (Test-Path -LiteralPath $ZipFile) {
    Remove-Item -LiteralPath $ZipFile -Force
}

Write-Host ''
Write-Host 'Creating source package:'
Write-Host "  `"$ZipFile`""
Write-Host ''

Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::CreateFromDirectory($SourceDir, $ZipFile, [System.IO.Compression.CompressionLevel]::Optimal, $false)

if (-not (Test-Path -LiteralPath $ZipFile)) {
    throw 'source ZIP was not created'
}

Write-Host ''
Write-Host 'Source package created successfully:'
Write-Host "  `"$ZipFile`""
Write-Host ''
