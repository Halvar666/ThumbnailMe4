#!/usr/bin/env bash
set -euo pipefail

APPDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DESKTOP_DIR="${XDG_DATA_HOME:-$HOME/.local/share}/applications"
DESKTOP_FILE="$DESKTOP_DIR/thumbnailme4.desktop"
ICON_PATH="$APPDIR/share/icons/hicolor/scalable/apps/thumbnailme4.svg"

mkdir -p "$DESKTOP_DIR"

cat > "$DESKTOP_FILE" <<EOF
[Desktop Entry]
Type=Application
Name=Thumbnail me 4
Comment=Create video thumbnail sheets
Exec=$APPDIR/ThumbnailMe4.sh %F
Icon=$ICON_PATH
Terminal=false
Categories=AudioVideo;Video;Graphics;
MimeType=video/x-matroska;video/mp4;video/x-msvideo;video/x-ms-wmv;video/mpeg;video/webm;video/quicktime;
StartupNotify=true
EOF

chmod 0644 "$DESKTOP_FILE"

if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "$DESKTOP_DIR" >/dev/null 2>&1 || true
fi

echo "Desktop entry installed:"
echo "  $DESKTOP_FILE"
echo
echo "You may need to refresh your application menu or log out/in."
