/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOSLISTPROXYMODEL_H
#define VIDEOSLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

/// @class: VideoListProxyModel
/// @brief Item proxy model for video list
///
/// This class implements the proxy item model for the video list.

class VideosListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /// @brief Constructor
    /// @param parent QObject The parent
    VideosListProxyModel(QObject *parent = 0);

    /// @brief Destructor
    ~VideosListProxyModel();

    /// @brief Set search term
    /// @param searchTerm QString The search term
    void setSearchTerm(const QString &searchTerm);

    /// @brief Get search term
    /// @return QString The search term
    QString getSearchTerm () const {return m_searchTerm; }

protected:
    /// @brief Returns true if the item in the row should be included in the model; otherwise returns false.
    /// @param sourceRow int Specifies the source row index.
    /// @param sourceParent QModelIndex Specifies the source parent index.
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    /// @brief Returns true if the value of the first item is less than the value of the second item; otherwise returns false.
    /// @param first QModelIndex Specifies the first item index.
    /// @param second QModelIndex Specifies the second item index.
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString m_searchTerm;
};

#endif
