#!/usr/bin/env bash
set -e

APPDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPBIN="$APPDIR/bin/ThumbnailMe4"
WORKERBIN="$APPDIR/bin/ThumbnailMeWorker"

show_error() {
    local msg="$1"

    echo "$msg" >&2

    if command -v kdialog >/dev/null 2>&1; then
        kdialog --error "$msg" --title "Thumbnail me 4" 2>/dev/null || true
    elif command -v zenity >/dev/null 2>&1; then
        zenity --error --title="Thumbnail me 4" --text="$msg" 2>/dev/null || true
    fi
}

if [ ! -x "$APPBIN" ]; then
    show_error "Thumbnail me 4 cannot start because the main executable is missing or not executable:

$APPBIN"
    exit 1
fi

if [ ! -x "$WORKERBIN" ]; then
    show_error "Thumbnail me 4 cannot start because the worker executable is missing or not executable:

$WORKERBIN"
    exit 1
fi

# The lean Linux tarball intentionally uses system Qt/FFmpeg/MediaInfo.
# Default to XCB/XWayland for the broadest KDE/GNOME/XFCE/Cinnamon/MATE coverage.
# Users can override this, for example:
#   QT_QPA_PLATFORM=wayland ./ThumbnailMe4.sh
export QT_QPA_PLATFORM="${QT_QPA_PLATFORM:-xcb}"

if [ -z "${XDG_CONFIG_HOME:-}" ]; then
    export XDG_CONFIG_HOME="$HOME/.config"
fi

if [ -z "${XDG_DATA_HOME:-}" ]; then
    export XDG_DATA_HOME="$HOME/.local/share"
fi

export THUMBNAILME4_CONFIG_DIR="$XDG_CONFIG_HOME/ThumbnailMe4"
export THUMBNAILME4_DATA_DIR="$XDG_DATA_HOME/ThumbnailMe4"
mkdir -p "$THUMBNAILME4_CONFIG_DIR" "$THUMBNAILME4_DATA_DIR" 2>/dev/null || true

if [ -d "$APPDIR/lib" ]; then
    export LD_LIBRARY_PATH="$APPDIR/lib:${LD_LIBRARY_PATH:-}"
fi

if [ -d "$APPDIR/plugins" ]; then
    export QT_PLUGIN_PATH="$APPDIR/plugins:${QT_PLUGIN_PATH:-}"
fi

if [ -d "$APPDIR/plugins/platforms" ]; then
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/plugins/platforms:${QT_QPA_PLATFORM_PLUGIN_PATH:-}"
fi

if command -v ldd >/dev/null 2>&1; then
    missing="$(ldd "$APPBIN" "$WORKERBIN" 2>/dev/null | awk '/not found/ {print $1}' | sort -u || true)"
    if [ -n "$missing" ]; then
        show_error "Thumbnail me 4 cannot start because these shared libraries are missing:

$missing

Install runtime dependencies.

Manjaro / Arch:
sudo pacman -S --needed qt6-base qt6-imageformats qt6-svg ffmpeg libmediainfo

Debian / Ubuntu / Linux Mint:
sudo apt install qt6-base-dev libqt6svg6 libqt6svg6-plugins libavformat60 libavcodec60 libavutil58 libswscale7 libswresample4 libmediainfo0v5

Fedora:
sudo dnf install qt6-qtbase qt6-qtsvg ffmpeg-libs libmediainfo

openSUSE:
sudo zypper install libQt6Widgets6 libQt6Gui6 libQt6Core6 libQt6Svg6 ffmpeg-6-libavformat ffmpeg-6-libavcodec ffmpeg-6-libavutil ffmpeg-6-libswscale ffmpeg-6-libswresample libmediainfo0"
        exit 1
    fi
fi

exec "$APPBIN" "$@"
