@echo off
setlocal EnableExtensions

cd /d "%~dp0\.."
set RELEASE_BASENAME=ThumbnailMe4-4b2-win64
set DIST_DIR=%CD%\dist\%RELEASE_BASENAME%

if not exist "%DIST_DIR%" (
    echo ERROR: Windows ZIP release folder does not exist:
    echo   "%DIST_DIR%"
    exit /b 1
)

echo Checking Windows ZIP release folder:
echo   "%DIST_DIR%"
echo.

set MISSING=0

for %%F in (
    ThumbnailMe4.exe
    ThumbnailMeWorker.exe
    Qt6Core.dll
    Qt6Gui.dll
    Qt6Widgets.dll
) do (
    if exist "%DIST_DIR%\%%F" (
        echo OK: %%F
    ) else (
        echo MISSING: %%F
        set MISSING=1
    )
)

for %%F in (
    VCRUNTIME140.dll
    VCRUNTIME140_1.dll
    MSVCP140.dll
    MSVCP140_1.dll
    MSVCP140_2.dll
) do (
    if exist "%DIST_DIR%\%%F" (
        echo OK: %%F
    ) else (
        echo WARNING: %%F not bundled. Clean Windows systems may fail to start without the Microsoft Visual C++ runtime.
        if /I "%%F"=="VCRUNTIME140.dll" set MISSING=1
        if /I "%%F"=="VCRUNTIME140_1.dll" set MISSING=1
        if /I "%%F"=="MSVCP140.dll" set MISSING=1
    )
)

dir /b "%DIST_DIR%\avcodec-*.dll" >nul 2>nul
if errorlevel 1 (echo MISSING: avcodec-*.dll & set MISSING=1) else echo OK: avcodec-*.dll

dir /b "%DIST_DIR%\avformat-*.dll" >nul 2>nul
if errorlevel 1 (echo MISSING: avformat-*.dll & set MISSING=1) else echo OK: avformat-*.dll

dir /b "%DIST_DIR%\avutil-*.dll" >nul 2>nul
if errorlevel 1 (echo MISSING: avutil-*.dll & set MISSING=1) else echo OK: avutil-*.dll

dir /b "%DIST_DIR%\swscale-*.dll" >nul 2>nul
if errorlevel 1 (echo MISSING: swscale-*.dll & set MISSING=1) else echo OK: swscale-*.dll

if exist "%DIST_DIR%\imageformats" (
    echo OK: imageformats folder
) else (
    echo MISSING: imageformats folder
    set MISSING=1
)

if exist "%DIST_DIR%\imageformats\qwebp.dll" (
    echo OK: imageformats\qwebp.dll
) else (
    echo WARNING: imageformats\qwebp.dll not found. WebP output will not work unless Qt has another WebP writer.
)

if exist "%DIST_DIR%\imageformats\qjpeg.dll" (
    echo OK: imageformats\qjpeg.dll
) else (
    echo WARNING: imageformats\qjpeg.dll not found. JPEG output may fail if JPEG support is not built into QtGui.
)

for %%F in (
    LICENSE.txt
    NOTICE.md
    README.txt
    licenses\README_LICENSES.txt
    licenses\THIRD_PARTY_NOTICES.txt
    licenses\GPL-2.0-or-later.txt
    licenses\Qt\README_QT.txt
    licenses\FFmpeg\README_FFMPEG.txt
    licenses\MediaInfo\README_MEDIAINFO.txt
) do (
    if exist "%DIST_DIR%\%%F" (
        echo OK: %%F
    ) else (
        echo MISSING: %%F
        set MISSING=1
    )
)

dir /b "%DIST_DIR%\MediaInfo*.dll" >nul 2>nul
if errorlevel 1 (
    dir /b "%DIST_DIR%\libmediainfo*.dll" >nul 2>nul
    if errorlevel 1 (
        echo WARNING: MediaInfo DLL not found. App will run with FFmpeg fallback metadata.
    ) else echo OK: libmediainfo*.dll
) else echo OK: MediaInfo*.dll

if exist "%DIST_DIR%\ffmpeg.exe" echo WARNING: ffmpeg.exe is present but should not be needed for normal runtime.
if exist "%DIST_DIR%\ffprobe.exe" echo WARNING: ffprobe.exe is present but should not be needed for normal runtime.
if exist "%DIST_DIR%\ffplay.exe" echo WARNING: ffplay.exe is present but should not be needed for normal runtime.

for %%F in (
    d3dcompiler_47.dll
    dxcompiler.dll
    dxil.dll
    vc_redist.x64.exe
) do (
    if exist "%DIST_DIR%\%%F" (
        echo WARNING: optional file %%F is present; 4b2 release script should remove it.
    ) else (
        echo OK: optional file %%F not bundled
    )
)

echo.
if "%MISSING%"=="1" (
    echo Some required files are missing.
    exit /b 1
)

echo Windows ZIP release check passed.
endlocal
