# ThumbnailMe4 4b2q Windows release/source packaging fix

This patch is a 4b2q packaging revision for the existing 4b2p codebase.
It updates the Windows packaging scripts so the expected Windows outputs are:

- `dist\ThumbnailMe4-4b2q-win64\`
- `dist\ThumbnailMe4-4b2q-win64.zip`
- `dist\ThumbnailMe4-4b2q-source.zip`

Use from the Windows repository root:

```bat
cd /d C:\Dev\thumbnailme
tar -xf C:\Users\bosah\Downloads\thumbnailme_4b2q_unified_icon_windows_sync_files_fix.zip
rmdir /s /q build
rmdir /s /q dist
set PATH=C:\Dev\deps\ffmpeg-tm4-mtn-style\bin;C:\Qt\6.11.1\msvc2022_64\bin;%PATH%
set FFMPEG_ROOT=C:\Dev\deps\ffmpeg-tm4-mtn-style
set QT_BIN=C:\Qt\6.11.1\msvc2022_64\bin
set MEDIAINFO_ROOT=C:\Dev\deps\mediainfo
cmake -S . -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DQt6_DIR=C:\Qt\6.11.1\msvc2022_64\lib\cmake\Qt6
cmake --build build
tools\make_windows_zip_release.bat
tools\check_windows_zip_release.bat
tools\audit_windows_package_size.bat
tools\make_source_package.bat
```

The PowerShell source ZIP logic is separated into `make_source_package.ps1`, so the old `Compress-Archive` timestamp crash and broken `^` line continuation issue are avoided.
