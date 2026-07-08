#!/usr/bin/env bash
set -euo pipefail

APP_VERSION="${APP_VERSION:-4b2q}"
RELEASE_BASENAME="ThumbnailMe4-${APP_VERSION}-linux-x86_64"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build-linux}"
DIST_ROOT="$ROOT_DIR/dist"
APPDIR="$DIST_ROOT/${RELEASE_BASENAME}.AppDir"
OUT_APPIMAGE="$DIST_ROOT/${RELEASE_BASENAME}.AppImage"
TOOLS_DIR="${APPIMAGE_TOOLS_DIR:-$HOME/AppImageTools}"
LINUXDEPLOY="${LINUXDEPLOY:-$TOOLS_DIR/linuxdeploy-x86_64.AppImage}"
QT_PLUGIN="${LINUXDEPLOY_PLUGIN_QT:-$TOOLS_DIR/linuxdeploy-plugin-qt-x86_64.AppImage}"

if [[ ! -x "$LINUXDEPLOY" || ! -x "$QT_PLUGIN" ]]; then
    echo "ERROR: linuxdeploy tools are missing or not executable."
    echo
    echo "Run first:"
    echo "  tools/download_appimage_tools.sh"
    echo
    echo "Expected:"
    echo "  $LINUXDEPLOY"
    echo "  $QT_PLUGIN"
    exit 1
fi

export PATH="$TOOLS_DIR:$PATH"

if command -v qmake6 >/dev/null 2>&1; then
    export QMAKE="${QMAKE:-$(command -v qmake6)}"
elif [[ -x /usr/lib/qt6/bin/qmake6 ]]; then
    export QMAKE="${QMAKE:-/usr/lib/qt6/bin/qmake6}"
elif command -v qmake >/dev/null 2>&1; then
    export QMAKE="${QMAKE:-$(command -v qmake)}"
else
    echo "ERROR: qmake6 was not found. Install qt6-base-dev-tools."
    exit 1
fi

echo "Using QMAKE=$QMAKE"

echo
echo "Building Thumbnail me 4..."
# Always use a clean AppImage build directory. This avoids stale binaries when
# patching Linux/AppImage-specific source changes such as persistent settings.
rm -rf "$BUILD_DIR"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR"

if [[ ! -x "$BUILD_DIR/ThumbnailMe4" || ! -x "$BUILD_DIR/ThumbnailMeWorker" ]]; then
    echo "ERROR: build output is missing."
    exit 1
fi

echo
echo "Persistent settings source check:"
if grep -q 'THUMBNAILME4_CONFIG_DIR' "$ROOT_DIR/src/MainWindow.cpp" && grep -q 'persistentConfigDirectory' "$ROOT_DIR/src/MainWindow.cpp"; then
    echo "OK: src/MainWindow.cpp contains persistent AppImage settings support"
else
    echo "ERROR: src/MainWindow.cpp does not contain persistent AppImage settings support."
    echo "Apply the latest AppImage patch again."
    exit 1
fi

if command -v strings >/dev/null 2>&1; then
    if strings "$BUILD_DIR/ThumbnailMe4" | grep -q 'THUMBNAILME4_CONFIG_DIR'; then
        echo "OK: built binary contains THUMBNAILME4_CONFIG_DIR marker"
    else
        echo "WARNING: built binary marker was not visible through strings; continuing because source check passed."
        echo "         Runtime path verification is still done through AppRun and post-build extraction."
    fi
fi


echo
echo "WebP worker source check:"
if grep -q 'prepareImageForWebP' "$ROOT_DIR/src/WorkerMain.cpp" && grep -q 'maxWebpSide' "$ROOT_DIR/src/WorkerMain.cpp"; then
    echo "OK: src/WorkerMain.cpp contains robust WebP export safeguards"
else
    echo "ERROR: src/WorkerMain.cpp does not contain robust WebP export safeguards."
    echo "Apply the latest WebP/AppImage patch again."
    exit 1
fi

echo
echo "Preparing AppDir..."
rm -rf "$APPDIR" "$OUT_APPIMAGE"
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/share/applications"
mkdir -p "$APPDIR/usr/share/icons/hicolor/16x16/apps"
mkdir -p "$APPDIR/usr/share/icons/hicolor/24x24/apps"
mkdir -p "$APPDIR/usr/share/icons/hicolor/32x32/apps"
mkdir -p "$APPDIR/usr/share/icons/hicolor/48x48/apps"
mkdir -p "$APPDIR/usr/share/icons/hicolor/128x128/apps"
mkdir -p "$APPDIR/usr/share/icons/hicolor/scalable/apps"
mkdir -p "$APPDIR/usr/share/pixmaps"
mkdir -p "$APPDIR/usr/share/doc/thumbnailme4"
mkdir -p "$APPDIR/usr/share/thumbnailme4"

