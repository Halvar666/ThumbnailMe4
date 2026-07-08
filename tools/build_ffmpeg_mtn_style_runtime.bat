@echo off
setlocal EnableExtensions

rem Thumbnail me 4b2p - optimized FFmpeg runtime builder.
rem Run this from a Visual Studio Developer Command Prompt.
rem Requires MSYS2 bash, git, and Visual Studio build tools in PATH.

rem If MSYS2 is installed in C:\msys64, bash.exe is detected automatically.
rem Output default: C:\Dev\deps\ffmpeg-tm4-mtn-style

cd /d "%~dp0\.."

if "%FFMPEG_SRC%"=="" set FFMPEG_SRC=C:\Dev\src\ffmpeg
if "%FFMPEG_BUILD%"=="" set FFMPEG_BUILD=C:\Dev\build\ffmpeg-tm4-mtn-style
if "%FFMPEG_INSTALL%"=="" set FFMPEG_INSTALL=C:\Dev\deps\ffmpeg-tm4-mtn-style
if "%FFMPEG_GIT_URL%"=="" set FFMPEG_GIT_URL=https://git.ffmpeg.org/ffmpeg.git
if "%FFMPEG_BRANCH%"=="" set FFMPEG_BRANCH=master

where cl.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: cl.exe was not found.
    echo Run this from a Visual Studio Developer Command Prompt.
    exit /b 1
)

set BASH_EXE=
where bash.exe >nul 2>nul
if not errorlevel 1 (
    for /f "delims=" %%B in ('where bash.exe') do (
        if not defined BASH_EXE set BASH_EXE=%%B
    )
)
if not defined BASH_EXE if exist "C:\msys64\usr\bin\bash.exe" set BASH_EXE=C:\msys64\usr\bin\bash.exe
if not defined BASH_EXE (
    echo ERROR: MSYS2 bash.exe was not found.
    echo Install MSYS2 or add C:\msys64\usr\bin to PATH.
    exit /b 1
)

where git.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: git.exe was not found in PATH.
    exit /b 1
)

echo.
echo TM4 experimental MTN-style FFmpeg runtime
echo ============================================
echo Source:  %FFMPEG_SRC%
echo Build:   %FFMPEG_BUILD%
echo Install: %FFMPEG_INSTALL%
echo Bash:    %BASH_EXE%
echo.

if not exist "%FFMPEG_SRC%\.git" (
    echo Cloning FFmpeg source...
    mkdir "C:\Dev\src" >nul 2>nul
    git clone --depth 1 --branch "%FFMPEG_BRANCH%" "%FFMPEG_GIT_URL%" "%FFMPEG_SRC%"
    if errorlevel 1 exit /b 1
) else (
    echo Updating existing FFmpeg source...
    git -C "%FFMPEG_SRC%" fetch --depth 1 origin "%FFMPEG_BRANCH%"
    if errorlevel 1 exit /b 1
    git -C "%FFMPEG_SRC%" checkout "%FFMPEG_BRANCH%"
    if errorlevel 1 exit /b 1
    git -C "%FFMPEG_SRC%" pull --ff-only
    if errorlevel 1 exit /b 1
)

if exist "%FFMPEG_BUILD%" (
    echo Removing previous FFmpeg build folder...
    rmdir /s /q "%FFMPEG_BUILD%"
)
if exist "%FFMPEG_INSTALL%" (
    echo Removing previous FFmpeg install folder...
    rmdir /s /q "%FFMPEG_INSTALL%"
)
mkdir "%FFMPEG_BUILD%" >nul 2>nul
mkdir "%FFMPEG_INSTALL%" >nul 2>nul

set STRICT_ARG=
if /I "%TM4_FFMPEG_STRICT_MTN%"=="1" set STRICT_ARG=strict

echo Starting configure/build/install...
"%BASH_EXE%" "%CD%\tools\ffmpeg_mtn_style_configure.sh" "%FFMPEG_SRC%" "%FFMPEG_BUILD%" "%FFMPEG_INSTALL%" %STRICT_ARG%
if errorlevel 1 (
    echo.
    echo ERROR: FFmpeg MTN-style runtime build failed.
    exit /b 1
)

echo.
echo Ensuring MSVC import libraries are available in lib...
for %%L in (avcodec avformat avutil swresample swscale) do (
    if not exist "%FFMPEG_INSTALL%\lib\%%L.lib" (
        if exist "%FFMPEG_INSTALL%\bin\%%L.lib" (
            copy /Y "%FFMPEG_INSTALL%\bin\%%L.lib" "%FFMPEG_INSTALL%\lib\%%L.lib" >nul
            echo Copied %%L.lib from bin to lib.
        )
    )
)

echo.
echo Checking generated FFmpeg runtime...
call "%CD%\tools\check_ffmpeg_runtime_root.bat" "%FFMPEG_INSTALL%"
if errorlevel 1 exit /b 1

echo.
echo Done.
echo To build TM4 with this runtime, use:
echo   set FFMPEG_ROOT=%FFMPEG_INSTALL%
echo   set PATH=%FFMPEG_INSTALL%\bin;%%PATH%%
endlocal
