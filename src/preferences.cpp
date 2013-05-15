/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QDir>
#include <QApplication>
#include <QDesktopWidget>

#include <iostream>
#include <QtDebug>

#include "preferences.h"
#include "appvideospage.h"
#include "videofileinfo.h"
#include "metainfo.h"
#include "xdgpath.h"

PreferencesPrivate Preferences::m_private;
Preferences::LaunchMode Preferences::m_launchMode = Preferences::NormalMode;
QStringList Preferences::m_filesList;
XDGPath *Preferences::m_xdgPath = new XDGPath();

QSettings* PreferencesPrivate::settings()
{
    Q_ASSERT(QApplication::instance()); //make sure we create QSettings object after QApplication
    if (!m_settings)
        m_settings.reset(new QSettings);
    return m_settings.data();
}

QString Preferences::folderPath(VideoFolderCategory type)
{
    QSettings* settings = m_private.settings();
    QString xdgVideoPath = QString(m_xdgPath->getXDGVideosPath());

    switch (type) {
    case MyMovies:{
        return settings->value("VideoFolderCategory/MyMovies", xdgVideoPath + QString("/My Movies")).toString();
        break;
    }
    case Films:
        return settings->value("VideoFolderCategory/Films", xdgVideoPath + QString("/Films")).toString();
        break;

    case TVShows:
        return settings->value("VideoFolderCategory/TVShows", xdgVideoPath + QString("/TV Shows")).toString();
        break;

    default:
        return "";
        break;
    }

    // to avoid compiler warning
    Q_ASSERT(false);
    return QString();
}

QString Preferences::xdgVideoFolder()
{
    QString xdgVideoPath = QString(m_xdgPath->getXDGVideosPath());
    return xdgVideoPath;
}

void Preferences::setFolderPath(VideoFolderCategory type, const QString &path)
{
    QSettings* settings = m_private.settings();

    switch (type) {
    case MyMovies:
        settings->setValue("VideoFolderCategory/MyMovies", path);
        break;
    case Films:
        settings->setValue("VideoFolderCategory/Films", path);
        break;
    case TVShows:
        settings->setValue("VideoFolderCategory/TVShows", path);
        break;
    default:
        break;
    }
}

Preferences::MetaDataValue Preferences::listSortKey()
{
    QSettings* settings = m_private.settings();
    return (Preferences::MetaDataValue)settings->value("ListSortKey", (int)title).toInt();
}

void Preferences::setListSortKey(MetaDataValue key)
{
    QSettings* settings = m_private.settings();
    settings->setValue("ListSortKey", (int)key);
}

// Note: The list of metadata is hardcoded and cannot currently be changed as a preference.
QList<Preferences::MetaDataValue> Preferences::listMetaData()
{
    QList<MetaDataValue> list;
    list << filename << date << title << size << episodeName << showName << director << actor;
    return list;
}

QList<Preferences::MetaDataValue> Preferences::listMetaDataToDisplay(VideoFolderCategory type)
{
    QList<MetaDataValue> list;
    QSettings* settings = m_private.settings();
    QStringList values;

    switch (type) {
    case MyMovies:
        // The values are stored as comma separated numbers. The default is "2,1" (title and date).
        values = settings->value("MetaDataToDisplay/MyMovies", "2,1").toString().split(",");
        if (settings->value("MetaDataToDisplay/MyMovies", "2,1").toString().isEmpty()) {
            return list;
        }
        foreach(QString key, values) {
            list << (MetaDataValue)key.toInt();
        }
        break;

    case Films:
        // The values are stored as comma separated numbers. The default is "2,6" (title and director).
        values = settings->value("MetaDataToDisplay/Films", "2,6").toString().split(",");
        if (settings->value("MetaDataToDisplay/Films", "2,6").toString().isEmpty()) {
            return list;
        }
        foreach(QString key, values) {
            list << (MetaDataValue)key.toInt();
        }
        break;

    case TVShows:
        // The values are stored as comma separated numbers. The default is "4,5" (episodeName and showName).
        values = settings->value("MetaDataToDisplay/TVShows", "4,5").toString().split(",");
        if (settings->value("MetaDataToDisplay/TVShows", "4,5").toString().isEmpty()) {
            return list;
        }
        foreach(QString key, values) {
            list << (MetaDataValue)key.toInt();
        }
        break;

    default:
        break;
    }

    return list;
}

