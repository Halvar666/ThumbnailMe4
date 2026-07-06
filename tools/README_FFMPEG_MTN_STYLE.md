# Thumbnail me 4 - experimental MTN-style FFmpeg runtime

This is an internal helper for building the optimized FFmpeg runtime used by Thumbnail me 4b2.

The goal is to reduce the Windows ZIP size without going back to `mtn.exe`.
Thumbnail me 4 still decodes frames itself through FFmpeg libraries, but this
runtime uses the same general philosophy as Movie Thumbnailer (MTN): keep broad
input decoding support and remove output/streaming/tooling features that a
thumbnail generator does not need.

## Compared to the stock BtbN GPL shared FFmpeg build

The experimental configure script disables:

- command-line programs (`ffmpeg.exe`, `ffprobe.exe`, `ffplay.exe`)
- encoders
- muxers
- network support
- input/output devices
- postproc
- avfilter
- autodetected external libraries
- x86 assembly, to avoid a NASM dependency in the first experiment

It does **not** disable decoders or demuxers. This keeps broad input file support.
It also keeps parsers and bitstream filters by default because TM4 supports raw
H.264/H.265/AV1 style files and should remain tolerant of unusual inputs.

For a stricter MTN-like experiment, set this before running the builder:

```bat
set TM4_FFMPEG_STRICT_MTN=1
```

That additionally disables parsers and bitstream filters. Use this only for
experiments; it may reduce compatibility.

## Build

Run from a Visual Studio Developer Command Prompt:

```bat
cd /d C:\Dev\thumbnailme
tools\build_ffmpeg_mtn_style_runtime.bat
```

Default output:

```text
C:\Dev\deps\ffmpeg-tm4-mtn-style
```

Then build TM4 with:

```bat
set FFMPEG_ROOT=C:\Dev\deps\ffmpeg-tm4-mtn-style
set PATH=C:\Dev\deps\ffmpeg-tm4-mtn-style\bin;C:\Qt\6.11.1\msvc2022_64\bin;%PATH%
```

## Fallback

If the custom FFmpeg build fails or compatibility is poor, use the previous BtbN
FFmpeg root again:

```bat
set FFMPEG_ROOT=C:\Dev\deps\ffmpeg
set PATH=C:\Dev\deps\ffmpeg\bin;C:\Qt\6.11.1\msvc2022_64\bin;%PATH%
```


## Import library note

The helper now copies MSVC import libraries from `bin` to `lib` after FFmpeg install, because recent FFmpeg/MSVC builds may place `avcodec.lib`, `avformat.lib`, `avutil.lib`, `swresample.lib`, and `swscale.lib` in `bin`.


## Tested runtime note

The optimized FFmpeg runtime was tested with common H.264/H.265 MKV/MP4, WebM VP9/AV1, AVI/XviD, WMV, M2TS/TS, VOB/MPEG, FLV, WebP output, style preview, and an 8-file batch run.


## Beta 2 baseline

The optimized runtime is the current Windows runtime baseline for beta 2. It reduced FFmpeg runtime DLLs from roughly 130 MiB to roughly 18-19 MiB in testing.

The non-strict configuration keeps demuxers, decoders, parsers and bitstream filters enabled for compatibility. Do not enable strict mode for public beta builds unless a full compatibility pass is repeated.
