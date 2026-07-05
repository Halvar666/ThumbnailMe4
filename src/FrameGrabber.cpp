// SPDX-License-Identifier: GPL-2.0-or-later
#include "FrameGrabber.h"

#include "VideoTypes.h"

#include <QDir>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

static bool convertFrameToImage(AVFrame *frame, QImage &outImage, QString &outLog, int targetWidth = 0, int targetHeight = 0)
{
    if (!frame || frame->width <= 0 || frame->height <= 0 || frame->format < 0) {
        outLog = "Invalid decoded frame.";
        return false;
    }

    const int outWidth = targetWidth > 0 ? targetWidth : frame->width;
    const int outHeight = targetHeight > 0 ? targetHeight : frame->height;

    SwsContext *swsContext = sws_getContext(
        frame->width,
        frame->height,
        static_cast<AVPixelFormat>(frame->format),
        outWidth,
        outHeight,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        nullptr,
        nullptr,
        nullptr
    );

    if (!swsContext) {
        outLog = "sws_getContext failed.";
        return false;
    }

    QImage image(outWidth, outHeight, QImage::Format_RGB888);
    if (image.isNull()) {
        sws_freeContext(swsContext);
        outLog = "QImage allocation failed.";
        return false;
    }

    uint8_t *dstData[4] = {
        image.bits(),
        nullptr,
        nullptr,
        nullptr
    };

    int dstLinesize[4] = {
        image.bytesPerLine(),
        0,
        0,
        0
    };

    const uint8_t *srcData[4] = {
        frame->data[0],
        frame->data[1],
        frame->data[2],
        frame->data[3]
    };

    sws_scale(
        swsContext,
        srcData,
        frame->linesize,
        0,
        frame->height,
        dstData,
        dstLinesize
    );

    sws_freeContext(swsContext);

    outImage = image.copy();
    return !outImage.isNull();
}

