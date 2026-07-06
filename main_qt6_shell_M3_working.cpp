#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPixmap>
#include <QScrollArea>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/codec_desc.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/error.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

static QString ffmpegErrorText(int err)
{
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {0};
    av_strerror(err, buffer, sizeof(buffer));
    return QString::fromUtf8(buffer);
}

static QString formatDuration(int64_t duration)
{
    if (duration == AV_NOPTS_VALUE || duration <= 0) {
        return "neznámá";
    }

    const double totalSeconds = duration / static_cast<double>(AV_TIME_BASE);
    const int hours = static_cast<int>(totalSeconds) / 3600;
    const int minutes = (static_cast<int>(totalSeconds) % 3600) / 60;
    const int seconds = static_cast<int>(totalSeconds) % 60;

    return QString("%1:%2:%3")
        .arg(hours)
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

static QString rationalToFps(AVRational rate)
{
    if (rate.num <= 0 || rate.den <= 0) {
        return "neznámé";
    }

    return QString::number(av_q2d(rate), 'f', 3);
}

static QString secondsToTimeText(double seconds)
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

static QString probeVideoFile(const QString &fileName)
{
    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        return QString("Nepodařilo se otevřít soubor:\n%1\n\nFFmpeg chyba:\n%2")
            .arg(fileName, ffmpegErrorText(ret));
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        avformat_close_input(&formatContext);
        return QString("Nepodařilo se načíst informace o streamu:\n%1")
            .arg(ffmpegErrorText(ret));
    }

    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (ret < 0) {
        avformat_close_input(&formatContext);
        return "Soubor neobsahuje použitelný video stream.";
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

    QString codecShortName = QString::fromUtf8(avcodec_get_name(codecParams->codec_id));

    QString pixelFormat = "neznámý";
    if (codecParams->format >= 0) {
        const char *pixName = av_get_pix_fmt_name(static_cast<AVPixelFormat>(codecParams->format));
        if (pixName) {
            pixelFormat = QString::fromUtf8(pixName);
        }
    }

    QString container = "neznámý";
    if (formatContext->iformat && formatContext->iformat->long_name) {
        container = QString::fromUtf8(formatContext->iformat->long_name);
    }

    QString bitRate = "neznámý";
    if (formatContext->bit_rate > 0) {
        bitRate = QString::number(formatContext->bit_rate / 1000) + " kb/s";
    }

    QString text;
    text += "FFmpeg probe OK\n\n";
    text += "Soubor:\n";
    text += fileName + "\n\n";

    text += "Kontejner:\n";
    text += container + "\n\n";

    text += "Video stream:\n";
    text += QString("Index: %1\n").arg(videoStreamIndex);
    text += QString("Codec: %1 (%2)\n").arg(codecName, codecShortName);
    text += QString("Rozlišení: %1 × %2\n").arg(codecParams->width).arg(codecParams->height);
    text += QString("Pixel format: %1\n").arg(pixelFormat);
    text += QString("FPS avg: %1\n").arg(rationalToFps(videoStream->avg_frame_rate));
    text += QString("FPS real: %1\n").arg(rationalToFps(videoStream->r_frame_rate));
    text += QString("Délka: %1\n").arg(formatDuration(formatContext->duration));
    text += QString("Bitrate: %1\n").arg(bitRate);
    text += QString("Počet streamů: %1\n\n").arg(formatContext->nb_streams);

    text += "Knihovny:\n";
    text += QString("libavformat: %1\n").arg(avformat_version());
    text += QString("libavcodec: %1\n").arg(avcodec_version());
    text += QString("libavutil: %1\n").arg(avutil_version());

    avformat_close_input(&formatContext);
    return text;
}

static bool grabFrameAt(const QString &fileName, double seconds, QImage &outImage, QString &outLog)
{
    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;

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

    AVCodecContext *codecContext = avcodec_alloc_context3(decoder);
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

    const int64_t seekTarget = static_cast<int64_t>(seconds / av_q2d(videoStream->time_base));

    ret = av_seek_frame(formatContext, videoStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD);
    if (ret < 0) {
        outLog = "av_seek_frame failed: " + ffmpegErrorText(ret);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    avcodec_flush_buffers(codecContext);

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();

    if (!packet || !frame) {
        outLog = "Could not allocate AVPacket/AVFrame.";
        av_packet_free(&packet);
        av_frame_free(&frame);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return false;
    }

    bool gotFrame = false;
    double decodedTime = -1.0;

    while (av_read_frame(formatContext, packet) >= 0 && !gotFrame) {
        if (packet->stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(codecContext, packet);

            if (ret >= 0) {
                while (ret >= 0) {
                    ret = avcodec_receive_frame(codecContext, frame);

                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    }

                    if (ret < 0) {
                        outLog = "avcodec_receive_frame failed: " + ffmpegErrorText(ret);
                        break;
                    }

                    int64_t bestTimestamp = frame->best_effort_timestamp;
                    if (bestTimestamp != AV_NOPTS_VALUE) {
                        decodedTime = bestTimestamp * av_q2d(videoStream->time_base);
                    }

                    SwsContext *swsContext = sws_getContext(
                        frame->width,
                        frame->height,
                        static_cast<AVPixelFormat>(frame->format),
                        frame->width,
                        frame->height,
                        AV_PIX_FMT_RGB24,
                        SWS_BILINEAR,
                        nullptr,
                        nullptr,
                        nullptr
                    );

                    if (!swsContext) {
                        outLog = "sws_getContext failed.";
                        break;
                    }

                    QImage image(frame->width, frame->height, QImage::Format_RGB888);

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
                    gotFrame = true;
                    break;
                }
            }
        }

        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    if (!gotFrame) {
        outLog = "Frame nebyl dekódován.";
        return false;
    }

    outLog = QString("Frame grab OK\nPožadovaný čas: %1\nDekódovaný čas: %2")
        .arg(secondsToTimeText(seconds))
        .arg(decodedTime >= 0.0 ? secondsToTimeText(decodedTime) : QString("neznámý"));

    return true;
}

class MainWindow : public QMainWindow
{
public:
    MainWindow()
    {
        setWindowTitle("Thumbnail me 3.5 - Qt6 + FFmpeg frame grab");
        resize(1100, 760);

        auto *splitter = new QSplitter(this);

        output = new QTextEdit(splitter);
        output->setReadOnly(true);
        output->setText(
            "Thumbnail me Qt6 shell\n\n"
            "Milestone 3:\n"
            "- Qt6 Widgets běží\n"
            "- FFmpeg knihovny jsou linkované přímo\n"
            "- File → Open video načte metadata\n"
            "- Aplikace vytáhne jeden frame z poloviny videa a zobrazí ho\n\n"
            "Zatím stále nejde o finální UI, jen bezpečný portovací checkpoint."
        );

        preview = new QLabel(splitter);
        preview->setAlignment(Qt::AlignCenter);
        preview->setText("Preview frame");
        preview->setMinimumSize(480, 360);
        preview->setStyleSheet("QLabel { background: #202020; color: #dddddd; }");

        splitter->addWidget(output);
        splitter->addWidget(preview);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 2);

        setCentralWidget(splitter);

        auto *fileMenu = menuBar()->addMenu("&File");

        fileMenu->addAction("Open video...", this, [this]() {
            openVideo();
        });

        fileMenu->addSeparator();

        fileMenu->addAction("Exit", this, [this]() {
            close();
        });

        statusBar()->showMessage("Qt6 + FFmpeg frame grab ready");
    }

private:
    void openVideo()
    {
        const QString fileName = QFileDialog::getOpenFileName(
            this,
            "Open video",
            QDir::homePath(),
            "Video files (*.mkv *.mp4 *.avi *.mov *.webm *.m4v *.ts *.mpg *.mpeg);;All files (*.*)"
        );

        if (fileName.isEmpty()) {
            return;
        }

        statusBar()->showMessage("Probing video...");

        QString info = probeVideoFile(fileName);

        double grabSeconds = 10.0;

        AVFormatContext *formatContext = nullptr;
        QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

        if (avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr) == 0) {
            if (avformat_find_stream_info(formatContext, nullptr) == 0) {
                if (formatContext->duration != AV_NOPTS_VALUE && formatContext->duration > 0) {
                    const double durationSeconds =
                        formatContext->duration / static_cast<double>(AV_TIME_BASE);
                    grabSeconds = durationSeconds / 2.0;
                }
            }
            avformat_close_input(&formatContext);
        }

        QImage frameImage;
        QString frameLog;

        statusBar()->showMessage("Grabbing frame...");

        if (grabFrameAt(fileName, grabSeconds, frameImage, frameLog)) {
            const QPixmap pixmap = QPixmap::fromImage(frameImage);
            preview->setPixmap(
                pixmap.scaled(
                    preview->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                )
            );
        } else {
            preview->setText("Frame grab failed");
        }

        output->setText(info + "\n\n" + frameLog);
        statusBar()->showMessage("Done");
    }

private:
    QTextEdit *output = nullptr;
    QLabel *preview = nullptr;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}