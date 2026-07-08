@echo off
setlocal EnableExtensions
cd /d "%~dp0\.."

set "VERSION=4b2q"
set "ROOT=%CD%"
set "DIST=%ROOT%\dist"
set "OUTDIR=%DIST%\ThumbnailMe4-%VERSION%-win64"
set "ZIP_FILE=%DIST%\ThumbnailMe4-%VERSION%-win64.zip"

if not exist "%OUTDIR%" (
  echo ERROR: Windows release folder is missing:
  echo   "%OUTDIR%"
  exit /b 1
)

if not exist "%ZIP_FILE%" (
  echo ERROR: Windows release ZIP is missing:
  echo   "%ZIP_FILE%"
  exit /b 1
)

set "ERR=0"

if not exist "%OUTDIR%\ThumbnailMe4.exe" echo ERROR: Missing ThumbnailMe4.exe & set "ERR=1"
if not exist "%OUTDIR%\ThumbnailMeWorker.exe" echo ERROR: Missing ThumbnailMeWorker.exe & set "ERR=1"
if not exist "%OUTDIR%\Qt6Core.dll" echo ERROR: Missing Qt6Core.dll & set "ERR=1"
if not exist "%OUTDIR%\Qt6Gui.dll" echo ERROR: Missing Qt6Gui.dll & set "ERR=1"
if not exist "%OUTDIR%\Qt6Widgets.dll" echo ERROR: Missing Qt6Widgets.dll & set "ERR=1"
if not exist "%OUTDIR%\platforms\qwindows.dll" echo ERROR: Missing platforms\qwindows.dll & set "ERR=1"
if not exist "%OUTDIR%\MediaInfo.dll" echo ERROR: Missing MediaInfo.dll & set "ERR=1"
if not exist "%OUTDIR%\licenses\README_LICENSES.txt" echo ERROR: Missing licenses\README_LICENSES.txt & set "ERR=1"

if not exist "%OUTDIR%\avcodec-*.dll" echo ERROR: Missing avcodec FFmpeg DLL & set "ERR=1"
if not exist "%OUTDIR%\avformat-*.dll" echo ERROR: Missing avformat FFmpeg DLL & set "ERR=1"
if not exist "%OUTDIR%\avutil-*.dll" echo ERROR: Missing avutil FFmpeg DLL & set "ERR=1"
if not exist "%OUTDIR%\swscale-*.dll" echo ERROR: Missing swscale FFmpeg DLL & set "ERR=1"

if not "%ERR%"=="0" (
  echo.
  echo Windows ZIP release check failed.
  exit /b 1
)

echo Windows ZIP release check passed.
echo Folder:
echo   "%OUTDIR%"
echo ZIP:
echo   "%ZIP_FILE%"
exit /b 0
