#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -lt 3 ]; then
    echo "Usage: ffmpeg_mtn_style_configure.sh <ffmpeg-src> <build-dir> <install-prefix> [strict]" >&2
    exit 2
fi

win_to_posix() {
    if command -v cygpath >/dev/null 2>&1; then
        cygpath -u "$1"
    else
        printf '%s\n' "$1"
    fi
}

SRC="$(win_to_posix "$1")"
BUILD="$(win_to_posix "$2")"
PREFIX="$(win_to_posix "$3")"
STRICT="${4:-}"

if [ ! -x "$SRC/configure" ]; then
    echo "ERROR: FFmpeg configure script not found: $SRC/configure" >&2
    exit 1
fi

mkdir -p "$BUILD" "$PREFIX"
cd "$BUILD"

CONFIGURE_HELP="$("$SRC/configure" --help 2>/dev/null || true)"

COMMON_FLAGS=()

supports_flag() {
    local flag="$1"
    local key="${flag%%=*}"
    grep -Fq -- "$key" <<<"$CONFIGURE_HELP"
}

add_flag_if_supported() {
    local flag="$1"
    if supports_flag "$flag"; then
        COMMON_FLAGS+=("$flag")
    else
        echo "Note: skipping unsupported FFmpeg configure option: $flag"
    fi
}

# MSVC/Windows build identity.
add_flag_if_supported --toolchain=msvc
add_flag_if_supported --arch=x86_64
add_flag_if_supported --target-os=win64

# Shared DLLs + import libraries for the current TM4 CMake/MSVC workflow.
add_flag_if_supported --enable-shared
add_flag_if_supported --disable-static

# MTN-style size-oriented runtime:
# keep broad demuxer/decoder support, remove output, network, devices,
# external programs and unused processing frameworks.
add_flag_if_supported --enable-small
add_flag_if_supported --disable-doc
add_flag_if_supported --disable-programs
add_flag_if_supported --disable-network
add_flag_if_supported --disable-avdevice
add_flag_if_supported --disable-postproc
add_flag_if_supported --disable-encoders
add_flag_if_supported --disable-muxers
add_flag_if_supported --disable-indevs
add_flag_if_supported --disable-outdevs
add_flag_if_supported --disable-autodetect
add_flag_if_supported --disable-avfilter
add_flag_if_supported --disable-x86asm

# MTN's own helper also disables parsers and bitstream filters.
# For TM4 this is optional because we want broad compatibility with raw
# elementary streams and less-common container/codec combinations.
if [ "$STRICT" = "strict" ]; then
    add_flag_if_supported --disable-parsers
    add_flag_if_supported --disable-bsfs
fi

COMMON_FLAGS+=(--prefix="$PREFIX")

echo "Configuring FFmpeg with TM4 MTN-style options..."
echo "Source:  $SRC"
echo "Build:   $BUILD"
echo "Install: $PREFIX"
echo "Strict:  ${STRICT:-no}"
echo
echo "Enabled configure flags:"
printf '  %q\n' "${COMMON_FLAGS[@]}"
echo

"$SRC/configure" "${COMMON_FLAGS[@]}"

JOBS="${NUMBER_OF_PROCESSORS:-}"
if [ -z "$JOBS" ]; then
    JOBS="$(nproc 2>/dev/null || echo 4)"
fi

echo
echo "Building FFmpeg with $JOBS parallel jobs..."
make -j"$JOBS"

echo
echo "Installing FFmpeg runtime/development files..."
make install

echo
echo "FFmpeg MTN-style runtime installed to: $PREFIX"
