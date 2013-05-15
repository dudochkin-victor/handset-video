/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "thumbnailer.h"

const QString Thumbnailer::service = "org.freedesktop.thumbnails.Thumbnailer1";
const QString Thumbnailer::path = "/org/freedesktop/thumbnails/Thumbnailer1";
const char   *Thumbnailer::interface = "org.freedesktop.thumbnails.Thumbnailer1";

const QString Thumbnailer::default_flavor = "normal";
const QString Thumbnailer::default_schedular = "foreground";

Thumbnailer *Thumbnailer::thumbnailerInstance = NULL;


Thumbnailer *Thumbnailer::instance()
{
    if (thumbnailerInstance)
        return thumbnailerInstance;
    else {
        thumbnailerInstance = new Thumbnailer(QDBusConnection::sessionBus(), NULL);
        return thumbnailerInstance;
    }
}

void Thumbnailer::emitReadySignal(const unsigned int &handle, const QStringList &urls)
{
    emit ready(handle, urls);
}

Thumbnailer::Thumbnailer(const QDBusConnection &connection, QObject *parent) :
        QDBusAbstractInterface(service, path, interface, connection, parent)
{
    QDBusConnection::sessionBus().connect(service, path, interface,
                                    "Ready", this, SLOT(emitReadySignal(const unsigned int, const QStringList)));
}

Thumbnailer::~Thumbnailer()
{
}
