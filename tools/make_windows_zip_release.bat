@echo off
setlocal EnableExtensions EnableDelayedExpansion

cd /d "%~dp0\.."

set "ROOT=%CD%"
set "VERSION=4b2q"
set "DIST=%ROOT%\dist"
set "OUTDIR=%DIST%\ThumbnailMe4-%VERSION%-win64"
set "ZIP_FILE=%DIST%\ThumbnailMe4-%VERSION%-win64.zip"

if not exist "%DIST%" mkdir "%DIST%"
if exist "%OUTDIR%" rmdir /s /q "%OUTDIR%"
if exist "%ZIP_FILE%" del /f /q "%ZIP_FILE%"
mkdir "%OUTDIR%"

if not defined QT_BIN (
  for %%I in (windeployqt.exe) do set "QT_BIN=%%~dp$PATH:I"
)
if defined QT_BIN if "%QT_BIN:~-1%"=="\" set "QT_BIN=%QT_BIN:~0,-1%"

if not exist "%QT_BIN%\windeployqt.exe" (
  echo ERROR: windeployqt.exe was not found.
  echo Set QT_BIN first, for example:
  echo   set QT_BIN=C:\Qt\6.11.1\msvc2022_64\bin
  exit /b 1
)

set "APP_EXE="
if exist "%ROOT%\build\ThumbnailMe4.exe" set "APP_EXE=%ROOT%\build\ThumbnailMe4.exe"
if not defined APP_EXE if exist "%ROOT%\build\Release\ThumbnailMe4.exe" set "APP_EXE=%ROOT%\build\Release\ThumbnailMe4.exe"
if not defined APP_EXE if exist "%ROOT%\build\ThumbnailMeQt6.exe" set "APP_EXE=%ROOT%\build\ThumbnailMeQt6.exe"
if not defined APP_EXE if exist "%ROOT%\build\Release\ThumbnailMeQt6.exe" set "APP_EXE=%ROOT%\build\Release\ThumbnailMeQt6.exe"

if not defined APP_EXE (
  echo ERROR: main executable was not found in build folder.
  echo Expected one of:
  echo   build\ThumbnailMe4.exe
  echo   build\Release\ThumbnailMe4.exe
  echo   build\ThumbnailMeQt6.exe
  echo   build\Release\ThumbnailMeQt6.exe
  exit /b 1
)

echo Creating Windows release folder:
echo   "%OUTDIR%"
echo.

copy /y "%APP_EXE%" "%OUTDIR%\ThumbnailMe4.exe" >nul

set "WORKER_EXE="
if exist "%ROOT%\build\ThumbnailMeWorker.exe" set "WORKER_EXE=%ROOT%\build\ThumbnailMeWorker.exe"
if not defined WORKER_EXE if exist "%ROOT%\build\Release\ThumbnailMeWorker.exe" set "WORKER_EXE=%ROOT%\build\Release\ThumbnailMeWorker.exe"
if defined WORKER_EXE copy /y "%WORKER_EXE%" "%OUTDIR%\ThumbnailMeWorker.exe" >nul

if exist "%ROOT%\dist_files\README_WINDOWS_ZIP.txt" copy /y "%ROOT%\dist_files\README_WINDOWS_ZIP.txt" "%OUTDIR%\README.txt" >nul
if exist "%ROOT%\README.md" copy /y "%ROOT%\README.md" "%OUTDIR%\README.md" >nul
if exist "%ROOT%\CREDITS.md" copy /y "%ROOT%\CREDITS.md" "%OUTDIR%\CREDITS.md" >nul
if exist "%ROOT%\NOTICE.md" copy /y "%ROOT%\NOTICE.md" "%OUTDIR%\NOTICE.md" >nul
if exist "%ROOT%\LICENSE" copy /y "%ROOT%\LICENSE" "%OUTDIR%\LICENSE.txt" >nul
if exist "%ROOT%\LICENSE.txt" copy /y "%ROOT%\LICENSE.txt" "%OUTDIR%\LICENSE.txt" >nul
if exist "%ROOT%\dist_files\settings.ini.example" copy /y "%ROOT%\dist_files\settings.ini.example" "%OUTDIR%\settings.ini" >nul
if exist "%ROOT%\dist_files\licenses" robocopy "%ROOT%\dist_files\licenses" "%OUTDIR%\licenses" /E >nul

echo Running windeployqt...
"%QT_BIN%\windeployqt.exe" --release --compiler-runtime "%OUTDIR%\ThumbnailMe4.exe"
if errorlevel 1 (
  echo ERROR: windeployqt failed.
  exit /b 1
)

