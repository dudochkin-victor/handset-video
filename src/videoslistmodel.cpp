/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QDebug>
#include <QVector>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>

#include <MTheme>

#include "videoslistmodel.h"
#include "videoslistproxymodel.h"
#include "videoslist.h"
#include "preferences.h"
#include "thumbnailmanager.h"

VideosListModel::VideosListModel(VideosList *videosList, QObject *parent, AppWindow::PlayerCategory category) :
        QAbstractListModel(parent),
        m_category(category)
{
    m_rootPathMyMovies = Preferences::folderPath(mapFromAppCategoryToPreferenceCategory(AppWindow::MyMovies));
    m_rootPathFilms = Preferences::folderPath(mapFromAppCategoryToPreferenceCategory(AppWindow::Films));
    m_rootPathTVShows = Preferences::folderPath(mapFromAppCategoryToPreferenceCategory(AppWindow::TVShows));
    m_rootPathXDGVideos = Preferences::xdgVideoFolder();

    qDebug() << "XDG " << m_rootPathMyMovies << m_rootPathFilms << m_rootPathTVShows << m_rootPathXDGVideos;

    fillFileInfoList();

    m_dirWatcher = new QFileSystemWatcher(this);
    m_dirWatcher->removePaths(m_dirWatcher->directories());
    m_dirWatcher->addPath(m_rootPathMyMovies);
    m_dirWatcher->addPath(m_rootPathFilms);
    m_dirWatcher->addPath(m_rootPathTVShows);
    m_dirWatcher->addPath(m_rootPathXDGVideos);
    connect(m_dirWatcher, SIGNAL(directoryChanged(const QString &)), SLOT(handleDirectoryChanged(const QString &)));

    connect(ThumbnailManager::instance(), SIGNAL(metainformationChanged(const QString &)), SLOT(handleMetainformationChanged(const QString &)));
    m_videosList = videosList;
    m_thumbSize = m_videosList->getMList()->cellCreator()->cellSize().toSize().height();
}

VideosListModel::~VideosListModel()
{
    delete m_dirWatcher;
}

bool VideosListModel::compareLastModified(const QFileInfo &left, const QFileInfo &right)
{
    if (left.lastModified() < right.lastModified()) {
        return false;
    }
    else {
        return true;
    }
}

bool VideosListModel::compareLastRead(const QFileInfo &left, const QFileInfo &right)
{
    QDateTime leftRead = left.lastRead();
    QDateTime rightRead = right.lastRead();

    if (leftRead < rightRead) {
        return false;
    }
    else {
        return true;
    }
}

void VideosListModel::fillFileInfoList()
{
    qDebug() << "VideosListModel::fillFileInfoList()";
    QDir myMoviesDir(m_rootPathMyMovies);
    QDir filmsDir(m_rootPathFilms);
    QDir tvShowsDir(m_rootPathTVShows);
    QDir xdgVideoDir(m_rootPathXDGVideos);

    m_files.clear();
    m_myMovieFiles.clear();
    m_filmFiles.clear();
    m_tvShowsFiles.clear();
    m_xdgVideoFiles.clear();

    m_myMovieFiles = myMoviesDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_filmFiles = filmsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_tvShowsFiles = tvShowsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_xdgVideoFiles = xdgVideoDir.entryInfoList(QDir::Files, QDir::NoSort);

    switch (m_category) {
        case AppWindow::Showall: {
            m_files += m_myMovieFiles;
            m_files += m_filmFiles;
            m_files += m_tvShowsFiles;
            m_files += m_xdgVideoFiles;
            qDebug() << "Category is Showall";
            break;
        }
        case AppWindow::MyMovies: {
            m_files += m_myMovieFiles;
            qDebug() << "Category is MyMovies";
            break;
        }
        case AppWindow::Films: {
            m_files += m_filmFiles;
            qDebug() << "Category is Films";
            break;
        }
        case AppWindow::TVShows: {
            m_files += m_tvShowsFiles;
            qDebug() << "Category is TVShows";
            break;
        }
        case AppWindow::RecentlyAdded: {
            QFileInfoList temp;
            temp += m_myMovieFiles;
            temp += m_filmFiles;
            temp += m_tvShowsFiles;
            temp += m_xdgVideoFiles;
            qSort(temp.begin(), temp.end(), VideosListModel::compareLastModified);
            m_files = temp.mid(0, 10);
            qDebug() << "Category is RecentlyAdded";
            break;
        }
        case AppWindow::RecentlyViewed: {
            QFileInfoList temp;
            temp += m_myMovieFiles;
            temp += m_filmFiles;
            temp += m_tvShowsFiles;
            temp += m_xdgVideoFiles;
            qSort(temp.begin(), temp.end(), VideosListModel::compareLastRead);
            m_files = temp.mid(0, 10);
            qDebug() << "Category is RecentlyViewed";
            break;
        }
        case AppWindow::Favourites: {
            QFileInfoList temp;

            for(QList<QFileInfo>::iterator i = m_myMovieFiles.begin(); i != m_myMovieFiles.end(); i++) {
                if (Preferences::isFavorite(i->absoluteFilePath()))
                    temp << *i;
            }

            for(QList<QFileInfo>::iterator i = m_filmFiles.begin(); i != m_filmFiles.end(); i++) {
                if (Preferences::isFavorite(i->absoluteFilePath()))
                    temp << *i;
            }

            for(QList<QFileInfo>::iterator i = m_tvShowsFiles.begin(); i != m_tvShowsFiles.end(); i++) {
                if (Preferences::isFavorite(i->absoluteFilePath()))
                    temp << *i;
            }
            m_files += temp;

            break;
        }
        default:
            Q_ASSERT(false);
            break;
    }
    qDebug() << "Files to display are: ";
    for(QList<QFileInfo>::iterator i = m_files.begin(); i != m_files.end(); i++) {
        qDebug() << i->absoluteFilePath();
        if (m_category == AppWindow::RecentlyViewed) {
            qDebug() << i->absoluteFilePath() << "Last Read " << i->lastRead() << "Last Modified " << i->lastModified();
        }
    }

}

