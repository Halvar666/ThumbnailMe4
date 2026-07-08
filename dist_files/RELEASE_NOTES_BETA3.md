# Thumbnail me 4 Beta 3

Third public beta of Thumbnail me 4.

## Added

- Linux AppImage build.
- Windows portable ZIP build.
- AppImage desktop integration files and Linux icon assets.
- MediaInfo-based header improvements, including compact subtitle language codes.
- WebP output and packaging improvements.

## Changed

- Synced Windows and Linux icon assets.
- Updated Windows release packaging for the 4b2q / Beta 3 build line.
- AppImage-specific files are kept separate from the Windows portable package.

## Fixed

- Fixed Windows source ZIP creation by avoiding PowerShell Compress-Archive timestamp issues.
- Improved WebP saving reliability.
- Fixed platform-specific worker executable naming.
- Improved Windows release checks and package size audit scripts.
- Improved MediaInfo and FFmpeg runtime packaging.

## Notes

- This is a beta release.
- Windows is distributed as a portable ZIP package.
- Linux is distributed as an AppImage.
- The matching source package and Git tag should be published with the binary assets.
