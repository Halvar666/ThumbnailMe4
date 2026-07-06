# Thumbnail me 4 beta 2 preparation checklist

Release baseline: **4b2**

## Required runtime checks

- Start `ThumbnailMe4.exe` on the development Windows VM.
- Start `ThumbnailMe4.exe` on a clean Windows 11 VM with no Visual Studio installed.
- Confirm no missing `VCRUNTIME*` / `MSVCP*` startup errors.
- Confirm `imageformats/qwebp.dll` is present.
- Confirm `MediaInfo.dll` is present.

## Video compatibility smoke test

Test generation with at least one file from each group:

- MKV H.264
- MKV H.265 / HEVC
- MP4 H.264
- MP4 H.265 / HEVC
- WebM VP9
- WebM AV1
- AVI / XviD
- WMV
- M2TS / TS
- VOB / MPEG
- FLV

## Feature checks

- Start selected / single file.
- Batch generation with several files.
- JPG output.
- PNG output.
- WebP output.
- Preview style.
- MediaInfo header with audio/video stream bitrate when available.
- Configuration load/save.
- Same folder as source.
- Filename suffix.
- Czech UI.
- Several non-Czech UI languages, especially one EU language and one wider-European language.

## Package checks

Run:

```bat
tools\check_windows_zip_release.bat
tools\audit_windows_package_size.bat
```

Expected direction compared to the older full FFmpeg runtime package:

- FFmpeg DLLs around 18-19 MiB total.
- No bundled `dxcompiler.dll`, `dxil.dll`, `d3dcompiler_47.dll`.
- No bundled `vc_redist.x64.exe`.
- App-local MSVC runtime DLLs present.
- ZIP should remain far below the old BtbN-based package size.

## Repository / release

For beta 2 release:

- create source package
- update GitHub repository
- tag `v4b2`
- upload Windows ZIP and source ZIP as release assets
