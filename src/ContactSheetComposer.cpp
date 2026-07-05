// SPDX-License-Identifier: GPL-2.0-or-later
#include "ContactSheetComposer.h"

#include "VideoTypes.h"

#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QRandomGenerator>
#include <QRect>
#include <QtGlobal>

#include <algorithm>

QVector<double> ContactSheetComposer::makeEvenTimes(
    double durationSeconds,
    int count,
    double startMarginPercent,
    double endMarginPercent,
    bool randomizePositions,
    double randomizePercent,
    bool useSpecificTimestep,
    double timestepSeconds
)
{
    QVector<double> result;

    if (durationSeconds <= 0.0 || count <= 0) {
        return result;
    }

    const double startPercent = qBound(0.0, startMarginPercent, 45.0);
    const double endPercent = qBound(0.0, endMarginPercent, 45.0);

    double start = durationSeconds * startPercent / 100.0;
    double end = durationSeconds - durationSeconds * endPercent / 100.0;

    if (end <= start) {
        start = 0.0;
        end = durationSeconds;
    }

    const double span = end - start;
    if (span <= 0.0) {
        result.append(durationSeconds * 0.5);
        return result;
    }

    if (useSpecificTimestep) {
        const double step = qBound(1.0, timestepSeconds, 3600.0);
        for (double t = start; t <= end && result.size() < count; t += step) {
            result.append(qBound(0.0, t, durationSeconds));
        }

        if (result.isEmpty()) {
            result.append(qBound(0.0, start + span * 0.5, durationSeconds));
        }

        return result;
    }

    const double step = span / static_cast<double>(count + 1);
    const double jitterLimit = randomizePositions
        ? step * qBound(0.0, randomizePercent, 50.0) / 100.0
        : 0.0;

    for (int i = 0; i < count; ++i) {
        double t = start + ((i + 1) / static_cast<double>(count + 1)) * span;

        if (jitterLimit > 0.0) {
            const double randomUnit = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
            t += randomUnit * jitterLimit;
            t = qBound(start, t, end);
        }

        result.append(qBound(0.0, t, durationSeconds));
    }

    std::sort(result.begin(), result.end());
    return result;
}

