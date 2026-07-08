// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaInfoReader.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QRegularExpression>
#include <QStringList>

namespace
{
    using MediaInfoHandle = void *;

    using MediaInfo_New_F = MediaInfoHandle (*)();
    using MediaInfo_Delete_F = void (*)(MediaInfoHandle);
    using MediaInfo_Open_F = size_t (*)(MediaInfoHandle, const wchar_t *);
    using MediaInfo_Close_F = size_t (*)(MediaInfoHandle);
    using MediaInfo_Get_F = const wchar_t *(*)(MediaInfoHandle, size_t, size_t, const wchar_t *, size_t, size_t);
    using MediaInfo_Option_F = const wchar_t *(*)(MediaInfoHandle, const wchar_t *, const wchar_t *);
    using MediaInfo_Count_Get_F = size_t (*)(MediaInfoHandle, size_t, size_t);

    constexpr size_t Stream_General = 0;
    constexpr size_t Stream_Video = 1;
    constexpr size_t Stream_Audio = 2;
    constexpr size_t Stream_Text = 3;

    constexpr size_t Info_Name = 0;
    constexpr size_t Info_Text = 1;

    struct MediaInfoApi
    {
        QLibrary library;
        MediaInfo_New_F New = nullptr;
        MediaInfo_Delete_F Delete = nullptr;
        MediaInfo_Open_F Open = nullptr;
        MediaInfo_Close_F Close = nullptr;
        MediaInfo_Get_F Get = nullptr;
        MediaInfo_Option_F Option = nullptr;
        MediaInfo_Count_Get_F Count_Get = nullptr;

        bool isLoaded() const
        {
            return library.isLoaded() && New && Delete && Open && Close && Get;
        }
    };

    QStringList candidateLibraryNames()
    {
        QStringList names;

        const QString appDir = QCoreApplication::applicationDirPath();
        if (!appDir.isEmpty()) {
#ifdef Q_OS_WIN
            names << QDir(appDir).filePath("MediaInfo.dll");
            names << QDir(appDir).filePath("MediaInfo64.dll");
            names << QDir(appDir).filePath("libmediainfo.dll");
#else
            names << QDir(appDir).filePath("libmediainfo.so");
            names << QDir(appDir).filePath("libmediainfo.so.0");
#endif
        }

#ifdef Q_OS_WIN
        names << "MediaInfo.dll";
        names << "MediaInfo64.dll";
        names << "libmediainfo.dll";
#else
        names << "mediainfo";
        names << "libmediainfo.so";
        names << "libmediainfo.so.0";
#endif

        return names;
    }

    bool loadApi(MediaInfoApi &api)
    {
        const QStringList candidates = candidateLibraryNames();
        for (const QString &candidate : candidates) {
            api.library.setFileName(candidate);
            if (!api.library.load()) {
                continue;
            }

            api.New = reinterpret_cast<MediaInfo_New_F>(api.library.resolve("MediaInfo_New"));
            api.Delete = reinterpret_cast<MediaInfo_Delete_F>(api.library.resolve("MediaInfo_Delete"));
            api.Open = reinterpret_cast<MediaInfo_Open_F>(api.library.resolve("MediaInfo_Open"));
            api.Close = reinterpret_cast<MediaInfo_Close_F>(api.library.resolve("MediaInfo_Close"));
            api.Get = reinterpret_cast<MediaInfo_Get_F>(api.library.resolve("MediaInfo_Get"));
            api.Option = reinterpret_cast<MediaInfo_Option_F>(api.library.resolve("MediaInfo_Option"));
            api.Count_Get = reinterpret_cast<MediaInfo_Count_Get_F>(api.library.resolve("MediaInfo_Count_Get"));

            if (api.isLoaded()) {
                return true;
            }

            api.library.unload();
            api.New = nullptr;
            api.Delete = nullptr;
            api.Open = nullptr;
            api.Close = nullptr;
            api.Get = nullptr;
            api.Option = nullptr;
            api.Count_Get = nullptr;
        }

        return false;
    }

    QString wptrToQString(const wchar_t *text)
    {
        if (!text || !*text) {
            return QString();
        }

        return QString::fromWCharArray(text).trimmed();
    }