install -m 0755 "$BUILD_DIR/ThumbnailMe4" "$APPDIR/usr/bin/ThumbnailMe4"
install -m 0755 "$BUILD_DIR/ThumbnailMeWorker" "$APPDIR/usr/bin/ThumbnailMeWorker"
install -m 0755 "$ROOT_DIR/dist_files/linux/AppRun" "$APPDIR/AppRun"
install -m 0644 "$ROOT_DIR/dist_files/linux/thumbnailme4.desktop" "$APPDIR/usr/share/applications/thumbnailme4.desktop"

# Use the same icon artwork as the Windows build. The PNG files are extracted
# from resources/Thumbnailme.ico, so the Linux desktop/AppImage icon matches the
# Windows executable icon instead of using the temporary SVG placeholder.
install -m 0644 "$ROOT_DIR/dist_files/linux/icons/thumbnailme4_16.png" "$APPDIR/usr/share/icons/hicolor/16x16/apps/thumbnailme4.png"
install -m 0644 "$ROOT_DIR/dist_files/linux/icons/thumbnailme4_24.png" "$APPDIR/usr/share/icons/hicolor/24x24/apps/thumbnailme4.png"
install -m 0644 "$ROOT_DIR/dist_files/linux/icons/thumbnailme4_32.png" "$APPDIR/usr/share/icons/hicolor/32x32/apps/thumbnailme4.png"
install -m 0644 "$ROOT_DIR/dist_files/linux/icons/thumbnailme4_48.png" "$APPDIR/usr/share/icons/hicolor/48x48/apps/thumbnailme4.png"
install -m 0644 "$ROOT_DIR/dist_files/linux/icons/thumbnailme4_128.png" "$APPDIR/usr/share/icons/hicolor/128x128/apps/thumbnailme4.png"
install -m 0644 "$ROOT_DIR/dist_files/linux/thumbnailme4.png" "$APPDIR/usr/share/pixmaps/thumbnailme4.png"
if [[ -f "$ROOT_DIR/dist_files/linux/thumbnailme4.svg" ]]; then
    install -m 0644 "$ROOT_DIR/dist_files/linux/thumbnailme4.svg" "$APPDIR/usr/share/icons/hicolor/scalable/apps/thumbnailme4.svg"
fi

# Root copies help AppImage tools and desktop integrations discover metadata.
cp "$APPDIR/usr/share/applications/thumbnailme4.desktop" "$APPDIR/thumbnailme4.desktop"
cp "$APPDIR/usr/share/icons/hicolor/128x128/apps/thumbnailme4.png" "$APPDIR/thumbnailme4.png"

for f in README.md LICENSE NOTICE.md CREDITS.md; do
    if [[ -f "$ROOT_DIR/$f" ]]; then
        install -m 0644 "$ROOT_DIR/$f" "$APPDIR/usr/share/doc/thumbnailme4/$f"
    fi
done

if [[ -f "$ROOT_DIR/dist_files/README_LINUX.txt" ]]; then
    install -m 0644 "$ROOT_DIR/dist_files/README_LINUX.txt" "$APPDIR/usr/share/doc/thumbnailme4/README_LINUX.txt"
fi

if [[ -d "$ROOT_DIR/dist_files/licenses" ]]; then
    mkdir -p "$APPDIR/usr/share/doc/thumbnailme4/licenses"
    cp -a "$ROOT_DIR/dist_files/licenses/." "$APPDIR/usr/share/doc/thumbnailme4/licenses/"
fi

bundle_library_file() {
    local src="$1"
    local dest_dir="$APPDIR/usr/lib"
    local base

    if [[ -z "$src" || ! -f "$src" ]]; then
        return 1
    fi

    case "$src" in
        /lib*/ld-linux-*|/usr/lib*/ld-linux-*|*/libc.so.*|*/libm.so.*|*/libdl.so.*|*/librt.so.*|*/libpthread.so.*|*/libresolv.so.*|*/libnsl.so.*|*/libutil.so.*)
            return 0
            ;;
    esac

    mkdir -p "$dest_dir"
    base="$(basename "$src")"
    if [[ ! -e "$dest_dir/$base" ]]; then
        cp -L "$src" "$dest_dir/$base"
        echo "bundled: $base"
    fi
}

