/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QPixmap>
#include <QStringList>
#include <QFileInfo>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QtDebug>

#include <MAbstractCellCreator>

#include "thumbnailmanager.h"
#include "preferences.h"
#include "metainfo.h"
#include "errormessage.h"
#include "thumbnailer.h"
#include "videoproperty.h"
#include "appwindow.h"
#include "videoslist.h"

ThumbnailManager *ThumbnailManager::m_instance = 0;
QString ThumbnailManager::thumbnailFolder = QDir::toNativeSeparators(QDir::homePath()) + \
                                                  QDir::separator() + QString(".thumbnails") + \
                                                  QDir::separator() + QString("normal");
QString ThumbnailManager::thumbnailSuffix = QString(".png");
QString ThumbnailManager::thumbnailSuffix1 = QString(".jpeg");

QDir ThumbnailManager::thumbnailDir = QDir(ThumbnailManager::thumbnailFolder);

ThumbnailManager::ThumbnailManager()
{
    // Do cleanup before application exits.
    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(cleanup()));
    QObject::connect(Thumbnailer::instance(), SIGNAL(ready(uint,QStringList)), this, SLOT(handleThumbnailReady(uint,QStringList)));
    // Create the directories, if necessary
    createDirectories();
    ////????????
    //MTheme::addPixmapDirectory("/usr/share/themes/base/meegotouch/icons");

    m_defaultThumbnailPic = *MTheme::pixmapCopy("icon-m-content-video");
    m_defaultThumbnailScaled = false;

    //checkMplayerExists();
}


ThumbnailManager::~ThumbnailManager()
{
    qDeleteAll(m_metaInfoCache);
    m_metaInfoCache.clear();
}

ThumbnailManager *ThumbnailManager::instance()
{

    if (m_instance == 0) {
        m_instance = new ThumbnailManager;
        m_instance->loadMetaInfo();
    }

    return m_instance;
}

QStringList ThumbnailManager::thumbnailPaths(const QString &file, QByteArray &md5Result)
{
    QString path = QString("file://") + file;
    QString retStr;
    QStringList ret;
    //use its Utf8 to perform md5
    md5Result = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5);
    retStr = thumbnailFolder + QDir::separator() + md5Result.toHex() + thumbnailSuffix;
    ret << retStr;
    retStr = thumbnailFolder + QDir::separator() + md5Result.toHex() + thumbnailSuffix1;
    ret << retStr;
    return ret;
}

QString ThumbnailManager::thumbnailExistPath(const QString &file, QByteArray &md5Result)
{
    QStringList thumbs;
    QString ret = "";

    thumbs = thumbnailPaths(file, md5Result);

    for (QStringList::iterator i = thumbs.begin(); i != thumbs.end(); i++) {
        QFileInfo info(*i);
        if (info.exists()) {
            ret = *i;
            break;
        }
    }
    return ret;
}

void ThumbnailManager::handleThumbnailReady(const unsigned int &handle, const QStringList &urls)
{
    Q_UNUSED(handle);

    for (QStringList::const_iterator i = urls.constBegin(); i != urls.constEnd(); ++i) {
        QString tmp(*i);
        QString videoFile = tmp.remove(QString("file://"));
        MetaInfo *mi = m_metaInfoCache.value(videoFile);
        if (mi) {
            QByteArray md5Result;
            QString thumbnail = thumbnailExistPath(videoFile, md5Result);
            if (thumbnail != QString("")) {
                mi->setThumbnailPic(QPixmap(thumbnail));
                mi->setThumbnailLoaded(TRUE);
                mi->setThumbnailScaled(false);
            }
        }
        emit metainformationChanged(QString((tmp.remove(QString("file://")))));
    }
}

void ThumbnailManager::cleanup()
{
    if (m_instance != 0) {
        m_instance->saveMetaInfo();
        delete m_instance;
        m_instance = 0;
    }
}