    QString getValue(
        const MediaInfoApi &api,
        MediaInfoHandle handle,
        size_t streamKind,
        size_t streamNumber,
        const wchar_t *parameter
    )
    {
        if (!api.Get || !handle || !parameter) {
            return QString();
        }

        return wptrToQString(api.Get(handle, streamKind, streamNumber, parameter, Info_Text, Info_Name));
    }

    QString firstNonEmpty(std::initializer_list<QString> values)
    {
        for (const QString &value : values) {
            const QString trimmed = value.trimmed();
            if (!trimmed.isEmpty()) {
                return trimmed;
            }
        }

        return QString();
    }

    int intValue(const QString &value)
    {
        bool ok = false;
        const int parsed = value.trimmed().toInt(&ok);
        return ok ? parsed : 0;
    }

    qint64 int64Value(const QString &value)
    {
        bool ok = false;
        const qint64 parsed = value.trimmed().toLongLong(&ok);
        return ok ? parsed : 0;
    }

    QString cleanUnitString(QString value)
    {
        value = value.trimmed();
        value.replace("Kbps", "kb/s", Qt::CaseInsensitive);
        value.replace("kbps", "kb/s", Qt::CaseInsensitive);
        value.replace("KHz", "kHz", Qt::CaseInsensitive);
        return value;
    }

    QString formatBytes(qint64 bytes)
    {
        if (bytes <= 0) {
            return QString();
        }

        const double gib = bytes / 1024.0 / 1024.0 / 1024.0;
        const double mib = bytes / 1024.0 / 1024.0;

        if (gib >= 1.0) {
            return QString::number(gib, 'f', 2) + " GiB";
        }

        return QString::number(mib, 'f', 2) + " MiB";
    }

    QString formatBitrateFromBps(qint64 bitsPerSecond)
    {
        if (bitsPerSecond <= 0) {
            return QString();
        }

        return QString::number((bitsPerSecond + 500) / 1000) + " kb/s";
    }

    QString rawOrFormattedBitrate(const QString &raw, const QString &formatted)
    {
        const qint64 bps = int64Value(raw);
        if (bps > 0) {
            return formatBitrateFromBps(bps);
        }

        return cleanUnitString(formatted);
    }

    bool containsDigit(const QString &value)
    {
        for (const QChar ch : value) {
            if (ch.isDigit()) {
                return true;
            }
        }

        return false;
    }

    QString streamBitrateText(
        const MediaInfoApi &api,
        MediaInfoHandle handle,
        size_t streamKind,
        size_t index
    )
    {
        const QString raw = firstNonEmpty({
            getValue(api, handle, streamKind, index, L"BitRate"),
            getValue(api, handle, streamKind, index, L"BitRate_Nominal"),
            getValue(api, handle, streamKind, index, L"BitRate_Maximum")
        });

        const QString formatted = firstNonEmpty({
            getValue(api, handle, streamKind, index, L"BitRate/String"),
            getValue(api, handle, streamKind, index, L"BitRate_Nominal/String"),
            getValue(api, handle, streamKind, index, L"BitRate_Maximum/String")
        });

        const QString text = rawOrFormattedBitrate(raw, formatted).trimmed();
        if (!containsDigit(text)) {
            return QString();
        }

        return text;
    }

    QString samplingRateText(const QString &raw, const QString &formatted)
    {
        const int hz = intValue(raw);
        if (hz > 0) {
            return QString::number(hz) + " Hz";
        }

        return cleanUnitString(formatted);
    }

    QString channelsText(const QString &raw, const QString &formatted)
    {
        const int channels = intValue(raw);
        if (channels > 0) {
            return QString("%1 channel%2").arg(channels).arg(channels == 1 ? "" : "s");
        }

        return formatted.trimmed();
    }

    QString languageText(const MediaInfoApi &api, MediaInfoHandle handle, size_t streamKind, size_t index)
    {
        return firstNonEmpty({
            getValue(api, handle, streamKind, index, L"Language/String3"),
            getValue(api, handle, streamKind, index, L"Language"),
            getValue(api, handle, streamKind, index, L"Language/String")
        });
    }

