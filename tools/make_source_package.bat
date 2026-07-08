@echo off
setlocal EnableExtensions
cd /d "%~dp0\.."

powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\tools\make_source_package.ps1"
exit /b %ERRORLEVEL%
