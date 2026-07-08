#!/usr/bin/env bash
set -euo pipefail

TOOLS_DIR="${APPIMAGE_TOOLS_DIR:-$HOME/AppImageTools}"
mkdir -p "$TOOLS_DIR"
cd "$TOOLS_DIR"

download() {
    local url="$1"
    local out="$2"
    if [[ -x "$out" ]]; then
        echo "OK: $TOOLS_DIR/$out already exists"
        return
    fi
    echo "Downloading $out..."
    if command -v wget >/dev/null 2>&1; then
        wget -O "$out" "$url"
    elif command -v curl >/dev/null 2>&1; then
        curl -L "$url" -o "$out"
    else
        echo "ERROR: wget or curl is required." >&2
        exit 1
    fi
    chmod +x "$out"
}

download \
    "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage" \
    "linuxdeploy-x86_64.AppImage"

download \
    "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage" \
    "linuxdeploy-plugin-qt-x86_64.AppImage"

echo
echo "AppImage tools are ready in: $TOOLS_DIR"
ls -lh "$TOOLS_DIR"/*.AppImage