bool FrameGrabber::grabFrameAt(
    const QString &fileName,
    double seconds,
    QImage &outImage,
    QString &outLog,
    SeekMode seekMode,
    int targetWidth,
    int targetHeight
)
{
    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;
    AVFrame *fallbackFrame = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        outLog = "avformat_open_input failed: " + ffmpegErrorText(ret);
        return false;
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        outLog = "avformat_find_stream_info failed: " + ffmpegErrorText(ret);
        avformat_close_input(&formatContext);
        return false;
    }

    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (ret < 0) {
        outLog = "No video stream found.";
        avformat_close_input(&formatContext);
        return false;
    }

    const int videoStreamIndex = ret;
    AVStream *videoStream = formatContext->streams[videoStreamIndex];
    AVCodecParameters *codecParams = videoStream->codecpar;

    const AVCodec *decoder = avcodec_find_decoder(codecParams->codec_id);
    if (!decoder) {
        outLog = "No decoder found for codec: " + QString::fromUtf8(avcodec_get_name(codecParams->codec_id));
        avformat_close_input(&formatContext);
        return false;
    }

    codecContext = avcodec_alloc_context3(decoder);
    if (!codecContext) {
        outLog = "avcodec_alloc_context3 failed.";
        avformat_close_input(&formatContext);
        return false;
    }

    ret = avcodec_parameters_to_context(codecContext, codecParams);
    if (ret < 0) {
        outLog = "avcodec_parameters_to_context failed: " + ffmpegErrorText(ret);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    ret = avcodec_open2(codecContext, decoder, nullptr);
    if (ret < 0) {
        outLog = "avcodec_open2 failed: " + ffmpegErrorText(ret);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    if (seconds < 0.0) {
        seconds = 0.0;
    }

    const AVRational avTimeBase = {1, AV_TIME_BASE};
    const int64_t seekMicros = static_cast<int64_t>(seconds * AV_TIME_BASE);
    const int64_t seekTarget = av_rescale_q(seekMicros, avTimeBase, videoStream->time_base);

    ret = av_seek_frame(formatContext, videoStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD);
    if (ret < 0) {
        ret = av_seek_frame(formatContext, -1, seekMicros, AVSEEK_FLAG_BACKWARD);
    }

    if (ret < 0) {
        outLog = "av_seek_frame failed: " + ffmpegErrorText(ret);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    avcodec_flush_buffers(codecContext);

    packet = av_packet_alloc();
    frame = av_frame_alloc();
    fallbackFrame = av_frame_alloc();

    if (!packet || !frame || !fallbackFrame) {
        outLog = "Could not allocate AVPacket/AVFrame.";
        av_packet_free(&packet);
        av_frame_free(&frame);
        av_frame_free(&fallbackFrame);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    bool gotFrame = false;
    bool haveFallbackFrame = false;
    QString conversionError;
    double decodedTime = -1.0;
    double fallbackTime = -1.0;
    int decodedFrameCount = 0;

    // 40 ms tolerance is roughly one frame at 25 fps.  It prevents skipping a good
    // frame because of tiny timestamp rounding differences.
    const double toleranceSeconds = 0.040;

    auto handleDecodedFrame = [&](AVFrame *decodedFrame) -> bool {
        ++decodedFrameCount;

        double currentTime = -1.0;
        const int64_t bestTimestamp = decodedFrame->best_effort_timestamp;
        if (bestTimestamp != AV_NOPTS_VALUE) {
            currentTime = bestTimestamp * av_q2d(videoStream->time_base);
        }

        av_frame_unref(fallbackFrame);
        if (av_frame_ref(fallbackFrame, decodedFrame) == 0) {
            haveFallbackFrame = true;
            fallbackTime = currentTime;
        }

        if (seekMode == SeekMode::Fast) {
            if (convertFrameToImage(decodedFrame, outImage, conversionError, targetWidth, targetHeight)) {
                decodedTime = currentTime;
                gotFrame = true;
                return true;
            }

            return true;
        }

        // Accurate mode decodes forward until the displayed timestamp reaches
        // the requested time. This is safer for sparse keyframes, but slower
        // than the original Thumbnail me / mtn-style fast seek.
        if (currentTime < 0.0 || currentTime + toleranceSeconds >= seconds) {
            if (convertFrameToImage(decodedFrame, outImage, conversionError, targetWidth, targetHeight)) {
                decodedTime = currentTime;
                gotFrame = true;
                return true;
            }

            return true;
        }

        return false;
    };

    auto receiveAvailableFrames = [&]() -> bool {
        while (true) {
            ret = avcodec_receive_frame(codecContext, frame);

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                return false;
            }

            if (ret < 0) {
                outLog = "avcodec_receive_frame failed: " + ffmpegErrorText(ret);
                return true;
            }

            const bool stop = handleDecodedFrame(frame);
            av_frame_unref(frame);

            if (stop) {
                return true;
            }
        }
    };

    while (!gotFrame && av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(codecContext, packet);

            if (ret < 0 && ret != AVERROR(EAGAIN)) {
                outLog = "avcodec_send_packet failed: " + ffmpegErrorText(ret);
                av_packet_unref(packet);
                break;
            }

            const bool stop = receiveAvailableFrames();
            if (stop) {
                av_packet_unref(packet);
                break;
            }
        }

        av_packet_unref(packet);
    }

    if (!gotFrame && outLog.isEmpty()) {
        // Flush delayed frames.  This matters especially for H.264 with B-frames.
        ret = avcodec_send_packet(codecContext, nullptr);
        if (ret >= 0 || ret == AVERROR_EOF) {
            receiveAvailableFrames();
        }
    }

    if (!gotFrame && outLog.isEmpty() && haveFallbackFrame) {
        // If the requested timestamp is very near EOF or timestamps are odd, use
        // the last frame decoded after seeking instead of failing completely.
        if (convertFrameToImage(fallbackFrame, outImage, conversionError, targetWidth, targetHeight)) {
            gotFrame = true;
            decodedTime = fallbackTime;
            outLog = QString("Frame grab fallback OK | requested: %1 | decoded: %2 | decoded frames: %3")
                .arg(secondsToTimeText(seconds))
                .arg(decodedTime >= 0.0 ? secondsToTimeText(decodedTime) : QString("unknown"))
                .arg(decodedFrameCount);
        }
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&fallbackFrame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    if (!gotFrame) {
        if (!conversionError.isEmpty()) {
            outLog = conversionError;
        }

        if (outLog.isEmpty()) {
            outLog = "Frame was not decoded.";
        }

        return false;
    }

    if (outLog.isEmpty()) {
        outLog = QString("%1 frame grab OK | requested: %2 | decoded: %3 | decoded frames: %4")
            .arg(seekMode == SeekMode::Fast ? QString("Fast") : QString("Accurate"))
            .arg(secondsToTimeText(seconds))
            .arg(decodedTime >= 0.0 ? secondsToTimeText(decodedTime) : QString("unknown"))
            .arg(decodedFrameCount);
    }

    return true;
}
