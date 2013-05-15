/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef THUMBNAILMANAGERTHREAD_H
#define THUMBNAILMANAGERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QMutex>

/// @class: ThumbnailManagerThread
/// @brief Helper class for generation video file thumbnail images.

class ThumbnailManagerThread : public QThread
{
    Q_OBJECT
public:
    /// @brief Constructor
    /// @param parent QObject Option parent object
    ThumbnailManagerThread(QObject *parent = 0);

    /// @brief Destructor
    ~ThumbnailManagerThread();

    /// @brief Set list of the video file
    /// @param files QStringList List of fully qualified path to the video file
    void setVideoFiles(const QStringList &files) {m_videoFiles = files;}

    /// @brief Return list of the video file
    /// @return QStringList The list of fully qualified path to the video file
    QStringList videoFiles() const {return m_videoFiles;}

    /// @brief Return thumbnail path of the video file
    /// @param filePath const QString& The path of the video file
    /// @param md5Result QByteArray& The calculated md5 result of the video file
    /// @return QString The path of thumbnail of the video file
    QString getThumbnailPath(const QString &filePath, QByteArray &md5Result);

public slots:
    /// @brief Exit from the thread (slot)
    void exitThread();

protected:
    /// @brief The starting point for the thread
    virtual void run();

private:
    /// @brief Return the state of exit flag
    bool checkExitFlag();

private:
    QStringList m_videoFiles;
    QMutex m_mutex;
    bool m_exitFlag;
};

#endif // THUMBNAILMANAGERTHREAD_H
