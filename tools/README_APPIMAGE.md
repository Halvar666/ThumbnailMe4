# Thumbnail me 4 AppImage build notes

Internal build baseline: **4b2p**

The public Linux package should be an AppImage rather than one tarball per distribution.
The lean tar.xz package remains useful as a fallback and for debugging, but it is not distro-independent.

## Ubuntu build dependencies

On Ubuntu 26.04 LTS:

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
  git \
  pkg-config \
  unzip \
  wget \
  curl \
  file \
  patchelf \
  desktop-file-utils \
  qt6-base-dev \
  qt6-base-dev-tools \
  qt6-tools-dev \
  qt6-tools-dev-tools \
  libqt6svg6-dev \
  qt6-image-formats-plugins \
  qt6-wayland \
  libavformat-dev \
  libavcodec-dev \
  libavutil-dev \
  libswscale-dev \
  libswresample-dev \
  libmediainfo-dev \
  mediainfo
```

FUSE for running AppImages on Ubuntu:

```bash
sudo apt install -y libfuse2t64 || sudo apt install -y libfuse2
```

## Download linuxdeploy tools

```bash
tools/download_appimage_tools.sh
```

This downloads:

- `linuxdeploy-x86_64.AppImage`
- `linuxdeploy-plugin-qt-x86_64.AppImage`

into `~/AppImageTools` by default.

## Build the AppImage

```bash
tools/build_appimage.sh
```

Expected output:

```text
dist/ThumbnailMe4-4b2p-linux-x86_64.AppImage
```

Run:

```bash
./dist/ThumbnailMe4-4b2p-linux-x86_64.AppImage
```

## Notes

- The script builds the application first, then creates an AppDir and lets linuxdeploy bundle the runtime dependencies.
- The AppRun wrapper defaults to `QT_QPA_PLATFORM=xcb` for broad compatibility across KDE, GNOME, XFCE, Cinnamon and MATE through X11/XWayland.
- Native Wayland can be tested with:

```bash
QT_QPA_PLATFORM=wayland ./dist/ThumbnailMe4-4b2p-linux-x86_64.AppImage
```

- Ubuntu 26.04 is acceptable for the first AppImage prototype. For maximum backward compatibility, a later public build may need an older LTS build VM.


## Persistent settings

The AppImage mount directory is temporary, so Linux builds store `settings.ini` in:

```text
~/.config/ThumbnailMe4/settings.ini
```

The Windows ZIP build remains portable and still stores `settings.ini` next to `ThumbnailMe4.exe`.


## 4b2p persistent settings note

The AppImage launcher exports `THUMBNAILME4_CONFIG_DIR` and `THUMBNAILME4_DATA_DIR`.
The application uses these variables before falling back to Qt standard paths.

Expected persistent files:

```text
~/.config/ThumbnailMe4/settings.ini
~/.config/ThumbnailMe4/thumbnailme4.ini
~/.local/share/ThumbnailMe4/
```


## 4b2p notes

This build script always cleans the AppImage build directory before compiling,
then verifies that the built binary contains persistent AppImage settings support.
It also bundles `libmediainfo.so` explicitly because MediaInfo is loaded at runtime
through `QLibrary`, so normal dependency scanners may miss it.

Useful verification commands on a test system:

```bash
./ThumbnailMe4-4b2p-linux-x86_64.AppImage --appimage-extract >/dev/null
find squashfs-root -iname '*mediainfo*'
grep -n 'THUMBNAILME4_CONFIG_DIR' squashfs-root/AppRun
THUMBNAILME4_DEBUG_PATHS=1 ./ThumbnailMe4-4b2p-linux-x86_64.AppImage
```


## 4b2p note

The worker now prepares WebP output more defensively:

- converts the contact sheet to RGB888 before WebP encoding,
- downscales WebP output if either side would exceed 16383 pixels,
- keeps a cross-platform FFmpeg CLI fallback (`ffmpeg` on Linux, `ffmpeg.exe` on Windows),
- verifies that the AppImage contains the Qt WebP image plugin (`libqwebp.so`).


## 4b2p unified build note

4b2p is the synchronization build before beta 3. It keeps the Linux/AppImage
fixes that were validated in 4b2o and carries the generic fixes back into the
shared source files used by Windows builds:

- robust WebP export in `ThumbnailMeWorker`
- compact subtitle language summary
- audio/video bitrate reporting through MediaInfo
- cross-platform worker executable lookup guarded by `Q_OS_WIN`

The AppImage icon now uses PNG files extracted from the same `resources/Thumbnailme.ico`
artwork used by the Windows executable.
