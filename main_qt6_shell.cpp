#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPixmap>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVector>
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
        return "neznĂˇmĂˇ";
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

static QString rationalToFps(AVRational rate)
{
    if (rate.num <= 0 || rate.den <= 0) {
        return "neznĂˇmĂ©";
    }

    return QString::number(av_q2d(rate), 'f', 3);
}

static double videoDurationSeconds(const QString &fileName)
{
    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        return 0.0;
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        avformat_close_input(&formatContext);
        return 0.0;
    }

    double seconds = 0.0;

    if (formatContext->duration != AV_NOPTS_VALUE && formatContext->duration > 0) {
        seconds = formatContext->duration / static_cast<double>(AV_TIME_BASE);
    }

    avformat_close_input(&formatContext);
    return seconds;
}

static QString probeVideoFile(const QString &fileName)
{
    QByteArray filePath = QDir::toNativeSeparators(fileName).toUtf8();

    AVFormatContext *formatContext = nullptr;

    int ret = avformat_open_input(&formatContext, filePath.constData(), nullptr, nullptr);
    if (ret < 0) {
        return QString("NepodaĹ™ilo se otevĹ™Ă­t soubor:\n%1\n\nFFmpeg chyba:\n%2")
            .arg(fileName, ffmpegErrorText(ret));
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        avformat_close_input(&formatContext);
        return QString("NepodaĹ™ilo se naÄŤĂ­st informace o streamu:\n%1")
            .arg(ffmpegErrorText(ret));
    }

    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (ret < 0) {
        avformat_close_input(&formatContext);
        return "Soubor neobsahuje pouĹľitelnĂ˝ video stream.";
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

    QString pixelFormat = "neznĂˇmĂ˝";
    if (codecParams->format >= 0) {
        const char *pixName = av_get_pix_fmt_name(static_cast<AVPixelFormat>(codecParams->format));
        if (pixName) {
            pixelFormat = QString::fromUtf8(pixName);
        }
    }

    QString container = "neznĂˇmĂ˝";
    if (formatContext->iformat && formatContext->iformat->long_name) {
        container = QString::fromUtf8(formatContext->iformat->long_name);
    }

    QString bitRate = "neznĂˇmĂ˝";
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
    text += QString("RozliĹˇenĂ­: %1 Ă— %2\n").arg(codecParams->width).arg(codecParams->height);
    text += QString("Pixel format: %1\n").arg(pixelFormat);
    text += QString("FPS avg: %1\n").arg(rationalToFps(videoStream->avg_frame_rate));
    text += QString("FPS real: %1\n").arg(rationalToFps(videoStream->r_frame_rate));
    text += QString("DĂ©lka: %1\n").arg(formatDuration(formatContext->duration));
    text += QString("Bitrate: %1\n").arg(bitRate);
    text += QString("PoÄŤet streamĹŻ: %1\n\n").arg(formatContext->nb_streams);

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
        outLog = "Frame nebyl dekĂłdovĂˇn.";
        return false;
    }

    outLog = QString("Frame grab OK | poĹľadovanĂ˝ ÄŤas: %1 | dekĂłdovanĂ˝ ÄŤas: %2")
        .arg(secondsToTimeText(seconds))
        .arg(decodedTime >= 0.0 ? secondsToTimeText(decodedTime) : QString("neznĂˇmĂ˝"));

    return true;
}

static QVector<double> makeEvenTimes(double durationSeconds, int count)
{
    QVector<double> result;

    if (durationSeconds <= 0.0 || count <= 0) {
        return result;
    }

    const double startMargin = durationSeconds * 0.07;
    const double endMargin = durationSeconds * 0.07;

    double start = startMargin;
    double end = durationSeconds - endMargin;

    if (end <= start) {
        start = 0.0;
        end = durationSeconds;
    }

    const double span = end - start;

    for (int i = 0; i < count; ++i) {
        const double t = start + ((i + 1) / static_cast<double>(count + 1)) * span;
        result.append(t);
    }

    return result;
}

