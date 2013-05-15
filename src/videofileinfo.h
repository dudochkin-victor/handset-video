/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOFILEINFO_H
#define VIDEOFILEINFO_H

#include <QDateTime>
#include <QString>

/// @class VideoFileInfo
/// @brief A helper class that is used internally in Preferences that
/// helps to store and retrieve video file associated information

class VideoFileInfo
{
public:
    /// @brief Constructor. Initializes VideoFileInfo class.
    VideoFileInfo();

    /// @brief Set last played date and time for a given video file.
    /// @param date QDateTime Date and time of last playback. Use QDateTime() to mark video as not having been played before.
    void setLastPlayedDate(const QDateTime & date);

    /// @brief Return last played date and time of the video file.
    /// @return last played date and time as QDateTime. Returns invalid QDateTime if the video has not been played before.
    QDateTime lastPlayedDate() const;

    /// @brief Sets the video file as a favorite.
    /// @param favorite bool Defines whether set the video file as a favorite: true for favorite, otherwise false.
    void setFavorite(bool favorite);

    /// @brief Returns whether the video file is a favorite
    /// @return bool The value is true if the video file is favorite, otherwise false.
    bool isFavorite() const;

    /// @brief Set added date and time for the video file.
    /// @param date QDateTime Date and time when file was added. Use QDateTime() to mark video as not having been added before.
    void setDateAdded(const QDateTime & date);

    /// @brief Return added date and time for the video file.
    /// @return added date and time as QDateTime. Returns invalid QDateTime if the video has not been added before.
    QDateTime dateAdded() const;

    /// @brief Get a VideoFileInfo object instance from its string representation.
    /// @param str QString string representation of VideoFileInfo
    /// @return VideoFileInfo as C++ object from its string representation.
    static VideoFileInfo fromString(const QString & str);

    /// @brief Return string representation of this VideoFileInfo object.
    /// @return QString in the following format:
    /// Favorite;LastPlayedDate;DateAdded
    /// where:
    /// Favorite       - {0|1}     - favorite or not
    /// LastPlayedDate - QDateTime - in Qt::TextDate format
    /// DateAdded      - QDateTime - in Qt::TextDate format
    QString toString() const;

protected:
    /// @brief Reset all the internal values to default.
    void reset();

private:
    bool m_favorite;
    QDateTime m_lastPlayedDate;
    QDateTime m_dateAdded;
};

#endif