void Preferences::setListMetaDataToDisplay(VideoFolderCategory type, const QList<MetaDataValue> &values)
{
    // The values are stored as comma separated numbers.
    QStringList list;
    foreach (MetaDataValue val, values) {
        list << QString::number((int)val);
    }

    QSettings* settings = m_private.settings();
    switch (type) {
    case MyMovies:
        settings->setValue("MetaDataToDisplay/MyMovies", list.join(","));
        break;

    case Films:
        settings->setValue("MetaDataToDisplay/Films", list.join(","));
        break;

    case TVShows:
        settings->setValue("MetaDataToDisplay/TVShows", list.join(","));
        break;

    default:
        break;
    }
}

int Preferences::controlsFadeOutTime()
{
    QSettings* settings = m_private.settings();
    return settings->value("ControlsFadeOutTime", 3000).toInt();
}

void Preferences::setControlsFadeOutTime(int ms)
{
    QSettings* settings = m_private.settings();
    settings->setValue("ControlsFadeOutTime", ms);
}

qlonglong Preferences::lastPlaybackPosition(const QString &filename)
{
    QSettings* settings = m_private.settings();
    return settings->value("LastPlaybackPosition/" + filename, -1).toLongLong();
}

void Preferences::setLastPlaybackPosition(const QString &filename, qlonglong frame)
{
    QSettings* settings = m_private.settings();

    // Frame of -1 means remove saved position.
    if (frame == -1) {
        settings->remove("LastPlaybackPosition/" + filename);
    } else {
        settings->setValue("LastPlaybackPosition/" + filename, frame);
    }
}

QSize Preferences::thumbnailSize()
{
    return QSize(110, 110);
}

QSize Preferences::screenSize()
{
    return QSize(QApplication::desktop()->width(), QApplication::desktop()->height());
}

void Preferences::setCurrentCategory(VideoFolderCategory category)
{
    QSettings* settings = m_private.settings();
    settings->setValue("CurrentCategory", static_cast<int>(category));
}

Preferences::VideoFolderCategory Preferences::currentCategory()
{
    QSettings* settings = m_private.settings();
    return static_cast<VideoFolderCategory>(settings->value("CurrentCategory", MyMovies).toInt());
}

void Preferences::setCurrentFilter(int filter)
{
    QSettings* settings = m_private.settings();
    settings->setValue("CurrentFilter", filter);
}

int Preferences::currentFilter()
{
    QSettings* settings = m_private.settings();
    return settings->value("CurrentFilter", AppVideosPage::All).toInt();
}

void Preferences::setLastPlayedDate(const QString &filePath, const QDateTime & date)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    info.setLastPlayedDate(date);
    settings->setValue("VideoFileInfo/" + filePath, info.toString());
}

QDateTime Preferences::lastPlayedDate(const QString &filePath)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    return info.lastPlayedDate();
}

void Preferences::setFavorite(const QString &filePath, bool favorite)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    info.setFavorite(favorite);
    settings->setValue("VideoFileInfo/" + filePath, info.toString());
}

bool Preferences::isFavorite(const QString &filePath)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    return info.isFavorite();
}

void Preferences::setDateAdded(const QString &filePath, const QDateTime & date)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    info.setDateAdded(date);
    settings->setValue("VideoFileInfo/" + filePath, info.toString());
}

QDateTime Preferences::dateAdded(const QString &filePath)
{
    QSettings* settings = m_private.settings();
    QString str = settings->value("VideoFileInfo/" + filePath).toString();
    VideoFileInfo info = VideoFileInfo::fromString(str);
    return info.dateAdded();
}

void Preferences::setShowContextMenuDelay(int deley)
{
    QSettings* settings = m_private.settings();
    settings->setValue("ShowContextMenuDelay", deley);
}

int Preferences::showContextMenuDelay()
{
    QSettings* settings = m_private.settings();
    return settings->value("ShowContextMenuDelay", 500).toInt();
}

void Preferences::setLastSearchTerm(const QString &term)
{
    QSettings* settings = m_private.settings();
    settings->setValue("LastSearchTerm", term);
}

