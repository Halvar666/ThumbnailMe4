#!/usr/bin/env bash
set -euo pipefail

APP_VERSION="${APP_VERSION:-4b2p}"
RELEASE_BASENAME="ThumbnailMe4-${APP_VERSION}-linux-x86_64"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build-linux}"
DIST_ROOT="$ROOT_DIR/dist"
DIST_DIR="$DIST_ROOT/$RELEASE_BASENAME"

if [[ ! -x "$BUILD_DIR/ThumbnailMe4" ]]; then
    echo "ERROR: $BUILD_DIR/ThumbnailMe4 not found or not executable."
    echo "Build first:"
    echo "  cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build build-linux"
    exit 1
fi

if [[ ! -x "$BUILD_DIR/ThumbnailMeWorker" ]]; then
    echo "ERROR: $BUILD_DIR/ThumbnailMeWorker not found or not executable."
    exit 1
fi

rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR/bin" "$DIST_DIR/output" "$DIST_DIR/logs"
mkdir -p "$DIST_DIR/share/icons/hicolor/scalable/apps"

install -m 0755 "$BUILD_DIR/ThumbnailMe4" "$DIST_DIR/bin/ThumbnailMe4"
install -m 0755 "$BUILD_DIR/ThumbnailMeWorker" "$DIST_DIR/bin/ThumbnailMeWorker"

if command -v strip >/dev/null 2>&1; then
    strip --strip-unneeded "$DIST_DIR/bin/ThumbnailMe4" 2>/dev/null || true
    strip --strip-unneeded "$DIST_DIR/bin/ThumbnailMeWorker" 2>/dev/null || true
fi

install -m 0755 "$ROOT_DIR/dist_files/ThumbnailMe4.sh" "$DIST_DIR/ThumbnailMe4.sh"
install -m 0755 "$ROOT_DIR/dist_files/linux/install_desktop_entry.sh" "$DIST_DIR/install_desktop_entry.sh"
install -m 0755 "$ROOT_DIR/dist_files/linux/uninstall_desktop_entry.sh" "$DIST_DIR/uninstall_desktop_entry.sh"
install -m 0644 "$ROOT_DIR/dist_files/linux/thumbnailme4.svg" "$DIST_DIR/share/icons/hicolor/scalable/apps/thumbnailme4.svg"

for f in README.md LICENSE NOTICE.md CREDITS.md; do
    if [[ -f "$ROOT_DIR/$f" ]]; then
        install -m 0644 "$ROOT_DIR/$f" "$DIST_DIR/$f"
    fi
done

install -m 0644 "$ROOT_DIR/dist_files/README_LINUX.txt" "$DIST_DIR/README_LINUX.txt"

if [[ -f "$ROOT_DIR/dist_files/settings.ini.example" ]]; then
    install -m 0644 "$ROOT_DIR/dist_files/settings.ini.example" "$DIST_DIR/settings.ini"
fi

mkdir -p "$DIST_DIR/licenses"
if [[ -d "$ROOT_DIR/dist_files/licenses" ]]; then
    cp -a "$ROOT_DIR/dist_files/licenses/." "$DIST_DIR/licenses/"
fi

echo
echo "Runtime dependency check on this system:"
if command -v ldd >/dev/null 2>&1; then
    missing="$(ldd "$DIST_DIR/bin/ThumbnailMe4" "$DIST_DIR/bin/ThumbnailMeWorker" 2>/dev/null | awk '/not found/ {print $1}' | sort -u || true)"
    if [[ -n "$missing" ]]; then
        echo "WARNING: missing shared libraries:"
        echo "$missing"
        echo
        echo "Install the runtime dependencies listed in README_LINUX.txt."
    else
        echo "OK: no missing shared libraries reported by ldd"
    fi
else
    echo "SKIPPED: ldd not found"
fi

echo
echo "Package size:"
du -sh "$DIST_DIR" || true

mkdir -p "$DIST_ROOT"
tar -C "$DIST_ROOT" -cJf "$DIST_ROOT/$RELEASE_BASENAME.tar.xz" "$RELEASE_BASENAME"

echo
echo "Tarball size:"
ls -lh "$DIST_ROOT/$RELEASE_BASENAME.tar.xz" || true

echo
echo "Linux test package created:"
echo "  $DIST_DIR"
echo
echo "Tarball created:"
echo "  $DIST_ROOT/$RELEASE_BASENAME.tar.xz"
echo
echo "Run:"
echo "  $DIST_DIR/ThumbnailMe4.sh"
