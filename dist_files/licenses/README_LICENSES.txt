Thumbnail me 4 - license bundle
=================================

This folder contains license and attribution notes for Thumbnail me 4
Windows ZIP releases.

This is a practical release checklist, not legal advice.

Summary
-------

Thumbnail me 4 application source:
  License: GPL-2.0-or-later
  Main text: GPL-2.0-or-later.txt

Qt 6 runtime and Qt plugins:
  Typical open-source license path: LGPL-3.0 or GPL terms, depending on
  the module and how it is used.
  Notes: Qt Image Formats is an add-on module. The WebP plugin is deployed
  as imageformats/qwebp.dll when available.
  See: Qt/README_QT.txt

Qt Image Formats WebP / libwebp:
  The WebP codec used by Qt Image Formats is libwebp.
  License: BSD-3-Clause according to Qt's attribution page.
  See: WebP/README_WEBP.txt and BSD-3-Clause.txt

FFmpeg DLLs:
  Current project setup expects BtbN FFmpeg shared binaries.
  If you use a GPL-enabled FFmpeg build, treat the FFmpeg binaries as GPL.
  If you intentionally switch to an LGPL-only FFmpeg build, update these
  notes and keep the exact build/source offer consistent.
  See: FFmpeg/README_FFMPEG.txt

MediaInfo / MediaInfoLib:
  MediaInfo is loaded dynamically if the DLL is present.
  License: BSD-style MediaInfoLib license.
  See: MediaInfo/README_MEDIAINFO.txt

Microsoft Visual C++ runtime:
  If deployment tools copy Microsoft runtime files, those files remain
  under Microsoft redistribution terms. Keep the runtime files unmodified.

Original Thumbnail me material:
  The current icon resources/Thumbnailme.ico is from the original Thumbnail
  me 3.0 package and should stay under the original project/GPL-compatible
  redistribution context.
  See: ThumbnailMe4/README_THUMBNAILME4.txt

Required release practice
-------------------------

For every public binary release:

1. Publish the exact corresponding Thumbnail me 4 source code.
2. Keep LICENSE, NOTICE.md and this licenses folder in the Windows ZIP package.
3. Keep third-party DLL names recognizable. Do not obfuscate FFmpeg DLL names.
4. Keep Qt plugins in their normal plugin subfolders, especially:
     imageformats/qwebp.dll
5. Record the exact third-party binary packages used for the release.
6. If FFmpeg binaries are redistributed, provide or link to the matching
   FFmpeg source/build information for the exact binaries used.
7. If MediaInfo binaries are redistributed, keep the MediaInfo notice.
8. Do not imply that Qt, FFmpeg, MediaInfo, libwebp or Microsoft endorse
   Thumbnail me 4.

Recommended Windows ZIP layout
---------------------------

ThumbnailMe4-4b2-win64/
  ThumbnailMe4.exe
  ThumbnailMeWorker.exe
  LICENSE.txt
  NOTICE.md
  licenses/
    README_LICENSES.txt
    THIRD_PARTY_NOTICES.txt
    GPL-2.0-or-later.txt
    GPL-3.0-or-later.txt
    LGPL-2.1-or-later.txt
    LGPL-3.0-only.txt
    BSD-3-Clause.txt
    Qt/
    FFmpeg/
    MediaInfo/
    WebP/
    ThumbnailMe4/
    Source/
  imageformats/
    qwebp.dll
