/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QApplication>
#include <QProcess>
#include <QFileInfo>
#include <QtDebug>
#include <QDir>
#include <QCryptographicHash>
#include <QtDBus/QtDBus>

#include "thumbnailmanagerthread.h"
#include "thumbnailer.h"
#include "thumbnailmanager.h"
#include "metainfo.h"
#include "videoproperty.h"

ThumbnailManagerThread::ThumbnailManagerThread(QObject *parent)
    : QThread(parent),
    m_exitFlag(false)
{
}

ThumbnailManagerThread::~ThumbnailManagerThread()
{
    exitThread();
}

void ThumbnailManagerThread::exitThread()
{
    if (!isRunning())
        return;

    m_mutex.lock();
    m_exitFlag = true;
    m_mutex.unlock();

    wait();
}

bool ThumbnailManagerThread::checkExitFlag()
{
    QMutexLocker locker(&m_mutex);
    return m_exitFlag;
}

QString ThumbnailManagerThread::getThumbnailPath(const QString &filePath, QByteArray &md5Result)
{
    QString path = QString("file://") + filePath;
    QString retStr;
    //use its Utf8 to perform md5
    md5Result = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5);
    retStr = ThumbnailManager::thumbnailFolder + QDir::separator() + md5Result.toHex() + ThumbnailManager::thumbnailSuffix;
    return retStr;
}

void ThumbnailManagerThread::run()
{
    Thumbnailer *thumbnailer = Thumbnailer::instance();
    for (int i = 0; i < m_videoFiles.size() && !checkExitFlag(); ++i) {
        QString fileName = m_videoFiles.at(i);
        QFileInfo fi(fileName);
        QString thumbnailPath;
        QString mimeType;
        QByteArray md5Result;

#if 0
        // Get thumbnails using mplayer - code disabled as it now uses Qt Tracker
        QStringList arguments;
        arguments << "-nosound";
        arguments << "-vo" << "jpeg:outdir=" + outDir;
        arguments << "-frames" << "1"; // Thumbnail frame is arbitrarily taken 1 second into video
        arguments << "-ss" << "1";
        arguments << "-vf" << "scale=110:110";
        arguments << fileName;

        QProcess mplayer;
        mplayer.start("mplayer", arguments);

        if (!mplayer.waitForFinished())
           return;

        if (QFile::exists(outDir + QString("00000001.jpg"))) {
            QFile::remove(outDir + fi.fileName() + QString(".jpg"));
            thumbnailPath = outDir + fi.fileName() + QString(".jpg");
            if (!QFile::rename(outDir + QString("00000001.jpg"), thumbnailPath))
                thumbnailPath.clear();
        }

        //get file metadata using mplayer
        arguments.clear();
        arguments << "-nosound";
        arguments << "-nojoystick";
        arguments << "-nolirc";
        arguments << "-nomouseinput";
        arguments << "-vo" << "null";
        arguments << "-ao" << "null";
        arguments << "-frames" << "0";
        arguments << "-msglevel" << "identify=6";
        arguments << fileName;

        mplayer.start("mplayer", arguments);

        if (!mplayer.waitForFinished())
           return;

        QByteArray output = mplayer.readAll();
        QTextStream in(&output);

        QRegExp rx_name("ID_CLIP_INFO_NAME*=");
        rx_name.setPatternSyntax(QRegExp::Wildcard);

        QRegExp rx_value("ID_CLIP_INFO_VALUE*=");
        rx_value.setPatternSyntax(QRegExp::Wildcard);
#endif

        thumbnailPath = getThumbnailPath(fileName, md5Result);
        mimeType = VideoProperty::getMimeType(fileName);

        // Some thumbnails may have not been created yet
        QFile f;
        f.setFileName(thumbnailPath);
        if (!f.exists()) {
            //request thumbnail from Tumbler
            ThumbnailManager::thumbnailDir.setNameFilters(QStringList(QString(md5Result.toHex()) + "*"));

            // Does not found even failed thumbnails
            if ((ThumbnailManager::thumbnailDir.entryList()).length() == 0) {
                thumbnailer->requestThumbnail(QString(("file://" + fileName)), mimeType);
            }
        }

        MetaInfo mi;
        mi.setFilePath(fileName);
        mi.setThumbnail(thumbnailPath);
        mi.setTitle(fi.baseName());
        mi.setEpisodeName(mi.title());
        mi.setDate(fi.created());
        mi.setSize(fi.size());
        mi.setShowName(tr("Unknown"));
        mi.setDirector(tr("Unknown"));
        mi.setLength(tr("Unknown"));
        mi.setHeight(0);
        mi.setWidth(0);
        mi.setFormat(tr("Unknown"));
        mi.setDescription(tr("Unknown"));
        mi.setCopyright(tr("Unknown"));

#if 0
        QString field;

        while (!in.atEnd()) {
            QString line = in.readLine();
            line = line.trimmed();

            if (line.contains("ID_VIDEO_FORMAT=")) {
                line.remove("ID_VIDEO_FORMAT=");
                 mi.setFormat(line);
            }
            else if (line.contains("ID_VIDEO_HEIGHT=")) {
                line.remove("ID_VIDEO_HEIGHT=");
                mi.setHeight(line.toInt());
            }
            else if (line.contains("ID_VIDEO_WIDTH=")) {
                line.remove("ID_VIDEO_WIDTH=");
                mi.setWidth(line.toInt());
            }
            else if (line.contains("ID_LENGTH=")) {
                line.remove("ID_LENGTH=");
                const int sec = static_cast<int>(line.toDouble());
                mi.setLength(QString("%1:%2:%3").arg(int(sec / 3600))
                              .arg(QString::number(int(sec / 60)), 2, QLatin1Char('0'))
                              .arg(QString::number(sec % 60), 2, QLatin1Char('0')));
            }
            else if (line.contains(rx_name)) {
                line.remove(rx_name);
                field = line;
            }
            else if (line.contains(rx_value)) {
                line.remove(rx_value);
                if (!field.isNull()) {
                    if (!field.compare("name", Qt::CaseInsensitive)) {
                        mi.setDirector(line);
                        mi.setShowName(mi.director());
                        mi.setDescription(mi.director());
                    }
                    else if (!field.compare("comments", Qt::CaseInsensitive)) {
                        mi.setDirector(line);
                        mi.setShowName(mi.director());
                        mi.setDescription(mi.director());
                    }
                    else if (!field.compare("copyright", Qt::CaseInsensitive))
                        mi.setCopyright(line);

                    field.clear();
                }
            }
        }
#endif
        qApp->postEvent(ThumbnailManager::instance(), new MetaInfoChangedEvent(mi));
    }
}
