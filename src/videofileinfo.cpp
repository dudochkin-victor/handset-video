/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QStringList>

#include "videofileinfo.h"

VideoFileInfo::VideoFileInfo()
    : m_favorite(false)
{
}

void VideoFileInfo::setLastPlayedDate(const QDateTime & date)
{
    m_lastPlayedDate = date;
}

QDateTime VideoFileInfo::lastPlayedDate() const
{
    return m_lastPlayedDate;
}

void VideoFileInfo::setFavorite(bool favorite)
{
    m_favorite = favorite;
}

bool VideoFileInfo::isFavorite() const
{
    return m_favorite;
}

void VideoFileInfo::setDateAdded(const QDateTime & date)
{
    m_dateAdded = date;
}

QDateTime VideoFileInfo::dateAdded() const
{
    return m_dateAdded;
}

VideoFileInfo VideoFileInfo::fromString(const QString & str/*, bool * ok*/)
{
    VideoFileInfo info;
    QStringList list = str.split(";");
    if (list.size() == 3) { // there are some records about this file
        bool ok;
        bool favorite = list.at(0).toInt(&ok);
        if (!ok)
            favorite = false;
        info.setFavorite(favorite);

        info.setLastPlayedDate( QDateTime::fromString(list.at(1)) );
        info.setDateAdded( QDateTime::fromString(list.at(2)) );
    }
    return info;
}

QString VideoFileInfo::toString() const
{
    return QString("%1;%2;%3")
          .arg(m_favorite ? "1" : "0")
          .arg(m_lastPlayedDate.toString())
          .arg(m_dateAdded.toString());
}

void VideoFileInfo::reset()
{
    m_lastPlayedDate = QDateTime();
    m_dateAdded = QDateTime();
    m_favorite = false;
}