    QString compactLanguageCode(QString value)
    {
        value = value.trimmed();
        if (value.isEmpty()) {
            return QString();
        }

        // MediaInfo can return values such as "eng English" when raw language
        // output is enabled. For the subtitle summary we only want the compact
        // language code, otherwise long multi-subtitle headers become unreadable.
        static const QRegularExpression codeTokenRe(QStringLiteral("^[A-Za-z]{2,3}$"));
        const QString normalized = value.replace('_', '-');
        const QStringList tokens = normalized.split(QRegularExpression(QStringLiteral("[\\s,;/()\\[\\]{}]+")), Qt::SkipEmptyParts);

        for (QString token : tokens) {
            token = token.trimmed();
            if (token.endsWith(':') || token.endsWith('.')) {
                token.chop(1);
            }

            const int dash = token.indexOf('-');
            if (dash > 0) {
                const QString prefix = token.left(dash);
                if (codeTokenRe.match(prefix).hasMatch()) {
                    return prefix.toLower();
                }
            }

            if (codeTokenRe.match(token).hasMatch()) {
                return token.toLower();
            }
        }

        return value;
    }

    QString subtitleLanguageCode(const MediaInfoApi &api, MediaInfoHandle handle, size_t index)
    {
        return compactLanguageCode(firstNonEmpty({
            getValue(api, handle, Stream_Text, index, L"Language/String3"),
            getValue(api, handle, Stream_Text, index, L"Language/String2"),
            getValue(api, handle, Stream_Text, index, L"Language"),
            getValue(api, handle, Stream_Text, index, L"Language/String")
        }));
    }

    QString streamTitleText(const MediaInfoApi &api, MediaInfoHandle handle, size_t streamKind, size_t index)
    {
        return firstNonEmpty({
            getValue(api, handle, streamKind, index, L"Title"),
            getValue(api, handle, streamKind, index, L"Menu_Forced")
        });
    }

    size_t streamCountByGeneralField(
        const MediaInfoApi &api,
        MediaInfoHandle handle,
        const wchar_t *parameter
    )
    {
        const int count = intValue(getValue(api, handle, Stream_General, 0, parameter));
        return count > 0 ? static_cast<size_t>(count) : 0;
    }

    size_t streamCount(
        const MediaInfoApi &api,
        MediaInfoHandle handle,
        size_t streamKind,
        const wchar_t *generalCountParameter
    )
    {
        if (api.Count_Get) {
            const size_t count = api.Count_Get(handle, streamKind, static_cast<size_t>(-1));
            if (count > 0 && count < 256) {
                return count;
            }
        }

        return streamCountByGeneralField(api, handle, generalCountParameter);
    }

    QString fallbackVideoLine(const VideoInfo &info)
    {
        if (!info.ok) {
            return QString();
        }

        QStringList parts;

        if (!info.codecShortName.isEmpty()) {
            parts << info.codecShortName.toLower();
        }

        if (!info.pixelFormatName.isEmpty() && info.pixelFormatName != "unknown") {
            parts << info.pixelFormatName;
        }

        if (info.width > 0 && info.height > 0) {
            parts << QString("%1x%2").arg(info.width).arg(info.height);
        }

        if (info.avgFps > 0.0) {
            parts << QString("%1 fps").arg(QString::number(info.avgFps, 'f', 2));
        }

        if (parts.isEmpty()) {
            return QString();
        }

        return "Video: " + parts.join(", ");
    }

    QString buildAudioLine(const MediaInfoApi &api, MediaInfoHandle handle, size_t index)
    {
        QStringList parts;

        const QString format = firstNonEmpty({
            getValue(api, handle, Stream_Audio, index, L"Format"),
            getValue(api, handle, Stream_Audio, index, L"CodecID")
        });
        if (!format.isEmpty()) {
            parts << format.toLower();
        }

        const QString bitrate = streamBitrateText(api, handle, Stream_Audio, index);
        if (!bitrate.isEmpty()) {
            parts << bitrate;
        }

        const QString samplingRate = samplingRateText(
            getValue(api, handle, Stream_Audio, index, L"SamplingRate"),
            getValue(api, handle, Stream_Audio, index, L"SamplingRate/String")
        );
        if (!samplingRate.isEmpty()) {
            parts << samplingRate;
        }

        const QString channels = channelsText(
            getValue(api, handle, Stream_Audio, index, L"Channels"),
            firstNonEmpty({
                getValue(api, handle, Stream_Audio, index, L"Channel(s)/String"),
                getValue(api, handle, Stream_Audio, index, L"Channels/String")
            })
        );
        if (!channels.isEmpty()) {
            parts << channels;
        }

        const QString bitDepth = firstNonEmpty({
            getValue(api, handle, Stream_Audio, index, L"BitDepth"),
            getValue(api, handle, Stream_Audio, index, L"BitDepth/String")
        });
        if (!bitDepth.isEmpty()) {
            if (intValue(bitDepth) > 0) {
                parts << ("s" + QString::number(intValue(bitDepth)));
            } else {
                parts << bitDepth;
            }
        }

        const QString language = languageText(api, handle, Stream_Audio, index);
        QString line = "Audio: " + parts.join(", ");
        if (!language.isEmpty()) {
            line += " (" + language + ")";
        }

        return line.trimmed();
    }

