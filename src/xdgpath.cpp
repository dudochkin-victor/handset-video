/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QProcess>
#include <QtDebug>
#include <QDir>
#include "xdgpath.h"

XDGPath::XDGPath()
{
    m_p = new QProcess();
    m_xdgPath.clear();
    m_usedProcess = false;
    connect(m_p, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
}

XDGPath::~XDGPath()
{
    delete m_p;
}

void XDGPath::readProcessOutput()
{
    QByteArray b = m_p->readAllStandardOutput();
    m_xdgPath = QString(b).simplified();

    qDebug() << "XDG result is " << m_xdgPath;
}

QString XDGPath::getXDGVideosPath()
{
    if (!m_usedProcess) {
        m_usedProcess = true;
        QString command = "xdg-user-dir";
        QStringList args;
        args <<  "VIDEOS";
        m_p->start(command, args);
        m_p->waitForFinished(2000);

        if (m_xdgPath.isEmpty())
            m_xdgPath = (QDir::homePath() + QString("/Videos"));
        return m_xdgPath;
    } else {
        return m_xdgPath;
    }
}
