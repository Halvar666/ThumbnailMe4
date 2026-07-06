param(
    [Parameter(Mandatory=$true)][string]$DistDir,
    [int]$Top = 60
)

$ErrorActionPreference = 'Stop'
$DistDir = (Resolve-Path -LiteralPath $DistDir).Path

$files = Get-ChildItem -LiteralPath $DistDir -Recurse -File -Force
$total = ($files | Measure-Object Length -Sum).Sum
if ($null -eq $total) { $total = 0 }

Write-Host ''
Write-Host 'Package size audit'
Write-Host '=================='
Write-Host ("Folder: {0}" -f $DistDir)
Write-Host ("Files:  {0}" -f $files.Count)
Write-Host ("Total:  {0:N2} MiB" -f ($total / 1MB))
Write-Host ''
Write-Host ("Top {0} largest files:" -f $Top)
$files | Sort-Object Length -Descending | Select-Object -First $Top @{Name='MiB';Expression={[math]::Round($_.Length / 1MB, 2)}}, @{Name='Path';Expression={$_.FullName.Substring($DistDir.Length + 1)}} | Format-Table -AutoSize

$ffmpeg = $files | Where-Object { $_.Name -match '^(avcodec|avformat|avutil|swscale|swresample|avfilter|avdevice|postproc)-.*\.dll$' -or $_.Name -match '^(lib|zlib|bz2|iconv|intl|lzma|xml2|x264|x265|vpx|aom|dav1d|openh264|svt|Svt|zimg|shaderc|spirv|vulkan).+\.dll$' }
if ($ffmpeg) {
    $ffTotal = ($ffmpeg | Measure-Object Length -Sum).Sum
    Write-Host ''
    Write-Host ("Probable FFmpeg/runtime DLLs: {0} files, {1:N2} MiB" -f $ffmpeg.Count, ($ffTotal / 1MB))
    $ffmpeg | Sort-Object Length -Descending | Select-Object @{Name='MiB';Expression={[math]::Round($_.Length / 1MB, 2)}}, Name | Format-Table -AutoSize
}

$zip = Join-Path (Split-Path $DistDir -Parent) ((Split-Path $DistDir -Leaf) + '.zip')
if (Test-Path -LiteralPath $zip) {
    $zipItem = Get-Item -LiteralPath $zip
    Write-Host ''
    Write-Host ("ZIP:    {0:N2} MiB" -f ($zipItem.Length / 1MB))
}
