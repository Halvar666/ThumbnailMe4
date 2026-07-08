// SPDX-License-Identifier: GPL-2.0-or-later
#include "ContactSheetComposer.h"
#include "FrameGrabber.h"
#include "MediaInfoReader.h"
#include "VideoProbe.h"
#include "VideoTypes.h"

#include <QApplication>
#include <QColor>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>
#include <QTextStream>
#include <QVector>

#include <algorithm>
#include <cmath>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

static QString videoHeaderInfo(const QString &fileName, const VideoInfo &info)
{
    return MediaInfoReader::summaryForFile(fileName, info);
}

static int optionInt(
    const QCommandLineParser &parser,
    const QCommandLineOption &option,
    int fallback,
    int minimum,
    int maximum
)
{
    bool ok = false;
    int value = parser.value(option).toInt(&ok);

    if (!ok) {
        value = fallback;
    }

    if (value < minimum) {
        value = minimum;
    }

    if (value > maximum) {
        value = maximum;
    }

    return value;
}

static double optionDouble(
    const QCommandLineParser &parser,
    const QCommandLineOption &option,
    double fallback,
    double minimum,
    double maximum
)
{
    bool ok = false;
    double value = parser.value(option).toDouble(&ok);

    if (!ok) {
        value = fallback;
    }

    if (value < minimum) {
        value = minimum;
    }

    if (value > maximum) {
        value = maximum;
    }

    return value;
}


static QByteArray imageFormatForExtension(const QString &extension)
{
    const QString normalized = extension.trimmed().toLower();

    if (normalized == "jpg" || normalized == "jpeg") {
        return "JPG";
    }

    if (normalized == "webp") {
        return "WEBP";
    }

    if (normalized == "png") {
        return "PNG";
    }

    return normalized.toUtf8().toUpper();
}

