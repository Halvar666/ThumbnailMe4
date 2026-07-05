Source availability notes
=========================

For public Thumbnail me 4 binary releases, provide:

1. Thumbnail me 4 source code corresponding exactly to the binary release.
   Recommended:
     - Git tag, e.g. v0.1.0-beta
     - Source archive, e.g. ThumbnailMe4-v0.1.0-beta-source.zip

2. Build instructions:
     - Visual Studio version
     - Qt version
     - FFmpeg binary package/version
     - MediaInfo package/version
     - Build commands

3. FFmpeg source/build information:
     - If redistributing FFmpeg DLLs, provide source or a matching source
       link/build recipe for the exact FFmpeg binaries used.

4. Qt and MediaInfo notices:
     - Keep the license bundle in Windows ZIP packages.

The helper script tools/make_source_package.bat creates a basic project
source zip excluding build/dist folders. Review the output before publishing.
