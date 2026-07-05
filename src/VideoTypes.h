// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QString>

extern "C" {
#include <libavcodec/codec_id.h>
#include <libavutil/avutil.h>
}

struct VideoInfo
{
    bool ok = false;
    QString error;

    QString fileName;
    QString containerName;
    QString codecLongName;
    QString codecShortName;
    QString pixelFormatName;

    int videoStreamIndex = -1;
    int width = 0;
    int height = 0;
    double avgFps = 0.0;
    double realFps = 0.0;
    double durationSeconds = 0.0;
    qint64 bitRate = 0;
    unsigned int streamCount = 0;

    QString toText() const;
};

QString ffmpegErrorText(int err);
QString secondsToTimeText(double seconds);
QString durationToTimeText(double seconds);
QString fpsToText(double fps);
