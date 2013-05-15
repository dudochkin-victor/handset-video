/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#include <QtDebug>

#include "videoslistproxymodel.h"
#include "videoslistmodel.h"

VideosListProxyModel::VideosListProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

VideosListProxyModel::~VideosListProxyModel()
{

}

void VideosListProxyModel::setSearchTerm(const QString &searchTerm)
{
    m_searchTerm = searchTerm;
}

bool VideosListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    qDebug() << "VideosListProxyModel::filterAcceptsRow " << source_row;

    VideosListModel *model = qobject_cast<VideosListModel *>(sourceModel());
    if (!model) {
        qWarning() << "Can't get a valid ListModel";
        return false;
    }

    QModelIndex index = model->index(source_row, 0, source_parent);
    if (!index.isValid()) {
        qWarning() << "Can't get a valid QModelIndex";
        return false;
    }

    /* Filter below */

    QString fileName = index.data(VideosListModel::TitleRole).toString();
    qDebug() << "VideosListProxyModel::filterAcceptsRow " << fileName << "Index row is " << index.row();
#if 1
    return true;
#else
    if (fileName == QString("Video_OGG_3") || fileName == QString("Video_OGG_8") || fileName == QString("2")
        || fileName == QString("Film1") || fileName ==QString("TV1")) {
        qDebug () << "Accepted " << fileName;
        return true;
    }
    else
        return false;
#endif
}

/* TODO: Make it support I18N I10N */
bool VideosListProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    //qDebug() << "VideosListProxyModel::lessThan " << left.row() << " " << right.row();

    VideosListModel *model = qobject_cast<VideosListModel *>(sourceModel());
    if (!model) {
        qWarning() << "Can't get a valid ListModel";
        return false;
    }

    QString leftFileName = left.data(VideosListModel::CaptionLevel1Role).toString();
    QString rightFileName = right.data(VideosListModel::CaptionLevel1Role).toString();

    if (QString::compare(leftFileName, rightFileName, Qt::CaseInsensitive) > 0)
        return false;
    else
        return true;
}

