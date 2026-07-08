Thumbnail me 4 Beta 3 - Windows x64 ZIP package
==========================================

This package is a standalone Windows x64 ZIP build of Thumbnail me 4 Beta 3.

It is not a PortableApps.com / PAF package. It is simply a self-contained
ZIP distribution with the required runtime DLLs and plugins placed next to
the application.

Start:
  ThumbnailMe4.exe

Main files:
  ThumbnailMe4.exe          GUI application
  ThumbnailMeWorker.exe     worker process used for generation
  settings.ini              local application settings
  imageformats/             Qt image format plugins, including qwebp.dll
  licenses/                 license and third-party notices
  README.txt                this file

Notes:
- The application stores settings.ini next to ThumbnailMe4.exe.
- Generated sheets can be written next to the source video or to a selected
  output folder.
- WebP output requires Qt imageformats/qwebp.dll.
- This build uses Qt, FFmpeg and MediaInfo components. See licenses/.
- DirectX shader compiler DLLs and the Visual C++ Redistributable installer
  are not bundled in this ZIP.
- Required Microsoft Visual C++ runtime DLLs are bundled app-locally next to
  ThumbnailMe4.exe, so the app can start on clean Windows installations.
- Source code for this release should be available from the matching Git tag
  or source archive.

Suggested release asset name:
  ThumbnailMe4-Beta3-win64.zip

Suggested GitHub release tag:
  v4b3

Beta 3 notes
------------------------
- This build uses an optimized FFmpeg runtime. It keeps broad input video support while removing unused output, network, device and program components.
- DirectX shader compiler DLLs and the Visual C++ Redistributable installer are not bundled.
- Required Microsoft Visual C++ runtime DLLs are bundled app-locally next to ThumbnailMe4.exe.
- MediaInfo headers include per-stream audio/video bitrates when MediaInfo exposes them.
- The multilingual UI translation set is beta-quality and may be refined later from user/native-speaker feedback.

