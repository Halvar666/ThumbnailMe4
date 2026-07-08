# Thumbnail me 4

Current beta release: **4b3**

Thumbnail me 4 is a modern Qt6 / FFmpeg / MediaInfo continuation and direct GPL-compatible fork of the original Thumbnail me thumbnail sheet generator.

It creates contact sheets from video files and keeps the original Thumbnail me workflow while modernizing the application for current Windows and Linux systems.

## Current status

4b3 / active development.

## Features

- Qt6-based GUI
- FFmpeg frame decoding
- MediaInfo metadata header, including per-stream audio/video bitrates when available
- Worker process for safer batch generation
- JPEG, PNG and WebP output
- Smart frame selection with start/end margins
- Skip dark / blank frames
- Skip similar frames
- Style preview without decoding the whole video
- Broad multilingual UI translation set
- Configuration profiles and `settings.ini`
- Optimized FFmpeg runtime size
- Smaller Windows x64 ZIP release packaging

## Credits

Thumbnail me 4 is a modern continuation and direct GPL-compatible fork of the original **Thumbnail me** project.

Original Thumbnail me project:

- Created by **Rousseau Quentin** / **kwent**
- Original repository: https://github.com/kwent/thumbnailme

Many thanks to Quentin Rousseau and the original Thumbnail me contributors for making the project available as free software.

## Build status

The current development target is Windows with:

- Visual Studio 2026 Developer Command Prompt, x64
- Qt 6.11.1 MSVC 2022 64-bit
- FFmpeg shared libraries
- MediaInfo DLL

## License

Thumbnail me 4 is released under the GNU GPL:

`GPL-2.0-or-later`

See `LICENSE`, `NOTICE.md`, `CREDITS.md` and `dist_files/licenses/` for third-party notices.

