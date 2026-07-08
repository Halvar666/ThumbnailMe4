# Thumbnail me 4b2p sync notes

4b2p is an internal synchronization build before public beta 3.

## Generic AppImage-tested fixes that also belong in Windows builds

- `src/WorkerMain.cpp`: robust WebP save path, RGB888 conversion, 16383-side
  WebP safety limit, and FFmpeg fallback hook. Windows keeps its portable
  behavior; fallback looks for `ffmpeg.exe` beside the worker or in PATH.
- `src/MediaInfoReader.cpp`: audio/video bitrate reporting and compact,
  deduplicated subtitle language codes.
- `src/MainWindow.cpp`: worker executable name is platform-specific
  (`ThumbnailMeWorker.exe` on Windows, `ThumbnailMeWorker` on Linux). Persistent
  AppImage paths are guarded so Windows still stores portable settings next to
  the executable.
- `CMakeLists.txt`: Linux build support without changing the Windows FFmpeg-root
  based build path.

## Packaging-only Linux/AppImage fixes

- AppImage settings exported through `AppRun`.
- MediaInfo runtime bundling and symlink guard.
- Qt WebP plugin verification.
- AppImage desktop icon synchronized with the Windows ICO artwork.

## Next public version

After 4b2p passes Windows ZIP and Linux AppImage smoke tests, rename/package as
public beta 3 (`4b3`).
