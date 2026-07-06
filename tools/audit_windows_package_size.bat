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

powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\audit_windows_package_size.ps1" -DistDir "%DIST_DIR%" -Top 60
endlocal
