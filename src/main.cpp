/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <MApplication>
#include <MApplicationWindow>

#include <QtDBus>

#include "appwindow.h"

int main(int argc, char *argv[])
{
    MApplication app(argc, argv);
    app.setOrganizationName("/com/meego/meegovideo");

    bool isSingleVideoMode = false;
    for (int i = 0; i < argc; i++) {
        if (QString(argv[i]) == "-single-video") {
            isSingleVideoMode = true;
        }
    }

    AppWindow w(isSingleVideoMode);
    w.show();
    QDBusConnection::sessionBus().registerService("com.meego.video");
    QDBusConnection::sessionBus().registerObject("/", &w);

    return app.exec();
}
