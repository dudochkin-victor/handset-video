/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#include <QDebug>
#include <QModelIndex>
#include <QFile>
#include <QPixmap>

#include <MWidget>
#include <MImageWidget>
#include <MLabel>
#include <MContentItem>

#include "videoslistitemcreator.h"
#include "videoslistmodel.h"
#include "videoslistproxymodel.h"
#include "videoslist.h"

void VideosListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    qDebug() << "VideosListItemCreator::updateCell" << index.row();

    if (!cell)
        return;

    if (!index.isValid()) {
        qWarning() << "Invalid ModelIndex inside VideosListItemCreator::updateCell " << index.row();
        return;
    }

    QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
    if (!model) {
        qWarning() << "Invalid proxy model inside VideosListItemCreator::updateCell !!" << index.row();
        return;
    }

    VideosListProxyModel *proxy = qobject_cast<VideosListProxyModel *>(model);
    if (!proxy) {
        qWarning() << "Invalid proxy model inside VideosListItemCreator::updateCell " << index.row();
        return;
    }

    QModelIndex modelIndex = proxy->mapToSource(index);
    VideosListModel *videosListModel = qobject_cast<VideosListModel *> (proxy->sourceModel());

    qDebug() << "Index model is " << index.row() << "Source Model is " << modelIndex.row();
    const bool isFavourite = qvariant_cast<bool>(videosListModel->data(modelIndex, VideosListModel::IsFavoriteRole));

    static QPixmap marker =  (*MTheme::pixmap("icon-m-common-favorite-mark"));

    MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
    QString C1String = modelIndex.data(VideosListModel::CaptionLevel1Role).toString();
    QString C2String = modelIndex.data(VideosListModel::CaptionLevel2Role).toString();
    QPixmap pixmap = modelIndex.data(VideosListModel::ThumbnailRole).value<QPixmap>();
    contentItem->setTitle(C1String);
    contentItem->setSubtitle(C2String);
    contentItem->setPixmap(pixmap);
    contentItem->boundingRect();
    contentItem->setOptionalPixmap(isFavourite ? marker : QPixmap());
}

MWidget *VideosListItemCreator::createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
{
    MContentItem *cell = dynamic_cast<MContentItem *>(recycler.take(MDetailedListItem::staticMetaObject.className()));

    if (cell == NULL)
    {
        qDebug() << "No recycle, create new cell ";
        cell = new MContentItem(MContentItem::IconAndTwoTextLabels);
        cell->setObjectName("VideosItem");
        //cell->imageWidget()->setObjectName("ThumbnailImage");
        //cell->titleLabelWidget()->setObjectName("TitleLabelWidget");
        //cell->setViewType("VideosItemType");
    }

    QMetaObject::invokeMethod(m_videosList, "handleItemCreated", Qt::DirectConnection, Q_ARG(const QModelIndex &, index), Q_ARG(MWidget *, cell));

    updateCell(index, cell);
    return cell;
}