void ThumbnailManager::createDirectories()
{
    QDir dir;
    dir.mkpath(Preferences::folderPath(Preferences::MyMovies) + QString("/.thumbnails"));
    dir.mkpath(Preferences::folderPath(Preferences::Films) + QString("/.thumbnails"));
    dir.mkpath(Preferences::folderPath(Preferences::TVShows) + QString("/.thumbnails"));
}
/*
void ThumbnailManager::checkMplayerExists()
{
    QProcess mplayer;
    mplayer.start("mplayer");
    m_isMplayerExists = mplayer.waitForStarted();
    mplayer.waitForFinished();
}*/

QPixmap ThumbnailManager::thumbnail(const QString &videoFile, int scaledWidth, int scaledHeight)
{
    qDebug() << "ThumbnailManager get thumbnail with scale " << scaledWidth << scaledHeight;
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (!mi) {
        qDebug() << "NO MetaInfo for " << videoFile << " return default thumbnail";
        if (!m_defaultThumbnailScaled) {
            m_defaultThumbnailScaled = true;
            m_defaultThumbnailPic = m_defaultThumbnailPic.scaled(scaledWidth, scaledHeight);
        }
        return m_defaultThumbnailPic;
    }
    else {
        if (mi->thumbnailLoaded()) {
            qDebug() << "Thumbnail loaded for " << videoFile;
            if (mi->thumbnailScaled()) {
                return mi->thumbnailPic();
            } else {
                qDebug() <<  videoFile << " Scale thumbnail to " << scaledWidth << " " << scaledHeight;
                mi->setThumbnailPic(mi->thumbnailPic().scaled(scaledWidth, scaledHeight));
                mi->setThumbnailScaled(true);
                return mi->thumbnailPic();
            }
        }
        else {
            qDebug() << "Thumbnail NOT loaded for " << videoFile << " return default thumbnail";
            if (!m_defaultThumbnailScaled) {
                m_defaultThumbnailScaled = true;
                m_defaultThumbnailPic = m_defaultThumbnailPic.scaled(scaledWidth, scaledHeight);
            }
            return m_defaultThumbnailPic;
        }
    }
}

QString ThumbnailManager::title(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->title();

    return QFileInfo(videoFile).baseName();
}

QDateTime ThumbnailManager::date(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->date();

    return QDateTime();
}

QString ThumbnailManager::episodeName(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->episodeName();

    return QFileInfo(videoFile).baseName();
}

QString ThumbnailManager::director(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->director();

    return tr("Unknown");
}

QString ThumbnailManager::showName(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->showName();

    return tr("Unknown");
}

QString ThumbnailManager::filmDescription(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->description();

    return tr("Unknown");
}

QString ThumbnailManager::format(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->format();

    return tr("Unknown");
}

QString ThumbnailManager::copyright(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->copyright();

    return tr("Unknown");
}

qint64 ThumbnailManager::size(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->size();

    return QFileInfo(videoFile).size();
}

int ThumbnailManager::width(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->width();

    return 0;
}

int ThumbnailManager::height(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->height();

    return 0;
}


QString ThumbnailManager::length(const QString &videoFile) const
{
    MetaInfo *mi = m_metaInfoCache.value(videoFile);
    if (mi)
        return mi->length();

    return tr("Unknown");
}

/*
void ThumbnailManager::handleCategoryChanged(const QString &path)
{
    //if (!m_isMplayerExists)
    //    return;

    QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
    QStringList files;

    while (it.hasNext()) {
        QString videoFile = it.next();
        QFileInfo video = it.fileInfo();
        QDateTime videoFileDate = video.created();

        //TODO: check if  this file really video file

        MetaInfo *mi = m_metaInfoCache.value(videoFile);
        bool needUpdate = false;

        if (!mi)
            needUpdate = true;
        else {
            QFileInfo thumbnail(mi->thumbnail());
            QDateTime thumbnailFileDate = thumbnail.created();
            if (videoFileDate > thumbnailFileDate)
                needUpdate = true;
        }

        QDateTime date = Preferences::dateAdded(videoFile);
        if (!date.isValid())
            Preferences::setDateAdded(videoFile, QDateTime::currentDateTime());

        if (needUpdate)
            files.append(videoFile);
    }

    updateThumbnails(files);
}

void ThumbnailManager::handleFileChanged(const QString &path)
{
    updateThumbnails(QStringList() << path);
}

void ThumbnailManager::handleFileRemoved(const QString &path)
{
    delete m_metaInfoCache.take(path);

    QFileInfo video(path);
    QString thumbnailFile = video.absolutePath() + "/.thumbnails/" + video.fileName() + ".jpg";

    if (QFile::exists(thumbnailFile))
        QFile::remove(thumbnailFile);
}
*/
/*void ThumbnailManager::handleFileAdded(const QString &path)
{
    updateThumbnails(QStringList() << path);
}

void ThumbnailManager::updateThumbnails(const QStringList &files)
{
    //if (!m_isMplayerExists)
    //    return;

    ThumbnailManagerThread *thread = new ThumbnailManagerThread(this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(destroyed(QObject *)), thread, SLOT(exitThread()));

    thread->setVideoFiles(files);
    thread->start();
}*/