if not defined FFMPEG_ROOT if exist "C:\Dev\deps\ffmpeg-tm4-mtn-style" set "FFMPEG_ROOT=C:\Dev\deps\ffmpeg-tm4-mtn-style"
if not defined FFMPEG_ROOT if exist "C:\Dev\deps\ffmpeg" set "FFMPEG_ROOT=C:\Dev\deps\ffmpeg"

if defined FFMPEG_ROOT (
  echo Copying FFmpeg runtime from:
  echo   "%FFMPEG_ROOT%"
  if exist "%FFMPEG_ROOT%\bin" copy /y "%FFMPEG_ROOT%\bin\*.dll" "%OUTDIR%\" >nul
  if exist "%ROOT%\tools\check_ffmpeg_runtime_root.bat" call "%ROOT%\tools\check_ffmpeg_runtime_root.bat" "%OUTDIR%"
) else (
  echo WARNING: FFMPEG_ROOT is not set and default FFmpeg dependency folders were not found.
)

if not defined MEDIAINFO_ROOT if exist "C:\Dev\deps\mediainfo" set "MEDIAINFO_ROOT=C:\Dev\deps\mediainfo"

if defined MEDIAINFO_ROOT (
  echo Copying MediaInfo runtime from:
  echo   "%MEDIAINFO_ROOT%"
  if exist "%MEDIAINFO_ROOT%\MediaInfo.dll" copy /y "%MEDIAINFO_ROOT%\MediaInfo.dll" "%OUTDIR%\" >nul
  if exist "%MEDIAINFO_ROOT%\bin\MediaInfo.dll" copy /y "%MEDIAINFO_ROOT%\bin\MediaInfo.dll" "%OUTDIR%\" >nul
  if exist "%MEDIAINFO_ROOT%\lib\MediaInfo.dll" copy /y "%MEDIAINFO_ROOT%\lib\MediaInfo.dll" "%OUTDIR%\" >nul
) else (
  echo WARNING: MEDIAINFO_ROOT is not set and C:\Dev\deps\mediainfo was not found.
)

echo Copying MSVC runtime if available...
powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\tools\copy_msvc_runtime_deps_4b2q.ps1" -OutDir "%OUTDIR%"
if errorlevel 1 (
  echo WARNING: MSVC runtime helper returned an error. Continuing because windeployqt may have copied it.
)

if not exist "%OUTDIR%\ThumbnailMe4.exe" (
  echo ERROR: ThumbnailMe4.exe is missing in release folder.
  exit /b 1
)

if not exist "%OUTDIR%\Qt6Core.dll" (
  echo ERROR: Qt6Core.dll is missing in release folder.
  exit /b 1
)

if not exist "%OUTDIR%\platforms\qwindows.dll" (
  echo ERROR: platforms\qwindows.dll is missing in release folder.
  exit /b 1
)

if not exist "%OUTDIR%\MediaInfo.dll" (
  echo WARNING: MediaInfo.dll is missing. MediaInfo header may not work in clean Windows installs.
)

if not exist "%OUTDIR%\avcodec-*.dll" (
  echo WARNING: FFmpeg avcodec DLL is missing. Video decoding may not work in clean Windows installs.
)

echo.
echo Removing files not wanted in portable ZIP...
del /Q %OUTDIR%\d3dcompiler_47.dll >nul 2>nul
del /Q %OUTDIR%\dxcompiler.dll >nul 2>nul
del /Q %OUTDIR%\dxil.dll >nul 2>nul
del /Q %OUTDIR%\vc_redist.x64.exe >nul 2>nul

echo Creating ZIP:
echo   "%ZIP_FILE%"

powershell -NoProfile -ExecutionPolicy Bypass -Command "Add-Type -AssemblyName System.IO.Compression.FileSystem; $src=$args[0]; $zip=$args[1]; if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }; [System.IO.Compression.ZipFile]::CreateFromDirectory($src, $zip, [System.IO.Compression.CompressionLevel]::Optimal, $false)" "%OUTDIR%" "%ZIP_FILE%"
if errorlevel 1 (
  echo ERROR: ZIP creation failed.
  exit /b 1
)

if not exist "%ZIP_FILE%" (
  echo ERROR: ZIP file was not created.
  exit /b 1
)

echo.
echo Windows ZIP release created successfully.
echo Folder:
echo   "%OUTDIR%"
echo ZIP:
echo   "%ZIP_FILE%"
echo.
exit /b 0