static QImage composeContactSheet(
    const QVector<QImage> &frames,
    const QVector<double> &times,
    int columns,
    int rows,
    int thumbWidth,
    int gap,
    const QString &title
)
{
    if (frames.isEmpty()) {
        return QImage();
    }

    const QSize firstSize = frames.first().size();
    const double aspect = firstSize.height() > 0
        ? firstSize.width() / static_cast<double>(firstSize.height())
        : 16.0 / 9.0;

    const int thumbHeight = static_cast<int>(thumbWidth / aspect);

    const int margin = 18;
    const int headerHeight = 58;
    const int footerHeight = 18;

    const int sheetWidth =
        margin * 2 + columns * thumbWidth + (columns - 1) * gap;

    const int sheetHeight =
        margin * 2 + headerHeight + footerHeight + rows * thumbHeight + (rows - 1) * gap;

    QImage sheet(sheetWidth, sheetHeight, QImage::Format_ARGB32);
    sheet.fill(QColor(20, 20, 20));

    QPainter painter(&sheet);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);

    QFont smallFont;
    smallFont.setPointSize(10);

    painter.setFont(titleFont);
    painter.setPen(Qt::white);

    QRect titleRect(margin, margin, sheetWidth - margin * 2, 28);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, title);

    painter.setFont(smallFont);
    painter.setPen(QColor(210, 210, 210));

    QString subtitle = QString("%1 snĂ­mkĹŻ | %2 Ă— %3 | thumb %4 px")
        .arg(frames.size())
        .arg(columns)
        .arg(rows)
        .arg(thumbWidth);

    QRect subtitleRect(margin, margin + 30, sheetWidth - margin * 2, 22);
    painter.drawText(subtitleRect, Qt::AlignLeft | Qt::AlignVCenter, subtitle);

    const int gridTop = margin + headerHeight;

    for (int i = 0; i < frames.size() && i < columns * rows; ++i) {
        const int col = i % columns;
        const int row = i / columns;

        const int x = margin + col * (thumbWidth + gap);
        const int y = gridTop + row * (thumbHeight + gap);

        QRect targetRect(x, y, thumbWidth, thumbHeight);

        QImage thumb = frames[i].scaled(
            thumbWidth,
            thumbHeight,
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );

        const int cropX = qMax(0, (thumb.width() - thumbWidth) / 2);
        const int cropY = qMax(0, (thumb.height() - thumbHeight) / 2);
        thumb = thumb.copy(cropX, cropY, thumbWidth, thumbHeight);

        painter.drawImage(targetRect, thumb);

        painter.setPen(QColor(70, 70, 70));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(targetRect.adjusted(0, 0, -1, -1));

        QString stamp = i < times.size() ? secondsToTimeText(times[i]) : QString();
        if (!stamp.isEmpty()) {
            QRect stampRect(x + 6, y + thumbHeight - 28, 92, 22);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(0, 0, 0, 170));
            painter.drawRoundedRect(stampRect, 4, 4);

            painter.setPen(Qt::white);
            painter.setFont(smallFont);
            painter.drawText(stampRect, Qt::AlignCenter, stamp);
        }
    }

    painter.end();
    return sheet;
}

class MainWindow : public QMainWindow
{
public:
    MainWindow()
    {
        setWindowTitle("Thumbnail me 3.5 - Qt6 + FFmpeg contact sheet prototype");
        resize(1200, 780);

        auto *splitter = new QSplitter(this);

        output = new QTextEdit(splitter);
        output->setReadOnly(true);
        output->setText(
            "Thumbnail me Qt6 shell\n\n"
            "Milestone 4:\n"
            "- Qt6 Widgets bÄ›ĹľĂ­\n"
            "- FFmpeg knihovny jsou linkovanĂ© pĹ™Ă­mo\n"
            "- File â†’ Open video naÄŤte metadata\n"
            "- Aplikace vytvoĹ™Ă­ prvnĂ­ 4Ă—4 contact sheet\n\n"
            "ZatĂ­m stĂˇle nejde o finĂˇlnĂ­ UI, jen bezpeÄŤnĂ˝ portovacĂ­ checkpoint."
        );

        preview = new QLabel(splitter);
        preview->setAlignment(Qt::AlignCenter);
        preview->setText("Contact sheet preview");
        preview->setMinimumSize(640, 480);
        preview->setStyleSheet("QLabel { background: #202020; color: #dddddd; }");

        splitter->addWidget(output);
        splitter->addWidget(preview);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 3);