QImage ContactSheetComposer::compose(
    const QVector<QImage> &frames,
    const QVector<double> &times,
    int columns,
    int rows,
    int thumbWidth,
    int gap,
    const QString &title,
    const QString &headerInfo,
    bool drawTimestamps,
    const QFont &textFont,
    bool drawInfoText,
    const QString &infoTextPosition,
    const QString &timestampPosition,
    bool drawAdditionalTitle,
    const QString &additionalTitle,
    const QColor &backgroundColor,
    const QColor &titleTextColor,
    const QColor &infoTextColor,
    const QColor &countTextColor,
    const QColor &timestampTextColor,
    const QColor &timestampBackgroundColor,
    bool drawTileBorder,
    const QColor &tileBorderColor
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
    const int footerHeight = 18;

    QFont baseFont = textFont;
    if (baseFont.pointSize() <= 0) {
        baseFont.setPointSize(10);
    }

    QFont titleFont = baseFont;
    titleFont.setPointSize(baseFont.pointSize() + 6);
    titleFont.setBold(true);

    QFont smallFont = baseFont;
    smallFont.setPointSize(baseFont.pointSize());

    QFontMetrics titleMetrics(titleFont);
    QFontMetrics smallMetrics(smallFont);

    const QString normalizedInfoPosition = infoTextPosition.trimmed().toLower();
    const QString normalizedTimestampPosition = timestampPosition.trimmed().toLower();

    const bool textBlockAtBottom = normalizedInfoPosition.startsWith("lower") || normalizedInfoPosition == "bottom";
    const bool textBlockAtRight = normalizedInfoPosition.endsWith("right");

    const bool timestampAtTop = normalizedTimestampPosition.startsWith("upper") || normalizedTimestampPosition == "top";
    const bool timestampAtRight = normalizedTimestampPosition.endsWith("right");

    QStringList infoLines = drawInfoText
        ? headerInfo.split('\n', Qt::SkipEmptyParts)
        : QStringList();

    if (drawAdditionalTitle && !additionalTitle.trimmed().isEmpty()) {
        infoLines << additionalTitle.trimmed();
    }

    const int titleHeight = titleMetrics.height() + 6;
    const int lineHeight = smallMetrics.height() + 3;
    const int infoLinesHeight = infoLines.isEmpty() ? 0 : infoLines.size() * lineHeight + 4;
    const int subtitleHeight = lineHeight + 6;
    const int textBlockHeight = titleHeight + infoLinesHeight + subtitleHeight + 12;

    const int sheetWidth = margin * 2 + columns * thumbWidth + (columns - 1) * gap;
    const int gridHeight = rows * thumbHeight + (rows - 1) * gap;
    const int sheetHeight = margin * 2 + textBlockHeight + footerHeight + gridHeight;

    QImage sheet(sheetWidth, sheetHeight, QImage::Format_ARGB32);
    sheet.fill(backgroundColor.isValid() ? backgroundColor : QColor(20, 20, 20));

    QPainter painter(&sheet);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    const int textAlignment = (textBlockAtRight ? Qt::AlignRight : Qt::AlignLeft) | Qt::AlignVCenter;

    const int textBlockTop = textBlockAtBottom
        ? margin + gridHeight + 10
        : margin;

    const int gridTop = textBlockAtBottom
        ? margin
        : margin + textBlockHeight;

    int textY = textBlockTop;

    painter.setFont(titleFont);
    painter.setPen(titleTextColor.isValid() ? titleTextColor : QColor(255, 255, 255));
    painter.setBrush(Qt::NoBrush);

    QRect titleRect(margin, textY, sheetWidth - margin * 2, titleHeight);
    painter.drawText(titleRect, textAlignment, title);
    textY += titleHeight;

    painter.setFont(smallFont);

    if (!infoLines.isEmpty()) {
        painter.setPen(infoTextColor.isValid() ? infoTextColor : QColor(230, 230, 230));
        painter.setBrush(Qt::NoBrush);

        for (const QString &line : infoLines) {
            QRect lineRect(margin, textY, sheetWidth - margin * 2, lineHeight);
            painter.drawText(lineRect, textAlignment, line);
            textY += lineHeight;
        }

        textY += 4;
    }

    painter.setPen(countTextColor.isValid() ? countTextColor : QColor(210, 210, 210));
    painter.setBrush(Qt::NoBrush);

    QString subtitle = QString("%1 thumbnails | %2 columns × %3 rows | thumb %4 px")
        .arg(frames.size())
        .arg(columns)
        .arg(rows)
        .arg(thumbWidth);

    QRect subtitleRect(margin, textY, sheetWidth - margin * 2, subtitleHeight);
    painter.drawText(subtitleRect, textAlignment, subtitle);

    const int maxFrames = std::min(static_cast<int>(frames.size()), columns * rows);

    for (int i = 0; i < maxFrames; ++i) {
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

        painter.setBrush(Qt::NoBrush);
        painter.drawImage(targetRect, thumb);

        if (drawTileBorder) {
            painter.setPen(tileBorderColor.isValid() ? tileBorderColor : QColor(70, 70, 70));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(targetRect.adjusted(0, 0, -1, -1));
        }

        QString stamp = (drawTimestamps && i < times.size()) ? secondsToTimeText(times[i]) : QString();
        if (!stamp.isEmpty()) {
            QFont timestampFont = smallFont;
            timestampFont.setBold(true);
            painter.setFont(timestampFont);

            const int stampWidth = 96;
            const int stampHeight = 22;
            const int stampMargin = 6;

            const int stampX = timestampAtRight
                ? x + thumbWidth - stampWidth - stampMargin
                : x + stampMargin;

            const int stampY = timestampAtTop
                ? y + stampMargin
                : y + thumbHeight - stampHeight - stampMargin;

            QRect stampRect(stampX, stampY, stampWidth, stampHeight);

            QColor stampBackground = timestampBackgroundColor.isValid()
                ? timestampBackgroundColor
                : QColor(0, 0, 0, 170);
            if (stampBackground.alpha() == 255) {
                stampBackground.setAlpha(170);
            }

            painter.setPen(Qt::NoPen);
            painter.setBrush(stampBackground);
            painter.drawRoundedRect(stampRect, 4, 4);

            painter.setPen(timestampTextColor.isValid() ? timestampTextColor : QColor(255, 255, 255));
            painter.setBrush(Qt::NoBrush);
            painter.drawText(stampRect, Qt::AlignCenter, stamp);

            painter.setFont(smallFont);
        }
    }

    painter.end();
    return sheet;
}



