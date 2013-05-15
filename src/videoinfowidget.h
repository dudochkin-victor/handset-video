/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOINFOWIDGET_H
#define VIDEOINFOWIDGET_H

#include <MStylableWidget>
#include <QModelIndex>

class MLabel;
class MImageWidget;

class VideoInfoWidget : public MStylableWidget
{
    Q_OBJECT
public:
    VideoInfoWidget(QGraphicsItem *parent = 0);
    virtual ~VideoInfoWidget();

    void showVideoInfo(const QModelIndex &index);

private:
    MLabel *m_videoTitle_EpisodeName;
    MLabel *m_dataCreated_director_showName;

    MLabel *m_videoFileSize;
    MLabel *m_bitRate;
    MLabel *m_videoCodec;

    MLabel *m_dateCreated;
    MLabel *m_lastViewed;
    MLabel *m_synopsis;

    MImageWidget *m_cover;
};



#endif
