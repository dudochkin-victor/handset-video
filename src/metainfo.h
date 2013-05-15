/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef METAINFO_H
#define METAINFO_H

#include <QEvent>
#include <QString>
#include <QDateTime>
#include <QPixmap>

/// @class MetaInfo
/// @brief Class to store video file metainformation

class MetaInfo
{
public:

    /// @brief Default constuctor
    MetaInfo()
    {
        m_size = 0;
        m_width = 0;
        m_height = 0;
    }

    /// @brief Copy constructor
    MetaInfo(const MetaInfo &mi)
    {
        m_filePath = mi.m_filePath;
        m_title = mi.m_title;
        m_episodeName = mi.m_episodeName;
        m_director = mi.m_director;
        m_showName = mi.m_showName;
        m_filmDescription = mi.m_filmDescription;
        m_format = mi.m_format;
        m_copyright = mi.m_copyright;
        m_thumbnail = mi.m_thumbnail;
        m_length = mi.m_length;
        m_date = mi.m_date;
        m_size = mi.m_size;
        m_width = mi.m_width;
        m_height = mi.m_height;
        m_thumbnailPic = mi.m_thumbnailPic;
        m_thumbnailLoaded = mi.m_thumbnailLoaded;
        m_thumbnailScaled = mi.m_thumbnailScaled;
    }

    ~MetaInfo()
    {
    }

    /// @brief Assignment operator
    /// @param other MetaInfo& The object to be copied
    /// @return MetaInfo& The object copied
    inline MetaInfo &operator=(const MetaInfo &other)
    {
        m_filePath = other.m_filePath;
        m_title = other.m_title;
        m_episodeName = other.m_episodeName;
        m_director = other.m_director;
        m_showName = other.m_showName;
        m_filmDescription = other.m_filmDescription;
        m_format = other.m_format;
        m_copyright = other.m_copyright;
        m_thumbnail = other.m_thumbnail;
        m_length = other.m_length;
        m_date = other.m_date;
        m_size = other.m_size;
        m_width = other.m_width;
        m_height = other.m_height;
        m_thumbnailPic = other.m_thumbnailPic;
        m_thumbnailLoaded = other.m_thumbnailLoaded;
        m_thumbnailScaled = other.m_thumbnailScaled;
        return *this;
    }

    /// @brief Sets a path to the video
    /// @param path QString The title to the video.
    inline void setFilePath(const QString &path) {m_filePath = path;}

    /// @brief Returns a path to the video
    inline QString filePath() const {return m_filePath;}

    /// @brief Sets a title of the video
    /// @param title QString The title of the video.
    inline void setTitle(const QString &title) {m_title = title;}

    /// @brief Returns a title of the video
    inline QString title() const {return m_title;}

    /// @brief Sets an episode name of the video
    /// @param episode QString The episode name of the video.
    inline void setEpisodeName(const QString &episode) {m_episodeName = episode;}

    /// @brief Returns an episode name of the video
    inline QString episodeName() const {return m_episodeName;}

    /// @brief Sets a show name of the video
    /// @param show QString The show name of the video.
    inline void setShowName(const QString &show) {m_showName = show;}

    /// @brief Returns a show name of the video
    inline QString showName() const {return m_showName;}

    /// @brief Sets a director of the video
    /// @param director QString The director of the video.
    inline void setDirector(const QString &director) {m_director = director;}

    /// @brief Returns a director of the video
    inline QString director() const {return m_director;}

    /// @brief Sets a date of the video
    /// @param date QDateTime The date of the video.
    inline void setDate(const QDateTime &date) {m_date = date;}

    /// @brief Returns a date of the video
    inline QDateTime date() const {return m_date;}

    /// @brief Sets a length of the video
    /// @param length QString The length of the video.
    inline void setLength(const QString &length) {m_length = length;}

    /// @brief Returns a length of the video
    inline QString length() const {return m_length;}

    /// @brief Sets a size of the video
    /// @param size qint64 The size of the video.
    inline void setSize(qint64 size) {m_size = size;}

    /// @brief Returns a size of the video
    inline qint64 size() const {return m_size;}

    /// @brief Set height parameter
    /// @param h int Height in pixels
    inline void setHeight(int h) {m_height = h;}

    /// @brief Returns height parameter
    inline int height() const {return m_height;}

    /// @brief Set width parameter
    /// @param w int Width in pixels
    inline void setWidth(int w) {m_width = w;}

    /// @brief Returns width parameter
    inline int width() const {return m_width;}

    /// @brief Sets a format of the video
    /// @param format QString The format of the video.
    inline void setFormat(const QString &format) {m_format = format;}

    /// @brief Returns a format of the video
    inline QString format() const {return m_format;}

    /// @brief Sets a description of the video
    /// @param description QString The description of the video.
    inline void setDescription(const QString &description) {m_filmDescription = description;}

    /// @brief Returns a description of the video
    inline QString description() const {return m_format;}

    /// @brief Set copyright string
    /// @param copyright QString The copyright string
    inline void setCopyright(const QString &copyright) {m_copyright = copyright;}

    /// @brief Returns copyright string
    inline QString copyright() const {return m_copyright;}

    /// @brief Sets thumbnail
    /// @param thumbnail QString The thumbnail of the video.
    inline void setThumbnail(const QString &thumbnail) {m_thumbnail = thumbnail;}

    /// @brief Returns thumbnail
    inline QString thumbnail() const {return m_thumbnail;}

    /// @brief Sets thumbnail
    /// @param thumbnail QString The thumbnail of the video.
    inline void setThumbnailPic(const QPixmap &thumbnailPic) {m_thumbnailPic = thumbnailPic;}

    /// @brief Returns thumbnail
    inline QPixmap thumbnailPic() const {return m_thumbnailPic;}

    inline void setThumbnailLoaded(bool loaded) {m_thumbnailLoaded = loaded;};

    inline void setThumbnailScaled(bool scaled) {m_thumbnailScaled = scaled;};

    inline bool thumbnailLoaded() const {return m_thumbnailLoaded;};

    inline bool thumbnailScaled() const {return m_thumbnailScaled;};


private:

    /// @brief File path
    QString m_filePath;
    /// @brief Title
    QString m_title;
    /// @brief Episode name
    QString m_episodeName;
    /// @brief Director
    QString m_director;
    /// @brief Show name
    QString m_showName;
    /// @brief Film's description
    QString m_filmDescription;
    /// @brief Format (e.g. codec)
    QString m_format;
    /// @brief Copyright information
    QString m_copyright;
    /// @brief Path to thumbnail file
    QString m_thumbnail;
    /// @brief Length (duration)
    QString m_length;
    /// @brief  Date
    QDateTime m_date;
    /// @brief File size
    qint64 m_size;
    /// @brief Width in pixels
    int m_width;
    /// @brief Height in pixels
    int m_height;

    QPixmap m_thumbnailPic;

    bool m_thumbnailLoaded;
    bool m_thumbnailScaled;
};


/// @class MetaInfoChangedEvent
/// @brief Class to handle meta info change event


class MetaInfoChangedEvent : public QEvent
{
public:
    enum {
        MetaInfoChanged = QEvent::User + 1
    };

    /// @brief Constructor
    /// @param mi const MetaInfo& The metainfo
    MetaInfoChangedEvent(const MetaInfo &mi)
        : QEvent(static_cast<QEvent::Type>(MetaInfoChanged)),
        m_mi(mi)
    {
    }

    /// @brief Return meta information
    /// @return cnst MeataInfo & The meta information
    inline const MetaInfo &metaInformation() const {return m_mi;}

private:
    MetaInfo m_mi;
};

#endif // METAINFO_H
