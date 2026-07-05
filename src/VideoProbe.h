// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "VideoTypes.h"

#include <QString>

class VideoProbe
{
public:
    static VideoInfo probe(const QString &fileName);
};
