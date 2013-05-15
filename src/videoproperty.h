/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOPROPERTY_H
#define VIDEOPROPERTY_H

#include <QString>

#include <QtTracker/Tracker>

/// @class VideoProperty
/// @brief Get video properties.
///
/// The VideoProperty class:
/// - get video properties using tracker service.
///

class VideoProperty
{
public:
    /// @brief Get mimetype of the video file
    /// @param filePath const QString& The path of the video file
    /// @return QString The mimetype of the video file
    static QString getMimeType(const QString &filePath);
};

#endif