Preferences::VideoFolderCategory VideosListModel::mapFromAppCategoryToPreferenceCategory(AppWindow::PlayerCategory category)
{
    switch (category) {
    case AppWindow::MyMovies:
        return Preferences::MyMovies;
    case AppWindow::Films:
        return Preferences::Films;
    case AppWindow::TVShows:
        return Preferences::TVShows;
    default:
        return Preferences::Error;
    }
}

int VideosListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_files.size();
}

void VideosListModel::addTask(const QModelIndex &index)
{
    qDebug() << "VideosListModel::addTask(), row is " << index.row();
    m_tasks << index;
}

void VideosListModel::startTaskQueue()
{
    if (m_tasks.isEmpty())
        return;

    const QModelIndex &index = m_tasks.takeFirst();
    processTask(index);
    if (!m_tasks.isEmpty())
        QTimer::singleShot(0, this, SLOT(startTaskQueue()));
}

void VideosListModel::stopTaskQueue()
{
    qDebug() << "VideosListModel::stopTaskQueue" << m_tasks.count();
    m_tasks.clear();
}

void VideosListModel::processTask(const QModelIndex &index)
{
    qDebug() << "VideosListModel::processTask()" << index.row() << "category is " << m_category;

    if (!index.isValid()) {
        qWarning() << "Invalid index inside VideosListModel::processTask " << index.row();
        return;
    }

    int row = index.row();
    QString file = filePath(index);
    qDebug() << "VideosListModel::processTask()" << row << " " << file;

    if (ThumbnailManager::instance()->metainformationExsit(file)) {
        qDebug() << file << "MetaInfo exist";
        return;
    } else {
        qDebug() << file << "MetaInfo NOT exist";
        ThumbnailManager::instance()->fillMetaInfo(file);
    }
}

QString VideosListModel::filePath(const QModelIndex &index) const
{
    QString path;

    if (!index.isValid())
        return path;

    const int row = index.row();
    if (row >= m_files.size())
        return path;

    QFileInfo info = m_files.at(row);
    return info.absoluteFilePath();
}

QString VideosListModel::fileName(const QModelIndex &index) const
{
    QString path;

    if (!index.isValid())
        return path;

    const int row = index.row();
    if (row >= m_files.size())
        return path;

    QFileInfo info = m_files.at(row);
    return info.fileName();
}

QFileInfo VideosListModel::fileInfo(const QModelIndex &index) const
{
    return QFileInfo(filePath(index));
}

