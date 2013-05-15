/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef APP_VIDEOS_PAGE_H
#define APP_VIDEOS_PAGE_H

#include <QStringList>
#include <QGraphicsItem>
#include <QModelIndex>

#include <MStylableWidget>
#include <MWidgetStyle>
#include <MApplicationPage>

#include "appwindow.h"

class VideosList;

class AppVideosPageWidget : public MStylableWidget
{
    Q_OBJECT
public:

    AppVideosPageWidget(const AppWindow::PlayerCategory &category, QGraphicsItem *parent = 0);
    ~AppVideosPageWidget();

    VideosList *getVideosList() {return m_videosList;}

private:
    //QStringList m_videoFiles;
    M_STYLABLE_WIDGET(MWidgetStyle);
    AppWindow::PlayerCategory m_category;
    VideosList *m_videosList;
};

class AppVideosPage : public MApplicationPage
{
    Q_OBJECT
public:
    /// @brief Enumerated type for filters
    enum FilterType {
        All = 0,
        RecentlyAdded,
        RecentlyViewed,
        Favorites,
        Search
    };

    AppVideosPage(const AppWindow::PlayerCategory &category, QGraphicsItem *parent = 0);
    ~AppVideosPage();

    void updateRecentlyViewed();
    void updateFavourites();
    QString firstVideoPath();

signals:
    void videoItemClicked(const QString &filePath);
    void videoInfoClicked(const QModelIndex &index);

private:
    AppWindow::PlayerCategory m_category;
    AppVideosPageWidget *m_centralWidget;

private slots:
    void emitvideoItemClickedSignal(const QString &filePath);
    void emitvideoInfoClickedSignal(const QModelIndex &index);

public slots:
    void scrollTo(const QString &letter);
};

#endif
