#!/usr/bin/env bash
set -euo pipefail

DESKTOP_DIR="${XDG_DATA_HOME:-$HOME/.local/share}/applications"
DESKTOP_FILE="$DESKTOP_DIR/thumbnailme4.desktop"

if [ -f "$DESKTOP_FILE" ]; then
    rm -f "$DESKTOP_FILE"
    echo "Desktop entry removed:"
    echo "  $DESKTOP_FILE"
else
    echo "Desktop entry was not installed:"
    echo "  $DESKTOP_FILE"
fi

if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "$DESKTOP_DIR" >/dev/null 2>&1 || true
fi
