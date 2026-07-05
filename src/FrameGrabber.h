// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QImage>
#include <QString>

class FrameGrabber
{
public:
    enum class SeekMode
    {
        Fast,
        Accurate
    };

    static bool grabFrameAt(
        const QString &fileName,
        double seconds,
        QImage &outImage,
        QString &outLog,
        SeekMode seekMode = SeekMode::Accurate,
        int targetWidth = 0,
        int targetHeight = 0
    );
};
