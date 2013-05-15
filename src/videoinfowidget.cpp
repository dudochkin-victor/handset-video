/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <MLabel>
#include <MImageWidget>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QPixmap>
#include <QDateTime>

#include "videoinfowidget.h"
#include "videoslistmodel.h"
#include "preferences.h"

VideoInfoWidget::VideoInfoWidget(QGraphicsItem *parent) : MStylableWidget(parent)
{
    m_videoTitle_EpisodeName = new MLabel(this);
    m_dataCreated_director_showName = new MLabel(this);

    m_videoFileSize = new MLabel(this);
    m_bitRate = new MLabel(this);
    m_videoCodec = new MLabel(this);

    m_dateCreated = new MLabel(this);
    m_lastViewed = new MLabel(this);
    m_synopsis = new MLabel(this);

    m_cover = new MImageWidget(this);

    m_synopsis->setWordWrap(true);

    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);

    QGraphicsLinearLayout *thumbnailLayout = new QGraphicsLinearLayout(Qt::Vertical);
    thumbnailLayout->setSpacing(0);
    thumbnailLayout->setContentsMargins(0, 0, 0, 0);
    thumbnailLayout->addItem(m_cover);
    thumbnailLayout->addStretch();
    mainLayout->addItem(thumbnailLayout);
    mainLayout->addStretch();

    QGraphicsLinearLayout *centralLayout = new QGraphicsLinearLayout(Qt::Vertical);
    centralLayout->addItem(m_videoTitle_EpisodeName);
    centralLayout->addItem(m_dataCreated_director_showName);

    centralLayout->addItem(new MLabel(""));

    centralLayout->addItem(m_videoFileSize);
    centralLayout->addItem(m_bitRate);
    centralLayout->addItem(m_videoCodec);

    centralLayout->addItem(new MLabel(""));

    centralLayout->addItem(m_dateCreated);
    centralLayout->addItem(m_lastViewed);
    centralLayout->addItem(m_synopsis);

    mainLayout->addItem(centralLayout);
    mainLayout->setStretchFactor(thumbnailLayout, 0);
    mainLayout->setStretchFactor(centralLayout, 1);
}

VideoInfoWidget::~VideoInfoWidget()
{
    delete m_videoTitle_EpisodeName;
    delete m_dataCreated_director_showName;

    delete m_videoFileSize;
    delete m_bitRate;
    delete m_videoCodec;

    delete m_dateCreated;
    delete m_lastViewed;
    delete m_synopsis;

    delete m_cover;
}

void VideoInfoWidget::showVideoInfo(const QModelIndex &index)
{
    Q_UNUSED(index);

    QString videoTitle_EpisodeName;
    QString dataCreated_director_showName;

    QString videoFileSize;
    float fileSize;
    QString bitRate;
    QString videoCodec;

    QString dateCreated;
    QString lastViewed;
    QString synopsis;

    QPixmap thumb;

    Preferences::VideoFolderCategory category = static_cast<Preferences::VideoFolderCategory>(index.data(VideosListModel::VideoCategoryRole).toInt());
    switch (category) {
    case Preferences::MyMovies:
        videoTitle_EpisodeName = index.data(VideosListModel::TitleRole).toString();
        dataCreated_director_showName = index.data(VideosListModel::DateRole).toDateTime().toString("MMM d'th' yyyy");
        break;
    case Preferences::Films:
        videoTitle_EpisodeName = index.data(VideosListModel::TitleRole).toString();
        dataCreated_director_showName = index.data(VideosListModel::DirectorRole).toString();
        break;
    case Preferences::TVShows:
        videoTitle_EpisodeName = index.data(VideosListModel::EpisodeNameRole).toString();
        dataCreated_director_showName = index.data(VideosListModel::ShowNameRole).toString();
        break;
    default:
        break;
    }

    fileSize = index.data(VideosListModel::SizeRole).toFloat();
    fileSize = fileSize/1024/1024;
    videoFileSize.setNum(fileSize, 'g', 2);
    videoFileSize += QString("MB");

    bitRate = index.data(VideosListModel::BitRateRole).toString();
    videoCodec = index.data(VideosListModel::FormatRole).toString();

    dateCreated = index.data(VideosListModel::DateRole).toDateTime().toString("dd/MM/yyyy HH:mm");
    lastViewed = index.data(VideosListModel::LastReadRole).toDateTime().toString("dd/MM/yyyy");
    synopsis = index.data(VideosListModel::FilmDescriptionRole).toString();

    thumb = index.data(VideosListModel::ThumbnailRole).value<QPixmap>();

    m_videoTitle_EpisodeName->setText(videoTitle_EpisodeName);
    m_dataCreated_director_showName->setText(dataCreated_director_showName);

    m_videoFileSize->setText(qtTrId("Size: %1").arg(videoFileSize));
    m_bitRate->setText(qtTrId("BitRate: %1").arg(bitRate));
    m_videoCodec->setText(qtTrId("Video Codec: %1").arg(videoCodec));

    m_dateCreated->setText(qtTrId("Date Created: %1").arg(dateCreated));
    m_lastViewed->setText(qtTrId("Last Viewed: %1").arg(lastViewed));
    m_synopsis->setText(synopsis);

    m_cover->setImage(thumb.toImage());
}
