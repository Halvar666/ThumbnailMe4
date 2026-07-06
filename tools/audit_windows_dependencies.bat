@echo off
setlocal

set RELEASE_BASENAME=ThumbnailMe4-4b2-win64
set DIST_DIR=%CD%\dist\%RELEASE_BASENAME%

if not exist "%DIST_DIR%\ThumbnailMe4.exe" (
    echo ERROR: Release folder does not exist or ThumbnailMe4.exe is missing:
    echo   "%DIST_DIR%"
    exit /b 1
)

where dumpbin >nul 2>nul
if errorlevel 1 (
    echo ERROR: dumpbin was not found. Run from a Visual Studio Developer Command Prompt.
    exit /b 1
)

echo Dependency audit
echo ================
echo Folder: %DIST_DIR%
echo.

pushd "%DIST_DIR%" >nul

echo ThumbnailMe4.exe
dumpbin /DEPENDENTS ThumbnailMe4.exe

echo.
echo ThumbnailMeWorker.exe
dumpbin /DEPENDENTS ThumbnailMeWorker.exe

echo.
echo Qt6Gui.dll
dumpbin /DEPENDENTS Qt6Gui.dll

echo.
echo platforms\qwindows.dll
dumpbin /DEPENDENTS platforms\qwindows.dll

echo.
echo avcodec DLL
for %%F in (avcodec-*.dll) do dumpbin /DEPENDENTS "%%F"

popd >nul
endlocal
