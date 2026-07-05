// SPDX-License-Identifier: GPL-2.0-or-later
#include "VideoProbe.h"

#include <QDir>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/codec_desc.h>
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/pixdesc.h>
}

QString ffmpegErrorText(int err)
{
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {0};
    av_strerror(err, buffer, sizeof(buffer));
    return QString::fromUtf8(buffer);
}

QString secondsToTimeText(double seconds)
{
    if (seconds < 0.0) {
        seconds = 0.0;
    }

    const int total = static_cast<int>(seconds);
    const int h = total / 3600;
    const int m = (total % 3600) / 60;
    const int s = total % 60;

    return QString("%1:%2:%3")
        .arg(h)
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

QString durationToTimeText(double seconds)
{
    if (seconds <= 0.0) {
        return "unknown";
    }
    return secondsToTimeText(seconds);
}

QString fpsToText(double fps)
{
    if (fps <= 0.0) {
        return "unknown";
    }
    return QString::number(fps, 'f', 3);
}

QString VideoInfo::toText() const
{
    if (!ok) {
        return "FFmpeg probe failed\n\n" + error;
    }

    QString bitRateText = "unknown";
    if (bitRate > 0) {
        bitRateText = QString::number(bitRate / 1000) + " kb/s";
    }

    QString text;
    text += "FFmpeg probe OK\n\n";
    text += "File:\n";
    text += fileName + "\n\n";

    text += "Container:\n";
    text += containerName + "\n\n";

    text += "Video stream:\n";
    text += QString("Index: %1\n").arg(videoStreamIndex);
    text += QString("Codec: %1 (%2)\n").arg(codecLongName, codecShortName);
    text += QString("Resolution: %1 × %2\n").arg(width).arg(height);
    text += QString("Pixel format: %1\n").arg(pixelFormatName);
    text += QString("FPS avg: %1\n").arg(fpsToText(avgFps));
    text += QString("FPS real: %1\n").arg(fpsToText(realFps));
    text += QString("Duration: %1\n").arg(durationToTimeText(durationSeconds));
    text += QString("Bitrate: %1\n").arg(bitRateText);
    text += QString("Streams: %1\n\n").arg(streamCount);

    text += "Libraries:\n";
    text += QString("libavformat: %1\n").arg(avformat_version());
    text += QString("libavcodec: %1\n").arg(avcodec_version());
    text += QString("libavutil: %1\n").arg(avutil_version());

    return text;
}

VideoInfo VideoProbe::probe(const QString &fileName)
{
    VideoInfo info;
    info.fileName = fileName;

    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        info.error = QString("Could not open file:\n%1\n\nFFmpeg error:\n%2")
            .arg(fileName, ffmpegErrorText(ret));
        return info;
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        info.error = QString("Could not read stream information:\n%1")
            .arg(ffmpegErrorText(ret));
        avformat_close_input(&formatContext);
        return info;
    }

    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (ret < 0) {
        info.error = "The file does not contain a usable video stream.";
        avformat_close_input(&formatContext);
        return info;
    }

    const int videoStreamIndex = ret;
    AVStream *videoStream = formatContext->streams[videoStreamIndex];
    AVCodecParameters *codecParams = videoStream->codecpar;

    const AVCodecDescriptor *codecDesc = avcodec_descriptor_get(codecParams->codec_id);

    QString codecName;
    if (codecDesc && codecDesc->long_name) {
        codecName = QString::fromUtf8(codecDesc->long_name);
    } else {
        codecName = QString::fromUtf8(avcodec_get_name(codecParams->codec_id));
    }

    QString pixelFormat = "unknown";
    if (codecParams->format >= 0) {
        const char *pixName = av_get_pix_fmt_name(static_cast<AVPixelFormat>(codecParams->format));
        if (pixName) {
            pixelFormat = QString::fromUtf8(pixName);
        }
    }

    QString container = "unknown";
    if (formatContext->iformat && formatContext->iformat->long_name) {
        container = QString::fromUtf8(formatContext->iformat->long_name);
    }

    double durationSeconds = 0.0;
    if (formatContext->duration != AV_NOPTS_VALUE && formatContext->duration > 0) {
        durationSeconds = formatContext->duration / static_cast<double>(AV_TIME_BASE);
    }

    info.ok = true;
    info.containerName = container;
    info.videoStreamIndex = videoStreamIndex;
    info.codecLongName = codecName;
    info.codecShortName = QString::fromUtf8(avcodec_get_name(codecParams->codec_id));
    info.width = codecParams->width;
    info.height = codecParams->height;
    info.pixelFormatName = pixelFormat;
    info.avgFps = av_q2d(videoStream->avg_frame_rate);
    info.realFps = av_q2d(videoStream->r_frame_rate);
    info.durationSeconds = durationSeconds;
    info.bitRate = formatContext->bit_rate;
    info.streamCount = formatContext->nb_streams;

    avformat_close_input(&formatContext);
    return info;
}