QString Preferences::lastSearchTerm()
{
    QSettings* settings = m_private.settings();
    return settings->value("LastSearchTerm", "").toString();
}

void Preferences::loadArguments(const QStringList & argList)
{
    if (argList.size() > 1) { // there are some arguments
        QString firstArg = argList.at(1);
        if (firstArg == "--loop") {
            m_launchMode = LoopMode;
        } else if (firstArg == "--from-list") {
            m_filesList = argList.mid(2);
            if (!m_filesList.isEmpty()) {
                m_launchMode = FromListMode;
            } else {
                std::cerr << "Warning: Files list for --from-list parameter expected. "
                             "The application will be launched in normal mode.\n";
            }
        } else {
            std::cerr << "Warning: Unrecognized options.\n";
            //printHelp();
        }
    }
}

QStringList Preferences::filesList()
{
    return m_filesList;
}

Preferences::LaunchMode Preferences::launchMode()
{
    return m_launchMode;
}

void Preferences::saveMetaInfo(const QHash<QString, MetaInfo *> &mi)
{
    QSettings* settings = m_private.settings();
    settings->beginGroup("MetaInfo");

    int counter = 0;

    QHashIterator<QString, MetaInfo *> it(mi);
    while (it.hasNext()) {
        it.next();
        settings->beginGroup(QString::number(counter++));
        MetaInfo *info = it.value();

        settings->setValue("Path", info->filePath());
        settings->setValue("Title", info->title());
        settings->setValue("EpisodeName", info->episodeName());
        settings->setValue("Director", info->director());
        settings->setValue("ShowName", info->showName());
        settings->setValue("FilmDescription", info->description());
        settings->setValue("Format", info->format());
        settings->setValue("Copyright", info->copyright());
        settings->setValue("Thumbnail", info->thumbnail());
        settings->setValue("Length", info->length());
        settings->setValue("Date", info->date());
        settings->setValue("Size", info->size());
        settings->setValue("Width", info->width());
        settings->setValue("Height", info->height());

        settings->endGroup();
    }

    settings->endGroup();
}

bool Preferences::loadMetaInfo(QHash<QString, MetaInfo *> &mi)
{
    QSettings* settings = m_private.settings();
    settings->beginGroup("MetaInfo");
    QStringList keys = settings->childGroups();

    for (int i = 0; i < keys.size(); ++i) {
        QString key = keys.at(i);
        settings->beginGroup(key);

        QString path = settings->value("Path").toString();

        if (!QFile::exists(path))
            continue;

        MetaInfo *info = new MetaInfo();

        info->setFilePath(path);
        info->setTitle(settings->value("Title").toString());
        info->setEpisodeName(settings->value("EpisodeName").toString());
        info->setDirector(settings->value("Director").toString());
        info->setShowName(settings->value("ShowName").toString());
        info->setDescription(settings->value("FilmDescription").toString());
        info->setFormat(settings->value("Format").toString());
        info->setCopyright(settings->value("Copyright").toString());
        info->setThumbnail(settings->value("Thumbnail").toString());
        info->setLength(settings->value("Length").toString());
        info->setDate(settings->value("Date").toDateTime());
        info->setSize(settings->value("Size").toInt());
        info->setWidth(settings->value("Width").toInt());
        info->setHeight(settings->value("Height").toInt());
        settings->endGroup();

        mi.insert(path, info);
    }


    settings->endGroup();

    return true;
}

bool Preferences::isDisplayMPlayerMessageEnabled()
{
    QSettings* settings = m_private.settings();
    return settings->value("MPlayerMessageEnabled", true).toBool();
}

void Preferences::setDisplayMPlayerMessageEnabled(bool enabled)
{
    QSettings* settings = m_private.settings();
    settings->setValue("MPlayerMessageEnabled", enabled);
}

void Preferences::setLastViewedPage(const int page)
{
    QSettings* settings = m_private.settings();
    settings->setValue("LastViewedPage", page);
    settings->sync();
}

int Preferences::lastViewedPage()
{
    QSettings* settings = m_private.settings();
    return settings->value("LastViewedPage", 0).toInt();
}
