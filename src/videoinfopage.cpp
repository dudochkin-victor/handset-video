/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "videoinfopage.h"
#include "videoinfowidget.h"


VideoInfoPage::VideoInfoPage(QGraphicsItem *parent) :
        MApplicationPage(parent)
{
    MApplicationPage::createContent();

    m_centralWidget = new VideoInfoWidget(this);
    setContentsMargins(0, 0, 0, 0);

    setCentralWidget(m_centralWidget);
    setTitle(qtTrId("meego_video_title"));
}

VideoInfoPage::~VideoInfoPage()
{
    delete m_centralWidget;
}

void VideoInfoPage::showVideoInfo(const QModelIndex &index)
{
    m_centralWidget->showVideoInfo(index);
}
