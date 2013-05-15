/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef THUMBNAILMANAGER_H
#define THUMBNAILMANAGER_H

#include <QObject>
#include <QHash>
#include <QPixmap>
#include <QDateTime>
#include <QString>
#include <QDir>

/// @class ThumbnailManager
/// @brief Generates and returns video file thumbnail images.
///
/// The Thumbnail Manager class:
/// - generates and/or extracts thumbnail images from video files in
///   the directories for each of the three categories
/// - returns a thumbnail image for a given video
/// - updates thumbnails when videos change and removes thumbnails for
///   videos no longer present
///
/// The class is a singleton. There should only ever be one instance
/// running per application. Typically you will create a thumbnail
/// manager on application startup in the updating state and then use
/// it to obtain thumbnails for video images.
///
/// Thumbnails are continuously generated in the background. Returning a
/// thumbnail should be fast as the thumbnail should already be
/// generated and on disk.
///
/// The thumbnail size and video directories are controlled by settings
/// in the application preferences.

class MetaInfo;

class ThumbnailManager : public QObject
{
    Q_OBJECT
public:

    /// @brief Destructor. Stops updating and cleans up any resources used.
    ~ThumbnailManager();

    /// @brief Return the instance of the thumbnail manager singleton.
    /// @return ThumbnailManager Returns the single instance.
    static ThumbnailManager *instance();

    /// @brief Default folder for storing thumbnails
    static QString thumbnailFolder;

    /// @brief Default suffix for thumbnails
    static QString thumbnailSuffix;

    static QString thumbnailSuffix1;

    /// @brief Default QDir for storing thumbnails
    static QDir thumbnailDir;

    QStringList thumbnailPaths(const QString &file, QByteArray &md5Result);
    QString thumbnailExistPath(const QString &file, QByteArray &md5Result);
    /// @brief Return the thumbnail for a video file, as a QPixmap. If no
    /// thumbnail is available for a video file, a default image file
    /// is returned. The videoFile should be a full path to the file.
    /// @param videoFile QString The full path to the video file.
    /// @return QPixmap The thumbnail as a QPixmap
    QPixmap thumbnail(const QString &videoFile, int scaledWidth, int scaledHeight);

    /// @brief Return title
    /// @param videoFile const QString& The video file
    /// @return QString The video file's title
    QString title(const QString &videoFile) const;

    /// @brief Return date
    /// @param videoFile const QString& The video file
    /// @return QString The video file's date
    QDateTime date(const QString &videoFile) const;

    /// @brief Return episode name
    /// @param videoFile const QString& The video file
    /// @return QString The video file's episode name
    QString episodeName(const QString &videoFile) const;

    /// @brief Return director
    /// @param videoFile const QString& The video file
    /// @return QString The video file's directory
    QString director(const QString &videoFile) const;

    /// @brief Return television show name
    /// @param videoFile const QString& The video file
    /// @return QString The video file's show name
    QString showName(const QString &videoFile) const;

    /// @brief Return file description
    /// @param videoFile const QString& The video file
    /// @return QString The video file's file description
    QString filmDescription(const QString &videoFile) const;

    /// @brief Return file format (codec)
    /// @param videoFile const QString& The video file
    /// @return QString The video file's format
    QString format(const QString &videoFile) const;

    /// @brief Return file copyright information
    /// @param videoFile const QString& The video file
    /// @return QString The video file's copyright string
    QString copyright(const QString &videoFile) const;

    /// @brief Return file size
    /// @param videoFile const QString& The video file
    /// @return QString The video file's size in bytes
    qint64 size(const QString &videoFile) const;

    /// @brief Return file length
    /// @param videoFile const QString& The video file
    /// @return QString The video file's length (duration)
    QString length(const QString &videoFile) const;

    /// @brief Return file width
    /// @param videoFile const QString& The video file
    /// @return QString The video file's width in pixels
    int width(const QString &videoFile) const;

    /// @brief Return file height
    /// @param videoFile const QString& The video file
    /// @return QString The video file's height in pixels
    int height(const QString &videoFile) const;

    /// @brief Event handler for metainfo changes event
    /// @param event QEvent* The event
    /// @return book True of the event was handled.
    virtual bool event(QEvent *event);

    /// @brief Update thumbnail for list of video files
    /// @param files QStringList The list of the video files.
    //void updateThumbnails(const QStringList &files);

    bool metainformationExsit(const QString &file);

    void fillMetaInfo(const QString &file);

signals:
    /// @brief This signal is emitted when a meta information is generated or need updated (signal). This can be used by application code that caches meta information to determine that it needs to update its copy.
    /// @param videoFile QString The path of the video file whose meta information changed.
    void metainformationChanged(const QString &videoFile);

private slots:
    /// @brief Handle categoryChanged signal from the model
    /// @param path QString The path of the current category.
    //void handleCategoryChanged(const QString &path);

    /// @brief Handle fileChanged signal from the model
    /// @param path QString The path of the video file.
    //void handleFileChanged(const QString &path);

    /// @brief Handle fileRemoved signal from the model
    /// @param path QString The path of the video file.
    //void handleFileRemoved(const QString &path);

    /// @brief Handle fileAdded signal from the model
    /// @param path QString The path of the video file.
    //void handleFileAdded(const QString &path);

    /// @brief Delete ThumbnailManager object
    void cleanup();

    /// @brief signal handler for ready signal of thumbnailer. Used to update UI when thumbnail is ready
    /// @param handle const unsigned int& Handler that is ready
    /// @param urls const QStringList& Urls of the video files that thumbnail is ready
    void handleThumbnailReady(const unsigned int &handle, const QStringList &urls);


 private:
    // Constructor is private because this is a singleton.
    // Construct it using the instance() method.
    ThumbnailManager();

    /// @brief Create the directories, if necessary
    void createDirectories();

    /// @brief Check if MPlayer available on system
    //void checkMplayerExists();

    void  loadMetaInfo();

    void  saveMetaInfo();

private:
    QHash<QString, MetaInfo *> m_metaInfoCache;
    bool m_isMplayerExists;
    static ThumbnailManager *m_instance;
    QPixmap m_defaultThumbnailPic;
    bool m_defaultThumbnailScaled;
};

#endif // THUMBNAILNMANAGER_H
