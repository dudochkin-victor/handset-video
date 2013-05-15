/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOSLISTITEMCREATOR_H
#define VIDEOSLISTITEMCREATOR_H

#include <MWidget>
#include <QModelIndex>
#include <MDetailedListItem>
#include <MAbstractCellCreator>
#include <MWidgetRecycler>

#include "appwindow.h"

class VideosList;

/// @class: VideosListItemCreator
/// @brief Delegate to draw items in the video list.
///
/// This class is the delegate that does the drawing of items in the video list.
/// This includes the video thumbnail, metadata to be shown such as title and date,
/// and an indication of whether the item is selected.

class VideosListItemCreator : public MAbstractCellCreator<MDetailedListItem>
{
public:
    /// @brief Constructor
    /// @param category AppWindow::PlayerCategory The category
    /// @param videosList VideosList The video list that contains it
    VideosListItemCreator(const AppWindow::PlayerCategory &category, VideosList *videosList) { m_category = category; m_videosList = videosList;}

    /// @brief Update the drawing of cell
    /// @param index QModelIndex The index to update
    /// @param cell MWidget The MWidget to update
    virtual void updateCell(const QModelIndex& index, MWidget* cell) const;

    /// @brief Create a cell
    /// @param index QModelIndex The index to create
    /// @param recycler MWidgetRecycler The recycler to use
    /// @return MWidget The widget created
    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const;
private:
    AppWindow::PlayerCategory m_category;
    VideosList *m_videosList;
};

#endif
