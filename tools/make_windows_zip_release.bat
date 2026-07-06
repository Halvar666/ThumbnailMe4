@echo off
setlocal EnableExtensions

rem Thumbnail me 4b2 - Windows x64 ZIP release builder.
rem Run from the project root or directly from tools.
rem Requires:
rem   build\ThumbnailMe4.exe
rem   build\ThumbnailMeWorker.exe
rem   FFMPEG_ROOT=C:\Dev\deps\ffmpeg
rem   windeployqt.exe in PATH or QT_BIN=C:\Qt\6.11.1\msvc2022_64\bin
rem Optional:
rem   MEDIAINFO_ROOT=C:\Dev\deps\mediainfo
rem   or MediaInfo DLL files already copied in build\

cd /d "%~dp0\.."

set RELEASE_VERSION=4b2
set RELEASE_BASENAME=ThumbnailMe4-4b2-win64
set BUILD_DIR=%CD%\build
set DIST_ROOT=%CD%\dist
set DIST_DIR=%DIST_ROOT%\%RELEASE_BASENAME%
set DIST_ZIP=%DIST_ROOT%\%RELEASE_BASENAME%.zip

if not exist "%BUILD_DIR%\ThumbnailMe4.exe" (
    echo ERROR: "%BUILD_DIR%\ThumbnailMe4.exe" not found.
    echo Run first: cmake --build build
    exit /b 1
)

if not exist "%BUILD_DIR%\ThumbnailMeWorker.exe" (
    echo ERROR: "%BUILD_DIR%\ThumbnailMeWorker.exe" not found.
    echo Run first: cmake --build build
    exit /b 1
)

if "%FFMPEG_ROOT%"=="" (
    echo ERROR: FFMPEG_ROOT is not set.
    echo Example: set FFMPEG_ROOT=C:\Dev\deps\ffmpeg
    exit /b 1
)

if not exist "%FFMPEG_ROOT%\bin" (
    echo ERROR: "%FFMPEG_ROOT%\bin" not found.
    exit /b 1
)

if exist "%DIST_DIR%" (
    echo Removing old Windows ZIP folder...
    rmdir /s /q "%DIST_DIR%"
)

mkdir "%DIST_DIR%" >nul 2>nul
mkdir "%DIST_DIR%\output" >nul 2>nul
mkdir "%DIST_DIR%\licenses" >nul 2>nul
mkdir "%DIST_DIR%\logs" >nul 2>nul

echo Copying EXE files...
copy /Y "%BUILD_DIR%\ThumbnailMe4.exe" "%DIST_DIR%\" >nul
copy /Y "%BUILD_DIR%\ThumbnailMeWorker.exe" "%DIST_DIR%\" >nul

echo Copying minimal FFmpeg runtime DLL set...
powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\copy_ffmpeg_runtime_deps.ps1" -FFmpegBin "%FFMPEG_ROOT%\bin" -DestDir "%DIST_DIR%"
if errorlevel 1 (
    echo ERROR: FFmpeg runtime dependency copy failed.
    exit /b 1
)

echo Copying MediaInfo DLL files...
if defined MEDIAINFO_ROOT (
    for /R "%MEDIAINFO_ROOT%" %%F in (*.dll) do copy /Y "%%F" "%DIST_DIR%\" >nul
) else (
    if exist "%BUILD_DIR%\MediaInfo*.dll" copy /Y "%BUILD_DIR%\MediaInfo*.dll" "%DIST_DIR%\" >nul
    if exist "%BUILD_DIR%\MediaInfo64*.dll" copy /Y "%BUILD_DIR%\MediaInfo64*.dll" "%DIST_DIR%\" >nul
    if exist "%BUILD_DIR%\libmediainfo*.dll" copy /Y "%BUILD_DIR%\libmediainfo*.dll" "%DIST_DIR%\" >nul
    if exist "%BUILD_DIR%\ZenLib*.dll" copy /Y "%BUILD_DIR%\ZenLib*.dll" "%DIST_DIR%\" >nul
    if exist "%BUILD_DIR%\libzen*.dll" copy /Y "%BUILD_DIR%\libzen*.dll" "%DIST_DIR%\" >nul
)

echo Running windeployqt...
where windeployqt.exe >nul 2>nul
if errorlevel 1 (
    if defined QT_BIN (
        if exist "%QT_BIN%\windeployqt.exe" (
            "%QT_BIN%\windeployqt.exe" --release --no-translations "%DIST_DIR%\ThumbnailMe4.exe"
            "%QT_BIN%\windeployqt.exe" --release --no-translations "%DIST_DIR%\ThumbnailMeWorker.exe"
        ) else (
            echo ERROR: windeployqt.exe not found in QT_BIN.
            exit /b 1
        )
    ) else (
        echo ERROR: windeployqt.exe not found in PATH and QT_BIN is not set.
        echo Example: set QT_BIN=C:\Qt\6.11.1\msvc2022_64\bin
        exit /b 1
    )
) else (
    windeployqt.exe --release --no-translations "%DIST_DIR%\ThumbnailMe4.exe"
    windeployqt.exe --release --no-translations "%DIST_DIR%\ThumbnailMeWorker.exe"
)

echo Copying Qt image format plugins...
set QT_IMAGEFORMATS=
if defined QT_BIN (
    if exist "%QT_BIN%\..\plugins\imageformats" set QT_IMAGEFORMATS=%QT_BIN%\..\plugins\imageformats
)