bool ThumbnailManager::event(QEvent *event)
{
    if (event->type() == static_cast<QEvent::Type>(MetaInfoChangedEvent::MetaInfoChanged)) {
        MetaInfoChangedEvent *mie = static_cast<MetaInfoChangedEvent *>(event);
        const MetaInfo &mi = mie->metaInformation();

        delete m_metaInfoCache.take(mi.filePath());
        m_metaInfoCache.insert(mi.filePath(), new MetaInfo(mi));

        emit metainformationChanged(mi.filePath());

        return true;
    }

    return QObject::event(event);
}

void ThumbnailManager::loadMetaInfo()
{
    //Preferences::loadMetaInfo(m_metaInfoCache);
}

void ThumbnailManager::saveMetaInfo()
{
    //Preferences::saveMetaInfo(m_metaInfoCache);
}

bool ThumbnailManager::metainformationExsit(const QString &file)
{
    MetaInfo *mi = m_metaInfoCache.value(file);
    if (!mi)
        return FALSE;
    else
        return TRUE;
}

void ThumbnailManager::fillMetaInfo(const QString &file)
{
    MetaInfo *mi = new MetaInfo;
    QFileInfo fileInfo = QFileInfo(file);
    QByteArray md5Result;
    QString thumbnail = thumbnailExistPath(file, md5Result);
    QFileInfo thumbnailInfo = QFileInfo(thumbnail);

    mi->setFilePath(file);
    mi->setThumbnail(thumbnail);
    mi->setTitle(fileInfo.baseName());
    mi->setEpisodeName(fileInfo.baseName());
    mi->setDate(fileInfo.created());
    mi->setSize(fileInfo.size());
    mi->setShowName(tr("Unknown"));
    mi->setDirector(tr("Unknown"));
    mi->setLength(tr("Unknown"));
    mi->setHeight(0);
    mi->setWidth(0);
    mi->setFormat(tr("Unknown"));
    mi->setDescription(tr("Unknown"));
    mi->setCopyright(tr("Unknown"));

    if (thumbnailInfo.exists()) {
        qDebug() << file << "Thumbnail Exists at " << thumbnail;
        mi->setThumbnailPic(QPixmap(thumbnail));
        mi->setThumbnailLoaded(TRUE);
        mi->setThumbnailScaled(false);
    } else {
        qDebug() << file << "Thumbnail NOT Exists at " << thumbnail;
        mi->setThumbnailPic(m_defaultThumbnailPic);

        /* start to request thumbnail */
        Thumbnailer *thumbnailer = Thumbnailer::instance();
        QString mimeType = VideoProperty::getMimeType(file);
        thumbnailDir.setNameFilters(QStringList(QString(md5Result.toHex()) + "*"));

        // Does not found even failed thumbnails
        if ((thumbnailDir.entryList()).length() == 0) {
            qDebug() << "Request Thumbnail for " << QString(("file://" + file)) << " " << mimeType;
            thumbnailer->requestThumbnail(QString(("file://" + file)), mimeType);
        }
    }
    m_metaInfoCache.insert(file, mi);

    emit metainformationChanged(file);
}
