Thumbnail me 4 - Linux test package
===================================

This is the Linux package information for Thumbnail me 4b3.

The preferred public Linux package is distributed as an AppImage:

    ./ThumbnailMe4-4b3-linux-x86_64.AppImage

This package is intentionally a lean, system-library package. It does not
bundle Qt, FFmpeg or MediaInfo libraries. That keeps the package small and
avoids copying a large, fragile part of one distribution into another.

Supported desktop goal
----------------------

The package is meant to run on common Linux desktops when the runtime
dependencies are installed:

- KDE Plasma
- GNOME
- XFCE
- Cinnamon
- MATE

The launcher defaults to the Qt XCB backend for broad compatibility across
X11 and XWayland sessions. Native Wayland can be tested manually:

    QT_QPA_PLATFORM=wayland ./ThumbnailMe4-4b3-linux-x86_64.AppImage

Runtime dependencies
--------------------

Manjaro / Arch:

    sudo pacman -S --needed qt6-base qt6-imageformats qt6-svg ffmpeg libmediainfo

Debian / Ubuntu / Linux Mint:

    sudo apt install qt6-base-dev libqt6svg6 libqt6svg6-plugins libavformat60 libavcodec60 libavutil58 libswscale7 libswresample4 libmediainfo0v5

Fedora:

    sudo dnf install qt6-qtbase qt6-qtsvg ffmpeg-libs libmediainfo

openSUSE:

    sudo zypper install libQt6Widgets6 libQt6Gui6 libQt6Core6 libQt6Svg6 ffmpeg-6-libavformat ffmpeg-6-libavcodec ffmpeg-6-libavutil ffmpeg-6-libswscale ffmpeg-6-libswresample libmediainfo0

Optional desktop menu entry
---------------------------

Install menu entry for the current user:

    ./install_desktop_entry.sh

Remove menu entry:

    ./uninstall_desktop_entry.sh

Build dependencies on Manjaro/Arch
----------------------------------

    sudo pacman -S --needed base-devel cmake ninja git pkgconf qt6-base qt6-imageformats qt6-svg ffmpeg libmediainfo mediainfo desktop-file-utils

Build from source
-----------------

    cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build-linux

Create the Linux tarball
------------------------

    chmod +x tools/make_linux_tarball.sh dist_files/ThumbnailMe4.sh
    tools/make_linux_tarball.sh

Package layout
--------------

    ThumbnailMe4.sh
    install_desktop_entry.sh
    uninstall_desktop_entry.sh
    bin/ThumbnailMe4
    bin/ThumbnailMeWorker
    share/icons/hicolor/scalable/apps/thumbnailme4.svg
    README_LINUX.txt
    LICENSE
    NOTICE.md
    CREDITS.md

Notes
-----

- Settings are stored in `~/.config/ThumbnailMe4/settings.ini`.
- Subtitle summaries use compact language codes, for example `eng; deu; jpn`, to keep the thumbnail header readable.
- The public 4b3 Linux build is distributed as an AppImage.

AppImage build
--------------

The preferred public Linux package is an AppImage. See:

    tools/README_APPIMAGE.md
    tools/download_appimage_tools.sh
    tools/build_appimage.sh

The tar.xz package remains available as a small system-library fallback.

AppImage note
-------------

The AppImage mount directory is temporary, so settings are stored outside the AppImage in:

    ~/.config/ThumbnailMe4/settings.ini
