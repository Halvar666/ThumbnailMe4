@echo off
setlocal EnableExtensions
cd /d "%~dp0\.."

set "VERSION=4b2q"
set "FOLDER=%CD%\dist\ThumbnailMe4-%VERSION%-win64"
set "ZIP=%CD%\dist\ThumbnailMe4-%VERSION%-win64.zip"

powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\audit_windows_package_size.ps1" -Folder "%FOLDER%" -Zip "%ZIP%"
exit /b %ERRORLEVEL%