    QString buildVideoLine(const MediaInfoApi &api, MediaInfoHandle handle, const VideoInfo &fallbackInfo)
    {
        QStringList parts;

        const QString videoFormat = firstNonEmpty({
            getValue(api, handle, Stream_Video, 0, L"Format"),
            getValue(api, handle, Stream_Video, 0, L"CodecID"),
            fallbackInfo.codecShortName
        });

        if (!videoFormat.isEmpty()) {
            QString codec = videoFormat.toLower();
            const QString videoProfile = getValue(api, handle, Stream_Video, 0, L"Format_Profile");
            if (!videoProfile.isEmpty()) {
                codec += " " + videoProfile;
            }
            parts << codec;
        }

        const QString videoBitrate = streamBitrateText(api, handle, Stream_Video, 0);
        if (!videoBitrate.isEmpty()) {
            parts << videoBitrate;
        }

        if (!fallbackInfo.pixelFormatName.isEmpty() && fallbackInfo.pixelFormatName != "unknown") {
            parts << fallbackInfo.pixelFormatName;
        }

        const QString width = firstNonEmpty({
            getValue(api, handle, Stream_Video, 0, L"Width"),
            getValue(api, handle, Stream_Video, 0, L"Width/String")
        });
        const QString height = firstNonEmpty({
            getValue(api, handle, Stream_Video, 0, L"Height"),
            getValue(api, handle, Stream_Video, 0, L"Height/String")
        });

        if (!width.isEmpty() && !height.isEmpty()) {
            parts << QString("%1x%2").arg(width, height);
        } else if (fallbackInfo.width > 0 && fallbackInfo.height > 0) {
            parts << QString("%1x%2").arg(fallbackInfo.width).arg(fallbackInfo.height);
        }

        const QString fps = firstNonEmpty({
            getValue(api, handle, Stream_Video, 0, L"FrameRate"),
            getValue(api, handle, Stream_Video, 0, L"FrameRate/String")
        });
        if (!fps.isEmpty()) {
            parts << (fps.contains("fps", Qt::CaseInsensitive) ? fps : fps + " fps");
        } else if (fallbackInfo.avgFps > 0.0) {
            parts << QString("%1 fps").arg(QString::number(fallbackInfo.avgFps, 'f', 2));
        }

        if (parts.isEmpty()) {
            return fallbackVideoLine(fallbackInfo);
        }

        return "Video: " + parts.join(", ");
    }

    QString buildSubtitleLine(const QStringList &items)
    {
        if (items.isEmpty()) {
            return QString();
        }

        return "Subtitles: " + items.join("; ");
    }

    QString textStreamDescription(const MediaInfoApi &api, MediaInfoHandle handle, size_t index)
    {
        const QString language = subtitleLanguageCode(api, handle, index);
        if (!language.isEmpty()) {
            return language;
        }

        const QString format = getValue(api, handle, Stream_Text, index, L"Format").trimmed();
        if (!format.isEmpty()) {
            return format.toLower();
        }

        return QString();
    }
}

bool MediaInfoReader::isAvailable()
{
    MediaInfoApi api;
    return loadApi(api);
}

QString MediaInfoReader::fallbackSummary(const QString &fileName, const VideoInfo &info)
{
    QStringList result;

    result << "File: " + QFileInfo(fileName).fileName();

    QStringList sizeParts;
    const QFileInfo fileInfo(fileName);
    if (fileInfo.exists()) {
        const qint64 bytes = fileInfo.size();
        const QString formattedSize = formatBytes(bytes);
        if (!formattedSize.isEmpty()) {
            sizeParts << QString("Size: %1 bytes (%2)").arg(bytes).arg(formattedSize);
        } else {
            sizeParts << QString("Size: %1 bytes").arg(bytes);
        }
    }

    if (info.durationSeconds > 0.0) {
        sizeParts << "duration: " + durationToTimeText(info.durationSeconds);
    }

    if (info.bitRate > 0) {
        sizeParts << "avg.bitrate: " + formatBitrateFromBps(info.bitRate);
    }

    if (!sizeParts.isEmpty()) {
        result << sizeParts.join(", ");
    }

    const QString video = fallbackVideoLine(info);
    if (!video.isEmpty()) {
        result << video;
    }

    return result.join("\n");
}