if "%QT_IMAGEFORMATS%"=="" (
    for %%Q in (windeployqt.exe) do (
        if exist "%%~dpQ..\plugins\imageformats" set QT_IMAGEFORMATS=%%~dpQ..\plugins\imageformats
    )
)

if not "%QT_IMAGEFORMATS%"=="" (
    mkdir "%DIST_DIR%\imageformats" >nul 2>nul
    for %%P in (qjpeg.dll qwebp.dll qpng.dll) do (
        if exist "%QT_IMAGEFORMATS%\%%P" copy /Y "%QT_IMAGEFORMATS%\%%P" "%DIST_DIR%\imageformats\" >nul
    )
) else (
    echo WARNING: Qt imageformats plugin folder not found. WebP output may not work.
)

if defined QT_BIN (
    if exist "%QT_BIN%\*webp*.dll" copy /Y "%QT_BIN%\*webp*.dll" "%DIST_DIR%\" >nul
    if exist "%QT_BIN%\*sharpyuv*.dll" copy /Y "%QT_BIN%\*sharpyuv*.dll" "%DIST_DIR%\" >nul
)

if exist "%DIST_DIR%\imageformats" (
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$dir='%DIST_DIR%\imageformats'; $keep=@('qjpeg.dll','qwebp.dll','qpng.dll'); Get-ChildItem -LiteralPath $dir -Filter '*.dll' -File | Where-Object { $keep -notcontains $_.Name } | Remove-Item -Force"
)

echo Copying app-local MSVC runtime DLLs...
powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\copy_msvc_runtime_deps.ps1" -DestDir "%DIST_DIR%"
if errorlevel 1 (
    echo ERROR: MSVC runtime dependency copy failed.
    exit /b 1
)

echo Copying Windows ZIP docs, licenses and default settings...
if exist "%CD%\dist_files\README_WINDOWS_ZIP.txt" copy /Y "%CD%\dist_files\README_WINDOWS_ZIP.txt" "%DIST_DIR%\README.txt" >nul
if exist "%CD%\LICENSE" copy /Y "%CD%\LICENSE" "%DIST_DIR%\LICENSE.txt" >nul
if exist "%CD%\NOTICE.md" copy /Y "%CD%\NOTICE.md" "%DIST_DIR%\NOTICE.md" >nul
if exist "%CD%\CREDITS.md" copy /Y "%CD%\CREDITS.md" "%DIST_DIR%\CREDITS.md" >nul
if exist "%CD%\dist_files\settings.ini.example" (
    if not exist "%DIST_DIR%\settings.ini" copy /Y "%CD%\dist_files\settings.ini.example" "%DIST_DIR%\settings.ini" >nul
)
if exist "%CD%\dist_files\licenses" (
    xcopy /E /I /Y "%CD%\dist_files\licenses" "%DIST_DIR%\licenses" >nul
)

echo.
echo Removing non-runtime leftovers if present...
del /Q "%DIST_DIR%\*.pdb" >nul 2>nul
del /Q "%DIST_DIR%\*.lib" >nul 2>nul
del /Q "%DIST_DIR%\*.exp" >nul 2>nul
del /Q "%DIST_DIR%\ffmpeg.exe" >nul 2>nul
del /Q "%DIST_DIR%\ffprobe.exe" >nul 2>nul
del /Q "%DIST_DIR%\ffplay.exe" >nul 2>nul

rem Qt/windeployqt may bundle DirectX shader compiler files for broader
rem graphics/QML scenarios. Thumbnail me 4 is a classic Qt Widgets app and
rem does not use them directly; tested for the beta 2 package without these files.
del /Q "%DIST_DIR%\d3dcompiler_47.dll" >nul 2>nul
del /Q "%DIST_DIR%\dxcompiler.dll" >nul 2>nul
del /Q "%DIST_DIR%\dxil.dll" >nul 2>nul

rem Do not bundle the Visual C++ Redistributable installer in the ZIP.
rem Required MSVC runtime DLLs are copied app-locally by copy_msvc_runtime_deps.ps1.
del /Q "%DIST_DIR%\vc_redist.x64.exe" >nul 2>nul

echo.
echo Windows ZIP folder created:
echo   "%DIST_DIR%"
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command "$d='%DIST_DIR%'; $z='%DIST_ZIP%'; $root=Join-Path (Split-Path $d -Parent) '_ziproot'; $leaf=Split-Path $d -Leaf; if(Test-Path $z){Remove-Item $z -Force}; if(Test-Path $root){Remove-Item $root -Recurse -Force}; New-Item -ItemType Directory -Path $root | Out-Null; Copy-Item -Path $d -Destination (Join-Path $root $leaf) -Recurse -Force; Compress-Archive -Path (Join-Path $root $leaf) -DestinationPath $z -Force; Remove-Item $root -Recurse -Force"

if exist "%DIST_ZIP%" (
    echo ZIP created:
    echo   "%DIST_ZIP%"
    echo ZIP layout: %RELEASE_BASENAME%\*
    echo ZIP top-level check:
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$z='%DIST_ZIP%'; Add-Type -AssemblyName System.IO.Compression.FileSystem; $zip=[IO.Compression.ZipFile]::OpenRead($z); try { $zip.Entries | Select-Object -First 8 | ForEach-Object { '  ' + $_.FullName } } finally { $zip.Dispose() }"
) else (
    echo WARNING: ZIP was not created, but Windows ZIP folder exists.
)

echo.
echo Running package size audit...
powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\audit_windows_package_size.ps1" -DistDir "%DIST_DIR%" -Top 40

endlocal
