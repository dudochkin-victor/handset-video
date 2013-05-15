/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOSLIST_H
#define VIDEOSLIST_H

#include <MStylableWidget>
#include <MWidgetStyle>
#include <MList>

#include "appwindow.h"

class VideosListItemCreator;
class VideosListModel;
class VideosListProxyModel;

/// @class VideosList
/// @brief Class to list video items for different category
///
/// This class implements list view for video items

class VideosList : public MStylableWidget
{
    Q_OBJECT;

public:
    /// @brief Constructor
    /// @param category AppWindow::PlayerCategory The category
    /// @param parent QGraphicsItem The parent
    VideosList(const AppWindow::PlayerCategory &category, QGraphicsItem *parent = 0);

    /// @brief Destructor
    ~VideosList();

    /// @brief Return internal MList
    /// @return MList* The internal MList
    MList *getMList() {return m_list;}

    /// @brief Scroll to the letter
    /// @param letter QString The letter to scroll to
    void scrollTo(const QString &letter);

    /// @brief Update for recently viewed, intended to be called after some video been played
    void updateRecentlyViewed();

    /// @brief Update for favorites, intended to be called after favorite action inside context menu is called
    void updateFavourites();

    QString firstVideoPath();

signals:
    /// @brief Signal Emitted when a video item is clicked
    /// @param filePath QString The file path of the video
    void videoItemClicked(const QString &filePath);

    void videoInfoClicked(const QModelIndex &index);

private slots:
    void queueTasks();
    void dequeueTasks();
    void handleItemCreated(const QModelIndex &index, MWidget *widget);
    void itemClickedHandler(QModelIndex index);
    void handleActionPlay();
    void handleActionFavourite();
    void handleActionInfo();
    void handleActionDelete();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
private:
    AppWindow::PlayerCategory m_category;
    M_STYLABLE_WIDGET(MWidgetStyle);

    MList *m_list;
    VideosListItemCreator *m_itemCreator;
    VideosListModel *m_listModel;
    VideosListProxyModel *m_listProxyModel;

    MAction *m_actionPlay;
    MAction *m_actionFavourite;
    MAction *m_actionInfo;
    MAction *m_actionDelete;
    QMap<MWidget *, QModelIndex> m_listItems;
    MWidget *m_actionWidget;
};

#endif // VIDEOSLIST_H
