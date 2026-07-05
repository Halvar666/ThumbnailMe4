@echo off
setlocal EnableExtensions

rem Thumbnail me 4 - source package helper.
rem Creates a clean source ZIP for public release review.
rem Run from the project root or directly from tools.

cd /d "%~dp0\.."

set DIST_ROOT=%CD%\dist
set STAGE_ROOT=%DIST_ROOT%\_sourceziproot
set SOURCE_NAME=ThumbnailMe4-4b1-source
set SOURCE_STAGE=%STAGE_ROOT%\%SOURCE_NAME%
set SOURCE_ZIP=%DIST_ROOT%\%SOURCE_NAME%.zip

if not exist "%DIST_ROOT%" mkdir "%DIST_ROOT%" >nul 2>nul
if exist "%STAGE_ROOT%" rmdir /s /q "%STAGE_ROOT%"
mkdir "%SOURCE_STAGE%" >nul 2>nul

echo Preparing clean source tree:
echo   "%SOURCE_STAGE%"
echo.

for %%I in (
    CMakeLists.txt
    README.md
    LICENSE
    NOTICE.md
    CREDITS.md
    .gitignore
    .gitattributes
) do (
    if exist "%CD%\%%I" copy /Y "%CD%\%%I" "%SOURCE_STAGE%\" >nul
)

for %%D in (
    src
    resources
    tools
    dist_files
) do (
    if exist "%CD%\%%D" xcopy /E /I /Y "%CD%\%%D" "%SOURCE_STAGE%\%%D" >nul
)

powershell -NoProfile -ExecutionPolicy Bypass -Command "$stage='%SOURCE_STAGE%'; Get-ChildItem -LiteralPath $stage -Recurse -File -Force | ForEach-Object { try { Unblock-File -LiteralPath $_.FullName -ErrorAction SilentlyContinue } catch {} }; Get-ChildItem -LiteralPath $stage -Recurse -Force | Where-Object { $_.Name -like '*:Zone.Identifier*' } | Remove-Item -Force -ErrorAction SilentlyContinue"

echo Creating source package:
echo   "%SOURCE_ZIP%"
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command "$stage='%SOURCE_STAGE%'; $zip='%SOURCE_ZIP%'; if(Test-Path $zip){Remove-Item $zip -Force}; Compress-Archive -Path $stage -DestinationPath $zip -Force"

if exist "%SOURCE_ZIP%" (
    echo Source ZIP created:
    echo   "%SOURCE_ZIP%"
    echo.
    echo ZIP top-level check:
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$z='%SOURCE_ZIP%'; Add-Type -AssemblyName System.IO.Compression.FileSystem; $zip=[IO.Compression.ZipFile]::OpenRead($z); try { $zip.Entries | Select-Object -First 12 | ForEach-Object { '  ' + $_.FullName } } finally { $zip.Dispose() }"
) else (
    echo ERROR: source ZIP was not created.
    exit /b 1
)

rmdir /s /q "%STAGE_ROOT%" >nul 2>nul

endlocal