QString MediaInfoReader::summaryForFile(const QString &fileName, const VideoInfo &fallbackInfo)
{
    MediaInfoApi api;
    if (!loadApi(api)) {
        return fallbackSummary(fileName, fallbackInfo);
    }

    MediaInfoHandle handle = api.New();
    if (!handle) {
        return fallbackSummary(fileName, fallbackInfo);
    }

    if (api.Option) {
        api.Option(handle, L"Language", L"raw");
        api.Option(handle, L"Complete", L"1");
    }

    const std::wstring nativeFileName = QDir::toNativeSeparators(fileName).toStdWString();
    if (api.Open(handle, nativeFileName.c_str()) == 0) {
        api.Delete(handle);
        return fallbackSummary(fileName, fallbackInfo);
    }

    const QFileInfo fileInfo(fileName);
    QStringList result;

    result << "File: " + fileInfo.fileName();

    const qint64 rawFileSize = firstNonEmpty({
        getValue(api, handle, Stream_General, 0, L"FileSize"),
        QString::number(fileInfo.size())
    }).toLongLong();

    const QString fileSizeString = firstNonEmpty({
        getValue(api, handle, Stream_General, 0, L"FileSize/String"),
        formatBytes(rawFileSize)
    });

    QStringList sizeLineParts;
    if (rawFileSize > 0 && !fileSizeString.isEmpty()) {
        sizeLineParts << QString("Size: %1 bytes (%2)").arg(rawFileSize).arg(fileSizeString);
    } else if (rawFileSize > 0) {
        sizeLineParts << QString("Size: %1 bytes").arg(rawFileSize);
    }

    const QString duration = firstNonEmpty({
        getValue(api, handle, Stream_General, 0, L"Duration/String3"),
        getValue(api, handle, Stream_General, 0, L"Duration/String"),
        fallbackInfo.durationSeconds > 0.0 ? durationToTimeText(fallbackInfo.durationSeconds) : QString()
    });
    if (!duration.isEmpty()) {
        sizeLineParts << "duration: " + duration;
    }

    const QString overallBitrate = rawOrFormattedBitrate(
        getValue(api, handle, Stream_General, 0, L"OverallBitRate"),
        getValue(api, handle, Stream_General, 0, L"OverallBitRate/String")
    );
    if (!overallBitrate.isEmpty()) {
        sizeLineParts << "avg.bitrate: " + overallBitrate;
    } else if (fallbackInfo.bitRate > 0) {
        sizeLineParts << "avg.bitrate: " + formatBitrateFromBps(fallbackInfo.bitRate);
    }

    if (!sizeLineParts.isEmpty()) {
        result << sizeLineParts.join(", ");
    }

    const size_t audioCount = streamCount(api, handle, Stream_Audio, L"AudioCount");
    for (size_t i = 0; i < audioCount; ++i) {
        const QString audioLine = buildAudioLine(api, handle, i);
        if (!audioLine.isEmpty() && audioLine != "Audio:") {
            result << audioLine;
        }
    }

    const QString videoLine = buildVideoLine(api, handle, fallbackInfo);
    if (!videoLine.isEmpty()) {
        result << videoLine;
    }

    const size_t subtitleCount = streamCount(api, handle, Stream_Text, L"TextCount");
    QStringList subtitleItems;
    for (size_t i = 0; i < subtitleCount; ++i) {
        const QString item = textStreamDescription(api, handle, i);
        if (!item.isEmpty() && !subtitleItems.contains(item, Qt::CaseInsensitive)) {
            subtitleItems << item;
        }
    }

    const QString subtitles = buildSubtitleLine(subtitleItems);
    if (!subtitles.isEmpty()) {
        result << subtitles;
    }


    if (api.Close) {
        api.Close(handle);
    }
    api.Delete(handle);

    if (result.isEmpty()) {
        return fallbackSummary(fileName, fallbackInfo);
    }

    return result.join("\n");
}
