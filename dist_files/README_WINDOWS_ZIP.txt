Thumbnail me 4b1 - Windows x64 ZIP package
==========================================

This package is a standalone Windows x64 ZIP build of Thumbnail me 4b1.

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
- Source code for this release should be available from the matching Git tag
  or source archive.

Suggested release asset name:
  ThumbnailMe4-4b1-win64.zip

Suggested GitHub release tag:
  v4b1
