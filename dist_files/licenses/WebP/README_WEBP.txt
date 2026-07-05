WebP / libwebp notices
======================

Thumbnail me 4 can write WebP through Qt Image Formats.

Runtime plugin:
  imageformats/qwebp.dll

Qt documents WebP support in Qt Image Formats as read/write support using
libwebp, version 1.6.0 in Qt 6.11.1 documentation.

License:
  BSD 3-clause "New" or "Revised" License

See:
  ../BSD-3-Clause.txt

Deployment:
- qwebp.dll must remain in the Qt plugin path:
    imageformats/qwebp.dll
- make_windows_zip_release.bat copies imageformats plugins when QT_BIN is set.
- check_windows_zip_release.bat warns if qwebp.dll is missing.