static QImage prepareImageForWebP(const QImage &sheet, QTextStream &err)
{
    if (sheet.isNull()) {
        return QImage();
    }

    // WebP stores width/height in 14-bit fields. Very tall contact sheets,
    // especially from portrait 9:16 videos, can exceed this and make some
    // encoders fail badly. Keep the export usable and avoid worker crashes.
    constexpr int maxWebpSide = 16383;

    QImage image = sheet;
    if (image.width() > maxWebpSide || image.height() > maxWebpSide) {
        const QSize targetSize = image.size().scaled(
            maxWebpSide,
            maxWebpSide,
            Qt::KeepAspectRatio
        );

        err << "WebP output is too large (" << image.width() << "x" << image.height()
            << "); downscaling to " << targetSize.width() << "x" << targetSize.height()
            << " for WebP compatibility.\n";

        image = image.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    if (image.isNull()) {
        return QImage();
    }

    // RGB888 is the safest input for Qt/libwebp in AppImage builds. The
    // contact sheet background is opaque, so we do not need to preserve alpha.
    if (image.format() != QImage::Format_RGB888) {
        image = image.convertToFormat(QImage::Format_RGB888);
    }

    return image;
}

static bool savePngTempFile(const QImage &image, QString &pngPath, QString &error)
{
    QTemporaryFile tempFile(QDir::tempPath() + "/thumbnailme4_webp_XXXXXX.png");
    tempFile.setAutoRemove(false);

    if (!tempFile.open()) {
        error = "Could not create temporary PNG file.";
        return false;
    }

    pngPath = tempFile.fileName();
    tempFile.close();

    QImageWriter pngWriter(pngPath, "PNG");
    if (!pngWriter.write(image)) {
        error = "Temporary PNG save failed: " + pngWriter.errorString();
        QFile::remove(pngPath);
        return false;
    }

    return true;
}

static QString ffmpegExecutablePath()
{
#ifdef Q_OS_WIN
    const QString local = QDir(QCoreApplication::applicationDirPath()).filePath("ffmpeg.exe");
    if (QFileInfo::exists(local)) {
        return local;
    }
    return "ffmpeg.exe";
#else
    const QString local = QDir(QCoreApplication::applicationDirPath()).filePath("ffmpeg");
    if (QFileInfo::exists(local)) {
        return local;
    }
    return "ffmpeg";
#endif
}

static bool saveWebPViaFFmpeg(const QImage &image, const QString &outputFile, int quality, QString &error)
{
    QString tempPng;
    if (!savePngTempFile(image, tempPng, error)) {
        return false;
    }

    QStringList arguments;
    arguments << "-y";
    arguments << "-hide_banner";
    arguments << "-loglevel" << "error";
    arguments << "-i" << tempPng;
    arguments << "-c:v" << "libwebp";
    arguments << "-q:v" << QString::number(qBound(1, quality, 100));
    arguments << outputFile;

    QProcess process;
    process.start(ffmpegExecutablePath(), arguments);

    if (!process.waitForStarted(5000)) {
#ifdef Q_OS_WIN
        error = "ffmpeg.exe could not be started. Make sure it is present next to ThumbnailMeWorker.exe.";
#else
        error = "ffmpeg could not be started. Install ffmpeg or rely on the bundled Qt WebP image plugin.";
#endif
        QFile::remove(tempPng);
        return false;
    }

    process.waitForFinished(-1);

    const QByteArray stdErr = process.readAllStandardError();
    const bool ok = process.exitStatus() == QProcess::NormalExit
        && process.exitCode() == 0
        && QFileInfo::exists(outputFile)
        && QFileInfo(outputFile).size() > 0;

    if (!ok) {
        error = QString("ffmpeg WebP fallback failed. exitCode=%1. %2")
            .arg(process.exitCode())
            .arg(QString::fromLocal8Bit(stdErr).trimmed());
        QFile::remove(tempPng);
        return false;
    }

    QFile::remove(tempPng);
    return true;
}

static bool saveSheetImage(
    const QImage &sheet,
    const QString &outputFile,
    int quality,
    QTextStream &err
)
{
    const QString extension = QFileInfo(outputFile).suffix().toLower();
    const int saveQuality = extension == "png" ? -1 : quality;
    const QByteArray imageFormat = imageFormatForExtension(extension);

    QImage imageToSave = sheet;
    if (extension == "webp") {
        imageToSave = prepareImageForWebP(sheet, err);
        if (imageToSave.isNull()) {
            err << "WebP preparation failed.\n";
            return false;
        }
    }

    QImageWriter writer(outputFile, imageFormat);
    if (saveQuality >= 0) {
        writer.setQuality(saveQuality);
    }

    if (writer.write(imageToSave)) {
        return true;
    }

    QStringList formats;
    for (const QByteArray &format : QImageWriter::supportedImageFormats()) {
        formats << QString::fromLatin1(format);
    }

    err << "Qt image writer failed: " << writer.errorString() << "\n";
    err << "Requested image format: " << QString::fromLatin1(imageFormat) << "\n";
    err << "Supported Qt image formats: " << formats.join(", ") << "\n";

    if (extension == "webp") {
        err << "Trying ffmpeg WebP fallback...\n";

        QString fallbackError;
        if (saveWebPViaFFmpeg(imageToSave, outputFile, saveQuality >= 0 ? saveQuality : 95, fallbackError)) {
            err << "ffmpeg WebP fallback OK.\n";
            return true;
        }

        err << fallbackError << "\n";
    }

    return false;
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

    outImage = image;
    return !outImage.isNull();
}

static bool isProbablyDarkFrame(const QImage &image)
{
    if (image.isNull()) {
        return true;
    }

    const QImage rgb = image.format() == QImage::Format_RGB888
        ? image
        : image.convertToFormat(QImage::Format_RGB888);

    if (rgb.isNull()) {
        return true;
    }

    const int stepX = std::max(1, rgb.width() / 64);
    const int stepY = std::max(1, rgb.height() / 64);

    double sum = 0.0;
    double sumSquares = 0.0;
    int samples = 0;

    for (int y = 0; y < rgb.height(); y += stepY) {
        const uchar *line = rgb.constScanLine(y);
        for (int x = 0; x < rgb.width(); x += stepX) {
            const uchar *pixel = line + x * 3;
            const double luma =
                (pixel[0] * 299.0 + pixel[1] * 587.0 + pixel[2] * 114.0) / 1000.0;

            sum += luma;
            sumSquares += luma * luma;
            ++samples;
        }
    }

    if (samples <= 0) {
        return true;
    }

    const double average = sum / samples;
    const double variance = std::max(0.0, (sumSquares / samples) - average * average);
    const double deviation = std::sqrt(variance);

    return average < 18.0 || (average < 28.0 && deviation < 8.0);
}

static double frameSimilarityPercent(const QImage &first, const QImage &second)
{
    if (first.isNull() || second.isNull()) {
        return 0.0;
    }

    const QImage a = first.convertToFormat(QImage::Format_RGB888).scaled(
        32,
        18,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
    );

    const QImage b = second.convertToFormat(QImage::Format_RGB888).scaled(
        32,
        18,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
    );

    if (a.isNull() || b.isNull() || a.size() != b.size()) {
        return 0.0;
    }

    double sumDifference = 0.0;
    int samples = 0;

    for (int y = 0; y < a.height(); ++y) {
        const uchar *lineA = a.constScanLine(y);
        const uchar *lineB = b.constScanLine(y);

        for (int x = 0; x < a.width(); ++x) {
            const uchar *pixelA = lineA + x * 3;
            const uchar *pixelB = lineB + x * 3;

            const double lumaA =
                (pixelA[0] * 299.0 + pixelA[1] * 587.0 + pixelA[2] * 114.0) / 1000.0;
            const double lumaB =
                (pixelB[0] * 299.0 + pixelB[1] * 587.0 + pixelB[2] * 114.0) / 1000.0;

            sumDifference += std::abs(lumaA - lumaB);
            ++samples;
        }
    }

    if (samples <= 0) {
        return 0.0;
    }

    const double averageDifference = sumDifference / samples;
    const double similarity = 100.0 - (averageDifference / 255.0) * 100.0;
    return qBound(0.0, similarity, 100.0);
}

static bool isSimilarFrame(const QImage &candidate, const QImage &reference, double thresholdPercent)
{
    if (reference.isNull()) {
        return false;
    }

    return frameSimilarityPercent(candidate, reference) >= qBound(80.0, thresholdPercent, 99.5);
}

static QVector<double> replacementTimes(
    double requestedTime,
    double durationSeconds,
    double startMarginPercent,
    double endMarginPercent
)
{
    QVector<double> candidates;

    const double start = durationSeconds * qBound(0.0, startMarginPercent, 45.0) / 100.0;
    const double end = durationSeconds - durationSeconds * qBound(0.0, endMarginPercent, 45.0) / 100.0;
    const double safeStart = end > start ? start : 0.0;
    const double safeEnd = end > start ? end : durationSeconds;

    const double baseStep = std::max(2.0, durationSeconds * 0.01);
    for (int i = 1; i <= 6; ++i) {
        const double offset = baseStep * i;

        const double forward = requestedTime + offset;
        if (forward <= safeEnd) {
            candidates.append(forward);
        }

        const double backward = requestedTime - offset;
        if (backward >= safeStart) {
            candidates.append(backward);
        }
    }

    return candidates;
}

static bool decodeFramesSequentially(
    const QString &fileName,
    const QVector<double> &times,
    QVector<QImage> &frames,
    QTextStream &out,
    QTextStream &err,
    int targetWidth,
    int targetHeight
)
{
    frames.clear();
    if (times.isEmpty()) {
        err << "No requested timestamps.\n";
        return false;
    }

    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        err << "sequential avformat_open_input failed: " << ffmpegErrorText(ret) << "\n";
        return false;
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        err << "sequential avformat_find_stream_info failed: " << ffmpegErrorText(ret) << "\n";
        avformat_close_input(&formatContext);
        return false;
    }

    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (ret < 0) {
        err << "sequential: no video stream found.\n";
        avformat_close_input(&formatContext);
        return false;
    }

    const int videoStreamIndex = ret;
    AVStream *videoStream = formatContext->streams[videoStreamIndex];
    AVCodecParameters *codecParams = videoStream->codecpar;

    const AVCodec *decoder = avcodec_find_decoder(codecParams->codec_id);
    if (!decoder) {
        err << "sequential: no decoder found.\n";
        avformat_close_input(&formatContext);
        return false;
    }

    codecContext = avcodec_alloc_context3(decoder);
    if (!codecContext) {
        err << "sequential: avcodec_alloc_context3 failed.\n";
        avformat_close_input(&formatContext);
        return false;
    }

    ret = avcodec_parameters_to_context(codecContext, codecParams);
    if (ret < 0) {
        err << "sequential avcodec_parameters_to_context failed: " << ffmpegErrorText(ret) << "\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    ret = avcodec_open2(codecContext, decoder, nullptr);
    if (ret < 0) {
        err << "sequential avcodec_open2 failed: " << ffmpegErrorText(ret) << "\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    packet = av_packet_alloc();
    frame = av_frame_alloc();
    if (!packet || !frame) {
        err << "sequential: packet/frame allocation failed.\n";
        av_packet_free(&packet);
        av_frame_free(&frame);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    int targetIndex = 0;
    int decodedFrames = 0;
    QImage lastImage;
    QString conversionError;
    const double toleranceSeconds = 0.040;

    auto handleFrame = [&](AVFrame *decodedFrame) {
        ++decodedFrames;

        double currentTime = -1.0;
        const int64_t bestTimestamp = decodedFrame->best_effort_timestamp;
        if (bestTimestamp != AV_NOPTS_VALUE) {
            currentTime = bestTimestamp * av_q2d(videoStream->time_base);
        } else if (codecContext->framerate.num > 0 && codecContext->framerate.den > 0) {
            currentTime = decodedFrames / av_q2d(codecContext->framerate);
        } else if (videoStream->avg_frame_rate.num > 0 && videoStream->avg_frame_rate.den > 0) {
            currentTime = decodedFrames / av_q2d(videoStream->avg_frame_rate);
        }

        QImage converted;
        QString localError;
        if (!convertFrameToImage(decodedFrame, converted, localError, targetWidth, targetHeight)) {
            conversionError = localError;
            return;
        }

        lastImage = converted;

        while (targetIndex < times.size()) {
            const double wanted = times[targetIndex];
            if (currentTime >= 0.0 && currentTime + toleranceSeconds < wanted) {
                break;
            }

            frames.append(converted);
            out << "FRAME " << (targetIndex + 1) << "/" << times.size()
                << " OK sequential t=" << secondsToTimeText(currentTime) << "\n";
            out.flush();
            ++targetIndex;
        }
    };

    auto receiveAll = [&]() {
        while (true) {
            ret = avcodec_receive_frame(codecContext, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break;
            }
            if (ret < 0) {
                err << "sequential avcodec_receive_frame failed: " << ffmpegErrorText(ret) << "\n";
                break;
            }

            handleFrame(frame);
            av_frame_unref(frame);

            if (targetIndex >= times.size()) {
                break;
            }
        }
    };

    while (targetIndex < times.size() && av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(codecContext, packet);
            if (ret >= 0 || ret == AVERROR(EAGAIN)) {
                receiveAll();
            } else {
                err << "sequential avcodec_send_packet failed: " << ffmpegErrorText(ret) << "\n";
            }
        }
        av_packet_unref(packet);
    }

    if (targetIndex < times.size()) {
        ret = avcodec_send_packet(codecContext, nullptr);
        if (ret >= 0 || ret == AVERROR_EOF) {
            receiveAll();
        }
    }

    while (targetIndex < times.size() && !lastImage.isNull()) {
        frames.append(lastImage);
        out << "FRAME " << (targetIndex + 1) << "/" << times.size()
            << " OK sequential fallback last frame\n";
        out.flush();
        ++targetIndex;
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    if (frames.isEmpty()) {
        if (!conversionError.isEmpty()) {
            err << "sequential conversion failed: " << conversionError << "\n";
        } else {
            err << "sequential: no frames decoded.\n";
        }
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ThumbnailMeWorker");

    QTextStream out(stdout);
    QTextStream err(stderr);

    QCommandLineParser parser;
    parser.setApplicationDescription("Thumbnail me Qt6 worker process + MediaInfo header");
    parser.addHelpOption();

    QCommandLineOption inputOpt(QStringList() << "i" << "input", "Input video file.", "file");
    QCommandLineOption outputOpt(QStringList() << "o" << "output", "Output image file.", "file");
    QCommandLineOption columnsOpt("columns", "Number of columns, horizontal.", "n", "4");
    QCommandLineOption rowsOpt("rows", "Number of rows, vertical.", "n", "4");
    QCommandLineOption thumbWidthOpt("thumb-width", "Thumbnail width in pixels.", "px", "320");
    QCommandLineOption gapOpt("gap", "Gap between thumbnails in pixels.", "px", "8");
    QCommandLineOption timestampsOpt("timestamps", "Draw timestamps, 1 or 0.", "bool", "1");
    QCommandLineOption qualityOpt("quality", "JPEG quality.", "n", "95");
    QCommandLineOption fontOpt("font", "QFont string.", "font");
    QCommandLineOption seekModeOpt("seek-mode", "Frame seeking mode: fast, balanced, accurate.", "mode", "fast");
    QCommandLineOption startMarginOpt("start-margin", "Start margin in percent.", "percent", "7");
    QCommandLineOption endMarginOpt("end-margin", "End margin in percent.", "percent", "7");
    QCommandLineOption skipDarkOpt("skip-dark", "Try to replace dark or blank frames, 1 or 0.", "bool", "0");
    QCommandLineOption skipSimilarOpt("skip-similar", "Try to replace frames similar to the previous accepted thumbnail, 1 or 0.", "bool", "0");
    QCommandLineOption similarThresholdOpt("similar-threshold", "Similarity threshold in percent.", "percent", "92");
    QCommandLineOption randomizeOpt("randomize", "Randomize positions, 1 or 0.", "bool", "0");
    QCommandLineOption randomizePercentOpt("randomize-percent", "Randomize amount in percent.", "percent", "10");
    QCommandLineOption specificTimestepOpt("specific-timestep", "Use fixed timestep, 1 or 0.", "bool", "0");
    QCommandLineOption timestepOpt("timestep", "Fixed timestep in seconds.", "seconds", "60");
    QCommandLineOption showInfoOpt("show-info", "Show technical information text, 1 or 0.", "bool", "1");
    QCommandLineOption infoPositionOpt("info-position", "Technical information text position: upper-left, lower-left, lower-right, upper-right.", "position", "upper-left");
    QCommandLineOption timestampPositionOpt("timestamp-position", "Timestamp position inside thumbnails: upper-left, lower-left, lower-right, upper-right.", "position", "lower-left");
    QCommandLineOption additionalTitleEnabledOpt("additional-title-enabled", "Show additional title, 1 or 0.", "bool", "1");
    QCommandLineOption additionalTitleOpt("additional-title", "Additional title text.", "text", "Generated by Thumbnail me");
    QCommandLineOption backgroundColorOpt("background-color", "Background color, e.g. #141414.", "color", "#141414");
    QCommandLineOption titleTextColorOpt("title-text-color", "Title text color.", "color", "#ffffff");
    QCommandLineOption infoTextColorOpt("info-text-color", "Info text color.", "color", "#e6e6e6");
    QCommandLineOption countTextColorOpt("count-text-color", "Thumbnail count text color.", "color", "#d2d2d2");
    QCommandLineOption timestampTextColorOpt("timestamp-text-color", "Timestamp text color.", "color", "#ffffff");
    QCommandLineOption timestampBackgroundColorOpt("timestamp-background-color", "Timestamp background color.", "color", "#000000");
    QCommandLineOption tileBorderOpt("tile-border", "Draw tile border, 1 or 0.", "bool", "1");
    QCommandLineOption tileBorderColorOpt("tile-border-color", "Tile border color.", "color", "#464646");

    parser.addOption(inputOpt);
    parser.addOption(outputOpt);
    parser.addOption(columnsOpt);
    parser.addOption(rowsOpt);
    parser.addOption(thumbWidthOpt);
    parser.addOption(gapOpt);
    parser.addOption(timestampsOpt);
    parser.addOption(qualityOpt);
    parser.addOption(fontOpt);
    parser.addOption(seekModeOpt);
    parser.addOption(startMarginOpt);
    parser.addOption(endMarginOpt);
    parser.addOption(skipDarkOpt);
    parser.addOption(skipSimilarOpt);
    parser.addOption(similarThresholdOpt);
    parser.addOption(randomizeOpt);
    parser.addOption(randomizePercentOpt);
    parser.addOption(specificTimestepOpt);
    parser.addOption(timestepOpt);
    parser.addOption(showInfoOpt);
    parser.addOption(infoPositionOpt);
    parser.addOption(timestampPositionOpt);
    parser.addOption(additionalTitleEnabledOpt);
    parser.addOption(additionalTitleOpt);
    parser.addOption(backgroundColorOpt);
    parser.addOption(titleTextColorOpt);
    parser.addOption(infoTextColorOpt);
    parser.addOption(countTextColorOpt);
    parser.addOption(timestampTextColorOpt);
    parser.addOption(timestampBackgroundColorOpt);
    parser.addOption(tileBorderOpt);
    parser.addOption(tileBorderColorOpt);

    parser.process(app);

    const QString inputFile = parser.value(inputOpt);
    const QString outputFile = parser.value(outputOpt);

    if (inputFile.isEmpty() || outputFile.isEmpty()) {
        err << "Missing input or output file.\n";
        return 2;
    }

    const int columns = optionInt(parser, columnsOpt, 4, 1, 50);
    const int rows = optionInt(parser, rowsOpt, 4, 1, 50);
    const int thumbWidth = optionInt(parser, thumbWidthOpt, 320, 120, 1200);
    const int gap = optionInt(parser, gapOpt, 8, 0, 80);
    const int quality = optionInt(parser, qualityOpt, 95, 1, 100);
    const bool drawTimestamps = parser.value(timestampsOpt) != "0";
    const QString seekModeValue = parser.value(seekModeOpt).trimmed().toLower();
    const double startMarginPercent = optionDouble(parser, startMarginOpt, 7.0, 0.0, 45.0);
    const double endMarginPercent = optionDouble(parser, endMarginOpt, 7.0, 0.0, 45.0);
    const bool skipDarkFrames = parser.value(skipDarkOpt) != "0";
    const bool skipSimilarFrames = parser.value(skipSimilarOpt) != "0";
    const double similarThresholdPercent = optionDouble(parser, similarThresholdOpt, 92.0, 80.0, 99.5);
    const bool randomizePositions = parser.value(randomizeOpt) != "0";
    const double randomizePercent = optionDouble(parser, randomizePercentOpt, 10.0, 1.0, 50.0);
    const bool useSpecificTimestep = parser.value(specificTimestepOpt) != "0";
    const double timestepSeconds = optionDouble(parser, timestepOpt, 60.0, 1.0, 3600.0);
    const bool showInfoText = parser.value(showInfoOpt) != "0";

    const QString rawInfoTextPosition = parser.value(infoPositionOpt).trimmed().toLower();
    const QString infoTextPosition =
        rawInfoTextPosition == "upper-right" || rawInfoTextPosition == "lower-left" || rawInfoTextPosition == "lower-right"
            ? rawInfoTextPosition
            : QString("upper-left");

    const QString rawTimestampPosition = parser.value(timestampPositionOpt).trimmed().toLower();
    const QString timestampPosition =
        rawTimestampPosition == "upper-left" || rawTimestampPosition == "upper-right" || rawTimestampPosition == "lower-right"
            ? rawTimestampPosition
            : QString("lower-left");

    const bool showAdditionalTitle = parser.value(additionalTitleEnabledOpt) != "0";
    const QString additionalTitle = parser.value(additionalTitleOpt);
    const QColor backgroundColor = QColor(parser.value(backgroundColorOpt)).isValid()
        ? QColor(parser.value(backgroundColorOpt))
        : QColor(20, 20, 20);

    auto colorOption = [&parser](const QCommandLineOption &option, const QColor &fallback) {
        const QColor color(parser.value(option));
        return color.isValid() ? color : fallback;
    };

    const QColor titleTextColor = colorOption(titleTextColorOpt, QColor(255, 255, 255));
    const QColor infoTextColor = colorOption(infoTextColorOpt, QColor(230, 230, 230));
    const QColor countTextColor = colorOption(countTextColorOpt, QColor(210, 210, 210));
    const QColor timestampTextColor = colorOption(timestampTextColorOpt, QColor(255, 255, 255));
    QColor timestampBackgroundColor = colorOption(timestampBackgroundColorOpt, QColor(0, 0, 0));
    timestampBackgroundColor.setAlpha(170);
    const bool drawTileBorder = parser.value(tileBorderOpt) != "0";
    const QColor tileBorderColor = colorOption(tileBorderColorOpt, QColor(70, 70, 70));

    const int count = columns * rows;

    QFont selectedFont;
    if (!parser.value(fontOpt).isEmpty()) {
        selectedFont.fromString(parser.value(fontOpt));
    }

    if (selectedFont.pointSize() <= 0) {
        selectedFont.setPointSize(10);
    }

    out << "INPUT: " << inputFile << "\n";
    out << "OUTPUT: " << outputFile << "\n";
    out << "GRID: " << columns << "x" << rows << "\n";
    out << "FRAME_SELECTION: start=" << startMarginPercent
        << "% end=" << endMarginPercent
        << "% skip_dark=" << (skipDarkFrames ? "yes" : "no")
        << " skip_similar=" << (skipSimilarFrames ? QString("yes@%1%").arg(similarThresholdPercent, 0, 'f', 1) : QString("no"))
        << " randomize=" << (randomizePositions ? "yes" : "no")
        << " timestep=" << (useSpecificTimestep ? QString::number(timestepSeconds) + "s" : QString("off"))
        << " info_text=" << (showInfoText ? "yes" : "no")
        << " info_position=" << infoTextPosition
        << " timestamp_position=" << timestampPosition
        << " additional_title=" << (showAdditionalTitle ? "yes" : "no")
        << " background=" << backgroundColor.name(QColor::HexRgb)
        << " title_color=" << titleTextColor.name(QColor::HexRgb)
        << " info_color=" << infoTextColor.name(QColor::HexRgb)
        << " timestamp_color=" << timestampTextColor.name(QColor::HexRgb)
        << " tile_border=" << (drawTileBorder ? tileBorderColor.name(QColor::HexRgb) : QString("off"))
        << "\n";
    out.flush();

    const VideoInfo info = VideoProbe::probe(inputFile);
    if (!info.ok || info.durationSeconds <= 0.0) {
        err << "Probe failed:\n" << info.toText() << "\n";
        return 3;
    }

    const QVector<double> times = ContactSheetComposer::makeEvenTimes(
        info.durationSeconds,
        count,
        startMarginPercent,
        endMarginPercent,
        randomizePositions,
        randomizePercent,
        useSpecificTimestep,
        timestepSeconds
    );
    QVector<QImage> frames;
    frames.reserve(times.size());

    const double aspect = (info.width > 0 && info.height > 0)
        ? info.width / static_cast<double>(info.height)
        : 16.0 / 9.0;
    const int targetWidth = thumbWidth;
    const int targetHeight = qMax(1, static_cast<int>(thumbWidth / aspect));

    const bool useSequentialMode =
        !skipDarkFrames &&
        !skipSimilarFrames &&
        seekModeValue != "fast" &&
        info.durationSeconds <= 120.0;

    const FrameGrabber::SeekMode frameSeekMode =
        seekModeValue == "accurate" ? FrameGrabber::SeekMode::Accurate : FrameGrabber::SeekMode::Fast;

    if (useSequentialMode) {
        out << "MODE: sequential short-video decode (" << seekModeValue << ")\n";
        out.flush();
        if (!decodeFramesSequentially(inputFile, times, frames, out, err, targetWidth, targetHeight)) {
            return 4;
        }
    } else {
        out << "MODE: " << (frameSeekMode == FrameGrabber::SeekMode::Fast ? "fast seek per thumbnail" : "accurate seek per thumbnail") << "\n";
        out.flush();

        QImage previousAcceptedFrame;

        for (int i = 0; i < times.size(); ++i) {
            QImage frame;
            QString frameLog;

            bool frameOk = FrameGrabber::grabFrameAt(inputFile, times[i], frame, frameLog, frameSeekMode, targetWidth, targetHeight);

            auto frameNeedsReplacement = [&](const QImage &candidate) {
                if (skipDarkFrames && isProbablyDarkFrame(candidate)) {
                    return true;
                }

                if (skipSimilarFrames && isSimilarFrame(candidate, previousAcceptedFrame, similarThresholdPercent)) {
                    return true;
                }

                return false;
            };

            if (frameOk && frameNeedsReplacement(frame)) {
                const bool dark = skipDarkFrames && isProbablyDarkFrame(frame);
                const bool similar = skipSimilarFrames && isSimilarFrame(frame, previousAcceptedFrame, similarThresholdPercent);

                out << "FRAME " << (i + 1) << "/" << times.size()
                    << (dark ? " DARK" : "")
                    << (similar ? " SIMILAR" : "")
                    << ", trying alternatives\n";

                const QVector<double> alternatives = replacementTimes(
                    times[i],
                    info.durationSeconds,
                    startMarginPercent,
                    endMarginPercent
                );

                for (const double candidateTime : alternatives) {
                    QImage replacementFrame;
                    QString replacementLog;

                    if (!FrameGrabber::grabFrameAt(inputFile, candidateTime, replacementFrame, replacementLog, frameSeekMode, targetWidth, targetHeight)) {
                        continue;
                    }

                    if (!frameNeedsReplacement(replacementFrame)) {
                        frame = replacementFrame;
                        frameLog = replacementLog;
                        frameOk = true;
                        out << "FRAME " << (i + 1) << "/" << times.size()
                            << " substituted at " << secondsToTimeText(candidateTime) << "\n";
                        break;
                    }
                }
            }

            if (frameOk) {
                frames.append(frame);
                previousAcceptedFrame = frame;
                out << "FRAME " << (i + 1) << "/" << times.size() << " OK\n";
            } else {
                out << "FRAME " << (i + 1) << "/" << times.size() << " FAILED: " << frameLog << "\n";
            }

            out.flush();
        }
    }

    if (frames.isEmpty()) {
        err << "No frames decoded.\n";
        return 4;
    }

    const QString title = QFileInfo(inputFile).fileName();

    QImage sheet = ContactSheetComposer::compose(
        frames,
        times,
        columns,
        rows,
        thumbWidth,
        gap,
        title,
        videoHeaderInfo(inputFile, info),
        drawTimestamps,
        selectedFont,
        showInfoText,
        infoTextPosition,
        timestampPosition,
        showAdditionalTitle,
        additionalTitle,
        backgroundColor,
        titleTextColor,
        infoTextColor,
        countTextColor,
        timestampTextColor,
        timestampBackgroundColor,
        drawTileBorder,
        tileBorderColor
    );

    frames.clear();

    if (sheet.isNull()) {
        err << "Contact sheet composition failed.\n";
        return 5;
    }

    if (!saveSheetImage(sheet, outputFile, quality, err)) {
        err << "Save failed: " << outputFile << "\n";
        return 6;
    }

    out << "SAVED: " << outputFile << "\n";
    out.flush();

    return 0;
}
