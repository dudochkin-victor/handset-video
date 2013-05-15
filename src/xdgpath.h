/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef XDGPATH_H
#define XDGPATH_H

#include <QString>
#include <QStringList>

class QProcess;

class XDGPath : public QObject
{
    Q_OBJECT
public:
    XDGPath();
    ~XDGPath();

    QString getXDGVideosPath();
private slots:
    void readProcessOutput();
private:
    QString m_xdgPath;
    QProcess *m_p;
    bool m_usedProcess;
};

#endif
