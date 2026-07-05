FFmpeg notices
==============

Thumbnail me 4 links dynamically to FFmpeg shared libraries.

Current development dependency:
  FFMPEG_ROOT=C:\Dev\deps\ffmpeg
  Intended package type: BtbN FFmpeg GPL shared build

Typical redistributed DLLs:
  avcodec-*.dll
  avformat-*.dll
  avutil-*.dll
  swresample-*.dll
  swscale-*.dll

License notes:
- FFmpeg is LGPL-2.1-or-later by default.
- GPL components may make the FFmpeg binary build GPL-2.0-or-later.
- The BtbN GPL shared package should be treated as GPL-covered.
- Thumbnail me 4 is GPL-2.0-or-later, so this is license-compatible.

Public binary release checklist:
1. Keep FFmpeg DLL names recognizable.
2. Include FFmpeg notices in the licenses folder.
3. Record the exact FFmpeg binary package name, version and source.
4. Provide a matching FFmpeg source offer or source link for the exact
   binaries redistributed.
5. If switching to an LGPL-only build, verify the build really is LGPL-only
   and update this file.
6. Mention FFmpeg in the About box / notices.

Suggested About/website notice:
  This software uses libraries from the FFmpeg project.
