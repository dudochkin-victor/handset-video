/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QGraphicsLinearLayout>
#include <QDebug>
#include <MAction>
#include <MApplicationMenu>

#include "appvideospage.h"
#include "videoslist.h"
#include "appwindow.h"

AppVideosPageWidget::AppVideosPageWidget(const AppWindow::PlayerCategory &category, QGraphicsItem *parent)
    : MStylableWidget(parent), m_category(category)
{
    qDebug() << "AppVideosPageWidget::AppVideosPageWidget";

    m_videosList  = new VideosList(category, parent);

    setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addItem(m_videosList);
}


AppVideosPageWidget::~AppVideosPageWidget()
{
    delete m_videosList;
}

AppVideosPage::AppVideosPage(const AppWindow::PlayerCategory &category, QGraphicsItem *parent) :
        MApplicationPage(parent), m_category(category)
{
    MApplicationPage::createContent();

    m_centralWidget = new AppVideosPageWidget(m_category, this);
    connect(m_centralWidget->getVideosList(), SIGNAL(videoItemClicked(const QString&)), this, SLOT(emitvideoItemClickedSignal(const QString&)));
    connect(m_centralWidget->getVideosList(), SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(emitvideoInfoClickedSignal(const QModelIndex&)));

    //% "Video player"
    setTitle(qtTrId("meego_video_title"));

    switch(category) {
        case AppWindow::Showall: {

            break;
        }
        case AppWindow::MyMovies: {
            setObjectName("MyMoviesPage");
            break;
        }
        case AppWindow::Films: {
            setObjectName("MyFilmsPage");
            break;
        }
        case AppWindow::TVShows: {
            setObjectName("MyTVsPage");
            break;
        }
        case AppWindow::RecentlyAdded: {

            break;
        }
        case AppWindow::RecentlyViewed: {

            break;
        }
        case AppWindow::Favourites: {

            break;
        }
    }
    setContentsMargins(0, 0, 0, 0);

    setCentralWidget(m_centralWidget);
    setPannable(false);
}

AppVideosPage::~AppVideosPage()
{
    delete m_centralWidget;
}

void AppVideosPage::emitvideoItemClickedSignal(const QString &filePath)
{
    qDebug() << " AppVideosPage::emitvideoItemClickedSignal() " << filePath;
    emit(videoItemClicked(filePath));
}

void AppVideosPage::emitvideoInfoClickedSignal(const QModelIndex &index)
{
    qDebug() << " AppVideosPage::emitvideoInfoClickedSignal() ";
    emit(videoInfoClicked(index));
}

void AppVideosPage::scrollTo(const QString &letter)
{
    m_centralWidget->getVideosList()->scrollTo(letter);
}

void AppVideosPage::updateRecentlyViewed()
{
    Q_ASSERT(m_category == AppWindow::RecentlyViewed);

    m_centralWidget->getVideosList()->updateRecentlyViewed();
}

void AppVideosPage::updateFavourites()
{
    Q_ASSERT(m_category == AppWindow::Favourites);

    m_centralWidget->getVideosList()->updateFavourites();
}

QString AppVideosPage::firstVideoPath()
{
    return m_centralWidget->getVideosList()->firstVideoPath();
}
