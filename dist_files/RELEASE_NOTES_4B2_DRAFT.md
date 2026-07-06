# Thumbnail me 4b2

Second public beta of Thumbnail me 4, a modern Qt6 / FFmpeg / MediaInfo continuation and direct GPL-compatible fork of the original Thumbnail me thumbnail sheet generator.

## Added

- Broad multilingual UI translation set based on the original Thumbnail me 3 language support and expanded with additional European languages.
- Translation audit helper for detecting accidentally untranslated UI strings.
- MediaInfo stream bitrate display for audio and video streams when available.
- App-local Microsoft Visual C++ runtime DLL deployment for clean Windows systems.

## Changed

- Optimized FFmpeg runtime size while preserving broad input video support.
- Reduced Windows ZIP package size significantly.
- Removed unused Windows/DirectX compiler components from the release package.
- Removed bundled Visual C++ Redistributable installer and replaced it with app-local runtime DLLs.
- Release scripts now audit package size and runtime dependencies.
- Release scripts now copy only required FFmpeg runtime dependencies.
- Qt image format deployment is trimmed to required formats.

## Notes

- Translations are beta-quality and may be refined in future releases based on user feedback and native-speaker corrections.
- The optimized FFmpeg runtime was tested with common H.264/H.265 MKV/MP4, WebM VP9/AV1, AVI/XviD, WMV, M2TS/TS, VOB/MPEG and FLV files.
- Windows remains the only packaged/tested platform for this beta.
- This is a Windows x64 ZIP package, not a PortableApps.com / PAF package.