bool VideosListModel::remove(const QModelIndex &index) const
{
    const int row = index.row();
    if (row >= m_files.size())
        return false;
    QString path = filePath(index);
    qDebug() << "VideosListModel::remove " << path;

/*
    VideosListModel *self = const_cast<VideosListModel *>(this);
    self->m_files.removeAt(row);

    QString path = filePath(index);
    m_dirWatcher->removePath(path);
    qDebug() << "VideosListModel::remove " << path;*/
    return QFile::remove(path);
}

void VideosListModel::handleMetainformationChanged(const QString &file)
{
    Q_UNUSED(file);
    qDebug() << "VideosListModel::handleMetainformationChanged " << file;
    emit layoutChanged();
}

void VideosListModel::handleDirectoryChanged(const QString &directory)
{
    Q_UNUSED(directory);
    fillFileInfoList();
    emit layoutChanged();
}

QPixmap VideosListModel::thumbnailByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->thumbnail(filePath(index), m_thumbSize, m_thumbSize);
}

QString VideosListModel::titleByIndex(const QModelIndex & index) const
{
    return ThumbnailManager::instance()->title(filePath(index));
}

QDateTime VideosListModel::dateByIndex(const QModelIndex & index) const
{
    return ThumbnailManager::instance()->date(filePath(index));
}

QString VideosListModel::episodeNameByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->episodeName(filePath(index));
}

QString VideosListModel::directorByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->director(filePath(index));
}

QString VideosListModel::showNameByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->showName(filePath(index));
}

QString VideosListModel::filmDescriptionByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->filmDescription(filePath(index));
}

QString VideosListModel::formatByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->format(filePath(index));
}

qint64 VideosListModel::sizeByIndex(const QModelIndex & index) const
{
    return ThumbnailManager::instance()->size(filePath(index));
}

QString VideosListModel::lengthByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->length(filePath(index));
}

QString VideosListModel::copyrightByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->copyright(filePath(index));
}

int VideosListModel::widthByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->width(filePath(index));
}

int VideosListModel::heightByIndex(const QModelIndex &index) const
{
    return ThumbnailManager::instance()->height(filePath(index));
}

QString VideosListModel::filenameByIndex(const QModelIndex & index) const
{
    return fileName(index);
}

QString VideosListModel::videoBitRate(const QModelIndex &index) const
{
    //TODO
    return QString("BitRate");
}

QDateTime VideosListModel::lastRead(const QModelIndex &index) const
{
    QFileInfo info = fileInfo(index);
    return info.lastRead();
}

QVariant VideosListModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case ThumbnailRole:
        return thumbnailByIndex(index);
    case TitleRole:
        return titleByIndex(index);
    case DirectorRole:
        return directorByIndex(index);
    case ActorRole:
        return QString("Actor");   //TODO: Get actor
    case EpisodeNameRole:
        return episodeNameByIndex(index);
    case ShowNameRole:
        return showNameByIndex(index);
    case DateRole:
        return dateByIndex(index);
    case IsFavoriteRole:
        return Preferences::isFavorite(filePath(index));
    case LengthRole:
        return lengthByIndex(index);
    case SizeRole:
        return sizeByIndex(index);
    case FormatRole:
        return formatByIndex(index);
    case HeightRole:
        return heightByIndex(index);
    case WidthRole:
        return widthByIndex(index);
    case CopyrightRole:
        return copyrightByIndex(index);
    case FilmDescriptionRole:
        return filmDescriptionByIndex(index);
    case FilenameRole:
        return filenameByIndex(index);
    case CaptionLevel1Role: {
        QString text;
        switch (videoCategory(index)) {
        case Preferences::MyMovies:
        case Preferences::Films:
            text = titleByIndex(index);
            break;
        case Preferences::TVShows:
            text = episodeNameByIndex(index);
            break;
        default:
            /* TODO */
            text = titleByIndex(index);

            break;
        }
        return text;
        }
    case CaptionLevel2Role: {
        QString text;
        switch (videoCategory(index)) {
        case Preferences::MyMovies:
            text = dateByIndex(index).toString("MMM d, yyyy");
            break;
        case Preferences::Films:
            text = directorByIndex(index);
            if (text == tr("Unknown"))
                text = "Unknown";
            break;
        case Preferences::TVShows:
            text = showNameByIndex(index);
            if (text == tr("Unknown"))
                text = "Unknown";
            break;
        default:
            text = titleByIndex(index);

            break;
        }
        return text;
        }
    case FilePathRole:
        return filePath(index);
    case VideoCategoryRole:
        return videoCategory(index);
    case BitRateRole:
        return videoBitRate(index);
    case LastReadRole:
        return lastRead(index);
    default:
        break;
    }

    return QVariant();
}

