@echo off
setlocal EnableExtensions

if "%~1"=="" (
    if "%FFMPEG_ROOT%"=="" (
        echo ERROR: Pass FFmpeg root path or set FFMPEG_ROOT.
        echo Example: tools\check_ffmpeg_runtime_root.bat C:\Dev\deps\ffmpeg-tm4-mtn-style
        exit /b 1
    ) else (
        set ROOT=%FFMPEG_ROOT%
    )
) else (
    set ROOT=%~1
)

if not exist "%ROOT%\bin" (
    echo ERROR: "%ROOT%\bin" not found.
    exit /b 1
)
if not exist "%ROOT%\include" (
    echo ERROR: "%ROOT%\include" not found.
    exit /b 1
)
if not exist "%ROOT%\lib" (
    echo ERROR: "%ROOT%\lib" not found.
    exit /b 1
)

for %%P in (avcodec avformat avutil swscale) do (
    dir /b "%ROOT%\bin\%%P-*.dll" >nul 2>nul
    if errorlevel 1 (
        echo ERROR: Missing runtime DLL: %%P-*.dll
        exit /b 1
    ) else (
        echo OK: %%P runtime DLL
    )
)

rem MSVC FFmpeg builds normally install .lib import libraries.
rem If these are missing, TM4 may still run with copied DLLs, but CMake/MSVC cannot link against this root.
set MISSING_LIB=0
for %%L in (avcodec avformat avutil swscale) do (
    if exist "%ROOT%\lib\%%L.lib" (
        echo OK: %%L.lib
    ) else (
        if exist "%ROOT%\bin\%%L.lib" (
            echo WARNING: %%L.lib exists in bin, but CMake expects it in lib.
        ) else (
            echo WARNING: Missing %%L.lib
        )
        set MISSING_LIB=1
    )
)

echo.
echo FFmpeg runtime size summary:
powershell -NoProfile -ExecutionPolicy Bypass -Command "$root='%ROOT%'; Get-ChildItem -LiteralPath (Join-Path $root 'bin') -Filter '*.dll' -File | Sort-Object Length -Descending | Select-Object @{Name='MiB';Expression={[math]::Round($_.Length/1MB,2)}},Name | Format-Table -AutoSize; $sum=(Get-ChildItem -LiteralPath (Join-Path $root 'bin') -Filter '*.dll' -File | Measure-Object Length -Sum).Sum; if($null -eq $sum){$sum=0}; 'Total DLL size: {0:N2} MiB' -f ($sum/1MB)"

if "%MISSING_LIB%"=="1" (
    echo.
    echo WARNING: Some MSVC import libraries are missing.
    echo Re-run build_ffmpeg_mtn_style_runtime.bat or copy bin\*.lib to lib\.
)

endlocal