        setCentralWidget(splitter);

        auto *fileMenu = menuBar()->addMenu("&File");

        fileMenu->addAction("Open video...", this, [this]() {
            openVideo();
        });

        fileMenu->addAction("Save contact sheet as...", this, [this]() {
            saveContactSheet();
        });

        fileMenu->addSeparator();

        fileMenu->addAction("Exit", this, [this]() {
            close();
        });

        statusBar()->showMessage("Qt6 + FFmpeg contact sheet prototype ready");
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

        const int columns = 4;
        const int rows = 4;
        const int count = columns * rows;
        const int thumbWidth = 320;
        const int gap = 8;

        statusBar()->showMessage("Probing video...");
        QString info = probeVideoFile(fileName);

        const double duration = videoDurationSeconds(fileName);
        if (duration <= 0.0) {
            output->setText(info + "\n\nNepodaĹ™ilo se zjistit dĂ©lku videa.");
            return;
        }

        QVector<double> times = makeEvenTimes(duration, count);
        QVector<QImage> frames;

        QString log;
        log += info;
        log += "\n\nGeneruji 4Ă—4 contact sheet...\n";

        for (int i = 0; i < times.size(); ++i) {
            QApplication::processEvents();

            statusBar()->showMessage(
                QString("DekĂłduji snĂ­mek %1/%2...").arg(i + 1).arg(times.size())
            );

            QImage frame;
            QString frameLog;

            if (grabFrameAt(fileName, times[i], frame, frameLog)) {
                frames.append(frame);
                log += QString("[%1/%2] %3 OK\n")
                    .arg(i + 1)
                    .arg(times.size())
                    .arg(secondsToTimeText(times[i]));
            } else {
                log += QString("[%1/%2] %3 FAILED: %4\n")
                    .arg(i + 1)
                    .arg(times.size())
                    .arg(secondsToTimeText(times[i]), frameLog);
            }
        }

        if (frames.isEmpty()) {
            output->setText(log + "\nNepodaĹ™ilo se dekĂłdovat ĹľĂˇdnĂ˝ snĂ­mek.");
            preview->setText("Contact sheet failed");
            statusBar()->showMessage("Failed");
            return;
        }

        const QString title = QFileInfo(fileName).fileName();

        currentSheet = composeContactSheet(
            frames,
            times,
            columns,
            rows,
            thumbWidth,
            gap,
            title
        );

        if (currentSheet.isNull()) {
            output->setText(log + "\nNepodaĹ™ilo se vytvoĹ™it contact sheet.");
            preview->setText("Contact sheet failed");
            statusBar()->showMessage("Failed");
            return;
        }

        showCurrentSheet();

        log += "\nContact sheet OK\n";
        log += QString("VĂ˝slednĂˇ velikost: %1 Ă— %2 px\n")
            .arg(currentSheet.width())
            .arg(currentSheet.height());

        output->setText(log);
        statusBar()->showMessage("Contact sheet ready");
    }

    void saveContactSheet()
    {
        if (currentSheet.isNull()) {
            statusBar()->showMessage("No contact sheet to save");
            return;
        }

        const QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save contact sheet",
            QDir::homePath() + "/contact_sheet.jpg",
            "JPEG image (*.jpg *.jpeg);;PNG image (*.png)"
        );

        if (fileName.isEmpty()) {
            return;
        }

        if (currentSheet.save(fileName, nullptr, 95)) {
            statusBar()->showMessage("Saved: " + fileName);
        } else {
            statusBar()->showMessage("Save failed");
        }
    }

    void showCurrentSheet()
    {
        if (currentSheet.isNull()) {
            return;
        }

        const QPixmap pixmap = QPixmap::fromImage(currentSheet);
        preview->setPixmap(
            pixmap.scaled(
                preview->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }

    void resizeEvent(QResizeEvent *event) override
    {
        QMainWindow::resizeEvent(event);
        showCurrentSheet();
    }

private:
    QTextEdit *output = nullptr;
    QLabel *preview = nullptr;
    QImage currentSheet;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}