bundle_direct_dependencies() {
    local elf="$1"
    if ! command -v ldd >/dev/null 2>&1; then
        return 0
    fi

    ldd "$elf" 2>/dev/null | awk '
        /=> \/[^ ]+/ { print $3 }
        /^\t\/[^ ]+/ { print $1 }
    ' | while read -r dep; do
        bundle_library_file "$dep" || true
    done
}

bundle_mediainfo_runtime() {
    local lib=""

    if command -v ldconfig >/dev/null 2>&1; then
        lib="$(ldconfig -p 2>/dev/null | awk '/libmediainfo\.so\.0 / { print $NF; exit }')"
        if [[ -z "$lib" ]]; then
            lib="$(ldconfig -p 2>/dev/null | awk '/libmediainfo\.so / { print $NF; exit }')"
        fi
    fi

    if [[ -z "$lib" ]]; then
        for candidate in \
            /usr/lib/x86_64-linux-gnu/libmediainfo.so.0 \
            /usr/lib/x86_64-linux-gnu/libmediainfo.so \
            /usr/lib/libmediainfo.so.0 \
            /usr/lib/libmediainfo.so; do
            if [[ -f "$candidate" ]]; then
                lib="$candidate"
                break
            fi
        done
    fi

    if [[ -z "$lib" || ! -f "$lib" ]]; then
        echo "ERROR: libmediainfo.so was not found. Install libmediainfo-dev."
        exit 1
    fi

    echo
    echo "Bundling MediaInfo runtime: $lib"
    mkdir -p "$APPDIR/usr/lib"
    bundle_library_file "$lib"
    bundle_direct_dependencies "$lib"

    # Resolve one extra level for direct MediaInfo dependencies such as libzen.
    for copied in "$APPDIR"/usr/lib/*.so*; do
        [[ -f "$copied" ]] || continue
        bundle_direct_dependencies "$copied"
    done

    # Avoid creating libmediainfo.so -> libmediainfo.so self-referential
    # symlinks. linuxdeploy follows these links during dependency scanning and
    # aborts with "Too many levels of symbolic links" if they are wrong.
    #
    # The application tries libmediainfo.so.0 explicitly, so the unversioned
    # development symlink is not needed in the AppImage.
    local real_lib real_base versioned_target
    real_lib="$(readlink -f "$lib")"
    real_base="$(basename "$real_lib")"
    versioned_target="$APPDIR/usr/lib/libmediainfo.so.0"

    rm -f "$APPDIR/usr/lib/libmediainfo.so"

    if [[ "$real_base" == "libmediainfo.so.0" ]]; then
        cp -Lf "$real_lib" "$versioned_target"
    elif [[ -f "$APPDIR/usr/lib/$real_base" ]]; then
        rm -f "$versioned_target"
        ln -s "$real_base" "$versioned_target"
    elif [[ -f "$versioned_target" ]]; then
        :
    else
        cp -Lf "$real_lib" "$versioned_target"
    fi

    echo "MediaInfo files in AppDir:"
    find "$APPDIR/usr/lib" -maxdepth 1 -iname '*mediainfo*' -printf '  %f\n' | sort
}

bundle_mediainfo_runtime

if command -v strip >/dev/null 2>&1; then
    strip --strip-unneeded "$APPDIR/usr/bin/ThumbnailMe4" 2>/dev/null || true
    strip --strip-unneeded "$APPDIR/usr/bin/ThumbnailMeWorker" 2>/dev/null || true
fi

# Ask the Qt plugin to include the image format plugins we need for output/preview.
export EXTRA_QT_PLUGINS="${EXTRA_QT_PLUGINS:-imageformats/libqjpeg.so;imageformats/libqwebp.so;imageformats/libqsvg.so;imageformats/libqico.so}"

# Keep linuxdeploy/appimagetool from deleting useful diagnostic output.
export NO_STRIP=1

echo
echo "Pre-linuxdeploy AppDir checks:"
if grep -q 'THUMBNAILME4_CONFIG_DIR' "$APPDIR/AppRun"; then
    echo "OK: AppRun exports THUMBNAILME4_CONFIG_DIR"
else
    echo "ERROR: AppRun does not export THUMBNAILME4_CONFIG_DIR"
    exit 1
fi
if find "$APPDIR/usr/lib" -maxdepth 1 -iname '*mediainfo*' | grep -q .; then
    echo "OK: MediaInfo runtime is bundled"
else
    echo "ERROR: MediaInfo runtime is not bundled"
    exit 1
fi

if [[ -L "$APPDIR/usr/lib/libmediainfo.so" ]]; then
    target="$(readlink "$APPDIR/usr/lib/libmediainfo.so")"
    if [[ "$target" == "libmediainfo.so" ]]; then
        echo "ERROR: self-referential libmediainfo.so symlink detected."
        exit 1
    fi
fi

if [[ -e "$APPDIR/usr/lib/libmediainfo.so.0" ]]; then
    if ! readlink -f "$APPDIR/usr/lib/libmediainfo.so.0" >/dev/null 2>&1; then
        echo "ERROR: broken libmediainfo.so.0 link detected."
        exit 1
    fi
else
    echo "ERROR: libmediainfo.so.0 is missing from AppDir."
    exit 1
fi

if [[ -f "$APPDIR/thumbnailme4.png" && -f "$APPDIR/usr/share/icons/hicolor/128x128/apps/thumbnailme4.png" ]]; then
    echo "OK: Windows-matched PNG icon is bundled"
else
    echo "ERROR: AppImage PNG icon files are missing"
    exit 1
fi

echo
echo "Running linuxdeploy..."
pushd "$DIST_ROOT" >/dev/null
"$LINUXDEPLOY" \
    --appdir "$APPDIR" \
    --executable "$APPDIR/usr/bin/ThumbnailMe4" \
    --executable "$APPDIR/usr/bin/ThumbnailMeWorker" \
    --desktop-file "$APPDIR/usr/share/applications/thumbnailme4.desktop" \
    --icon-file "$APPDIR/usr/share/icons/hicolor/128x128/apps/thumbnailme4.png" \
    --plugin qt \
    --output appimage
popd >/dev/null

created="$(find "$DIST_ROOT" -maxdepth 1 -type f -name '*.AppImage' -printf '%T@ %p\n' | sort -n | tail -1 | cut -d' ' -f2- || true)"
if [[ -z "$created" || ! -f "$created" ]]; then
    echo "ERROR: AppImage was not created."
    exit 1
fi

if [[ "$created" != "$OUT_APPIMAGE" ]]; then
    mv -f "$created" "$OUT_APPIMAGE"
fi
chmod +x "$OUT_APPIMAGE"

echo
echo "AppImage created:"
ls -lh "$OUT_APPIMAGE"

echo
echo "Quick file check:"
file "$OUT_APPIMAGE" || true

if [[ "${SKIP_APPIMAGE_VERIFY:-0}" != "1" ]]; then
    echo
    echo "Verifying AppImage contents..."
    VERIFY_DIR="$DIST_ROOT/verify-${RELEASE_BASENAME}"
    rm -rf "$VERIFY_DIR"
    mkdir -p "$VERIFY_DIR"
    pushd "$VERIFY_DIR" >/dev/null
    "$OUT_APPIMAGE" --appimage-extract >/dev/null
    if grep -q 'THUMBNAILME4_CONFIG_DIR' squashfs-root/AppRun; then
        echo "OK: extracted AppRun contains THUMBNAILME4_CONFIG_DIR"
    else
        echo "ERROR: extracted AppRun does not contain THUMBNAILME4_CONFIG_DIR"
        exit 1
    fi
    if find squashfs-root -iname '*mediainfo*' | grep -q 'libmediainfo'; then
        echo "OK: extracted AppImage contains MediaInfo runtime"
    else
        echo "ERROR: extracted AppImage does not contain MediaInfo runtime"
        exit 1
    fi
    if find squashfs-root -path '*imageformats*' -iname 'libqwebp.so' | grep -q .; then
        echo "OK: extracted AppImage contains Qt WebP image plugin"
    else
        echo "ERROR: extracted AppImage does not contain Qt WebP image plugin"
        echo "Install qt6-image-formats-plugins in the build VM and rebuild."
        exit 1
    fi
    if [[ -f squashfs-root/thumbnailme4.png ]] && [[ -f squashfs-root/usr/share/icons/hicolor/128x128/apps/thumbnailme4.png ]]; then
        echo "OK: extracted AppImage contains Windows-matched PNG icon"
    else
        echo "ERROR: extracted AppImage does not contain Windows-matched PNG icon"
        exit 1
    fi
    popd >/dev/null
    rm -rf "$VERIFY_DIR"
fi

echo
echo "Run:"
echo "  $OUT_APPIMAGE"
