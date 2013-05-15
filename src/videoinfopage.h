/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOINFOPAGE_H
#define VIDEOINFOPAGE_H

#include <MApplicationPage>
#include <QModelIndex>

class VideoInfoWidget;

class VideoInfoPage : public MApplicationPage
{
    Q_OBJECT
public:
    VideoInfoPage(QGraphicsItem *parent);
    virtual ~VideoInfoPage();

public slots:
    void showVideoInfo(const QModelIndex &index);

private:
    VideoInfoWidget *m_centralWidget;
};

#endif
