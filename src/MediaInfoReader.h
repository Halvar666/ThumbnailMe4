// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "VideoTypes.h"

#include <QString>

class MediaInfoReader
{
public:
    static QString summaryForFile(const QString &fileName, const VideoInfo &fallbackInfo);
    static bool isAvailable();

private:
    static QString fallbackSummary(const QString &fileName, const VideoInfo &info);
};