Preferences::VideoFolderCategory VideosListModel::videoCategory(const QModelIndex &index) const
{
    QString path = filePath(index);
    QFileInfo fileInfo(path);
    if (m_myMovieFiles.contains(fileInfo)) {
        return Preferences::MyMovies;
    } else if (m_filmFiles.contains(fileInfo)) {
        return Preferences::Films;
    } else if (m_tvShowsFiles.contains(fileInfo)) {
        return Preferences::TVShows;
    } else {
        return Preferences::Error;
    }
}

void VideosListModel::updateRecentlyViewed()
{
    qDebug() << "VideosListModel::updateRecentlyViewed() ";

    if (m_category != AppWindow::RecentlyViewed)
        return;

    QDir myMoviesDir(m_rootPathMyMovies);
    QDir filmsDir(m_rootPathFilms);
    QDir tvShowsDir(m_rootPathTVShows);
    QDir xdgVideosDir(m_rootPathXDGVideos);

    m_files.clear();
    m_myMovieFiles.clear();
    m_filmFiles.clear();
    m_tvShowsFiles.clear();
    m_xdgVideoFiles.clear();

    m_myMovieFiles = myMoviesDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_filmFiles = filmsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_tvShowsFiles = tvShowsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_xdgVideoFiles = xdgVideosDir.entryInfoList(QDir::Files, QDir::NoSort);

    QFileInfoList temp;
    temp += m_myMovieFiles;
    temp += m_filmFiles;
    temp += m_tvShowsFiles;
    temp += m_xdgVideoFiles;
    qSort(temp.begin(), temp.end(), VideosListModel::compareLastRead);
    m_files = temp.mid(0, 10);

    for(QList<QFileInfo>::iterator i = m_files.begin(); i != m_files.end(); i++) {
        qDebug() << i->absoluteFilePath();
        if (m_category == AppWindow::RecentlyViewed) {
            qDebug() << i->absoluteFilePath() << "Last Read " << i->lastRead() << "Last Modified " << i->lastModified();
        }
    }

    emit layoutChanged();
}

void VideosListModel::updateFavourites()
{
    qDebug() << "VideosListModel::updateFavourites()";

    if (m_category != AppWindow::Favourites)
        return;

    QDir myMoviesDir(m_rootPathMyMovies);
    QDir filmsDir(m_rootPathFilms);
    QDir tvShowsDir(m_rootPathTVShows);
    QDir xdgVideosDir(m_rootPathXDGVideos);

    m_files.clear();
    m_myMovieFiles.clear();
    m_filmFiles.clear();
    m_tvShowsFiles.clear();
    m_xdgVideoFiles.clear();

    m_myMovieFiles = myMoviesDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_filmFiles = filmsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_tvShowsFiles = tvShowsDir.entryInfoList(QDir::Files, QDir::NoSort);
    m_xdgVideoFiles = xdgVideosDir.entryInfoList(QDir::Files, QDir::NoSort);

    QFileInfoList temp;

    for(QList<QFileInfo>::iterator i = m_myMovieFiles.begin(); i != m_myMovieFiles.end(); i++) {
        if (Preferences::isFavorite(i->absoluteFilePath()))
            temp << *i;
    }

    for(QList<QFileInfo>::iterator i = m_filmFiles.begin(); i != m_filmFiles.end(); i++) {
        if (Preferences::isFavorite(i->absoluteFilePath()))
            temp << *i;
    }

    for(QList<QFileInfo>::iterator i = m_tvShowsFiles.begin(); i != m_tvShowsFiles.end(); i++) {
        if (Preferences::isFavorite(i->absoluteFilePath()))
            temp << *i;
    }

    for(QList<QFileInfo>::iterator i = m_xdgVideoFiles.begin(); i != m_xdgVideoFiles.end(); i++) {
        if (Preferences::isFavorite(i->absoluteFilePath()))
            temp << *i;
    }

    m_files += temp;
    for(QList<QFileInfo>::iterator i = m_files.begin(); i != m_files.end(); i++) {
        qDebug() << "updated favourites are " << i->absoluteFilePath();
    }

    emit layoutChanged();
}



