/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOSLISTMODEL_H
#define VIDEOSLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QString>
#include <QPixmap>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileSystemWatcher>

#include "preferences.h"
#include "appwindow.h"

class VideosListProxyModel;

/// @class: VideoListModel
/// @brief Item model for video list
///
/// This class implements the item model for the video list.
class VideosList;

class VideosListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    /// @brief Constructor
    /// @param parent QObject* The parent
    /// @param category AppWindow::PlayerCategory The category
    VideosListModel(VideosList *videosList, QObject *parent = 0, AppWindow::PlayerCategory category = AppWindow::MyMovies);

    /// @brief Destructor
    virtual ~VideosListModel();

    /// @brief Type defining model roles corresponding to metadata that can be displayed
    enum VideosListModelRoles {
        ThumbnailRole   = Qt::UserRole + 4,
        TitleRole       = Qt::UserRole + 5,
        DirectorRole    = Qt::UserRole + 6,
        ActorRole       = Qt::UserRole + 7,
        EpisodeNameRole = Qt::UserRole + 8,
        ShowNameRole    = Qt::UserRole + 9,
        DateRole        = Qt::UserRole + 10,
        CaptionLevel1Role = Qt::UserRole + 11,
        CaptionLevel2Role = Qt::UserRole + 12,
        IsFavoriteRole  = Qt::UserRole + 13,
        LengthRole      = Qt::UserRole + 14,
        SizeRole        = Qt::UserRole + 15,
        FormatRole      = Qt::UserRole + 16,
        FilmDescriptionRole = Qt::UserRole + 17,
        HeightRole = Qt::UserRole + 18,
        WidthRole = Qt::UserRole + 19,
        CopyrightRole = Qt::UserRole + 20,
        FilenameRole = Qt::UserRole + 21,
        FilePathRole = Qt::UserRole + 22,
        VideoCategoryRole = Qt::UserRole + 23,
        BitRateRole = Qt::UserRole + 24,
        LastReadRole = Qt::UserRole + 25
    };

    /// @brief Return file path
    /// @param index QModelIndex The index
    /// @return QString The file path
    QString filePath(const QModelIndex &index) const;

    /// @brief Return file name
    /// @param index QModelIndex The index
    /// @return QString The file name
    QString fileName(const QModelIndex &index) const;

    /// @brief Return QFileInfo
    /// @param index QModelIndex The index
    /// @return QString The QFileInfo
    QFileInfo fileInfo(const QModelIndex &index) const;

    /// @brief Returns the number of rows (files)
    /// @param parent QModelIndex Parent item
    /// @return The number of rows (files)
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// @brief Data method for model. It returns data given an index and display role
    /// @param index QModelIndex The index into the model for the item
    /// @param role int The display role
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// @brief Removes the model item index from the file system model and deletes the corresponding file from the file system
    /// @param index QModelIndex The index into the model for the item
    /// @return Return true if successful
    bool remove(const QModelIndex &index) const;

    /// @brief Add an index that need to be processed
    /// @param index QModelIndex The index
    void addTask(const QModelIndex &index);

    /// @brief Process the index, it is called when main thread is idle
    /// @param index QModelIndex The index
    void processTask(const QModelIndex &index);

    /// @brief Update data for recently viewed, intended to be called after some video been played
    void updateRecentlyViewed();

    /// @brief Update data for favorites, intended to be called after favorite action inside context menu is called
    void updateFavourites();

public slots:
    /// @brief Start to process tasks, actually to get meta info for the items
    void startTaskQueue();

    /// @brief Stop to process tasks
    void stopTaskQueue();

private slots:
    void handleMetainformationChanged(const QString &file);
    void handleDirectoryChanged(const QString &directory);

private:
    Preferences::VideoFolderCategory videoCategory(const QModelIndex &index) const;

private:
    QPixmap thumbnailByIndex(const QModelIndex &index) const;
    QString titleByIndex(const QModelIndex & index) const;
    QString filenameByIndex(const QModelIndex & index) const;
    QDateTime dateByIndex(const QModelIndex & index) const;
    QString episodeNameByIndex(const QModelIndex & index) const;
    QString directorByIndex(const QModelIndex & index) const;
    QString showNameByIndex(const QModelIndex & index) const;
    QString filmDescriptionByIndex(const QModelIndex & index) const;
    QString formatByIndex(const QModelIndex & index) const;
    qint64 sizeByIndex(const QModelIndex & index) const;
    QString lengthByIndex(const QModelIndex & index) const;
    QString copyrightByIndex(const QModelIndex &index) const;
    int widthByIndex(const QModelIndex &index) const;
    int heightByIndex(const QModelIndex &index) const;
    QString videoBitRate(const QModelIndex &index) const;
    QDateTime lastRead(const QModelIndex &index) const;
    Preferences::VideoFolderCategory mapFromAppCategoryToPreferenceCategory(AppWindow::PlayerCategory category);
    void fillFileInfoList();
    static bool compareLastModified(const QFileInfo &left, const QFileInfo &right);
    static bool compareLastRead(const QFileInfo &left, const QFileInfo &right);

private:
    QList<QModelIndex> m_tasks;
    QString m_rootPathMyMovies;
    QString m_rootPathFilms;
    QString m_rootPathTVShows;
    QString m_rootPathXDGVideos;

    QFileSystemWatcher *m_dirWatcher;
    QFileInfoList m_files;
    QFileInfoList m_myMovieFiles;
    QFileInfoList m_filmFiles;
    QFileInfoList m_tvShowsFiles;
    QFileInfoList m_xdgVideoFiles;
    AppWindow::PlayerCategory m_category;
    VideosList *m_videosList;
    int m_thumbSize;
};
#endif // VIDEOSLISTMODEL_H
