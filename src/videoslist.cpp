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
#include <QGraphicsLinearLayout>
#include <QTimer>

#include <MList>
#include <MPannableViewport>
#include <MTheme>
#include <MMessageBox>

#include "videoslist.h"
#include "videoslistitemcreator.h"
#include "videoslistmodel.h"
#include "videoslistproxymodel.h"
#include "appwindow.h"

VideosList::VideosList(const AppWindow::PlayerCategory &category, QGraphicsItem *parent) :
        MStylableWidget(parent) , m_category(category)
{
    qDebug() << "VideosList::VideosList";

    setObjectName("VideosList");
    setContentsMargins(0, 0, 0,0);
    
    //% "Play"
    m_actionPlay = new MAction(qtTrId("meegovideo_play"), this);
    m_actionPlay->setLocation(MAction::ObjectMenuLocation);
    connect(m_actionPlay, SIGNAL(triggered()), this, SLOT(handleActionPlay()));
    
    //% "Favourite"
    m_actionFavourite = new MAction(qtTrId("meegovideo_favourite"), this);
    m_actionFavourite->setLocation(MAction::ObjectMenuLocation);
    connect(m_actionFavourite, SIGNAL(triggered()), this, SLOT(handleActionFavourite()));
    
    //% "Info"
    m_actionInfo = new MAction(qtTrId("meegovideo_info"), this);
    m_actionInfo->setLocation(MAction::ObjectMenuLocation);
    connect(m_actionInfo, SIGNAL(triggered()), this, SLOT(handleActionInfo()));

    //% "Delete"
    m_actionDelete = new MAction(qtTrId("meegovideo_delete"), this);
    m_actionDelete->setLocation(MAction::ObjectMenuLocation);
    connect(m_actionDelete, SIGNAL(triggered()), this, SLOT(handleActionDelete()));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    MPannableViewport *viewport = new MPannableViewport(this);
    MWidgetController *container = new MWidgetController;
    container->setViewType("background");
    container->setContentsMargins(0, 0, 0, 0);

    viewport->setWidget(container);
    layout->addItem(viewport);

    QGraphicsLinearLayout *listLayout = new QGraphicsLinearLayout(Qt::Vertical, container);
    listLayout->setSpacing(0);
    listLayout->setContentsMargins(0, 0, 0, 0);

    m_list = new MList(this);
    m_list->setColumns(2);
    m_list->setSelectionMode(MList::SingleSelection);

    m_itemCreator = new VideosListItemCreator(category, this);
    m_list->setCellCreator(m_itemCreator);
    m_listModel = new VideosListModel(this, this, category);

    m_listProxyModel = new VideosListProxyModel(this);
    m_listProxyModel->setSourceModel(m_listModel);
    m_list->setItemModel(m_listProxyModel);
    m_listProxyModel->sort(0, Qt::AscendingOrder);

    connect(m_list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClickedHandler(QModelIndex)));
    connect(m_list, SIGNAL(panningStopped()), this, SLOT(queueTasks()));
    connect(m_list, SIGNAL(panningStarted()), this, SLOT(dequeueTasks()));
    connect(m_list, SIGNAL(displayEntered()), this, SLOT(queueTasks()));
    connect(m_list, SIGNAL(displayExited()), this, SLOT(dequeueTasks()));

    QTimer::singleShot(0, this, SLOT(queueTasks()));
    listLayout->addItem(m_list);
}

VideosList::~VideosList()
{
    qDebug() << "VideosList::~VideosList()";
    delete m_list;
    delete m_itemCreator;
    delete m_listModel;
    delete m_listProxyModel;

    delete m_actionPlay;
    delete m_actionFavourite;
    delete m_actionInfo;
    delete m_actionDelete;
}

void VideosList::queueTasks()
{
    qDebug() << "VideosList::queueTasks()" << "category " << m_category;

    QModelIndex firstVisibleIndex = m_list->firstVisibleItem();
    QModelIndex lastVisibleIndex = m_list->lastVisibleItem();
    int firstVisibleRow = firstVisibleIndex.row();
    int lastVisibleRow = lastVisibleIndex.row();

    for (int i = firstVisibleRow; i <= lastVisibleRow; i++)
    {
        /* Add index mapped from proxy model */
        const QModelIndex &index = m_listProxyModel->index(i, 0);
        QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
        qDebug() << "Map from index " << index.row() << " to " << modelIndex.row();
        if (!modelIndex.isValid())
            continue;
        m_listModel->addTask(modelIndex);
    }

    m_listModel->startTaskQueue();
}

QString VideosList::firstVideoPath()
{
    QString ret = QString("");
    const QModelIndex &index = m_listProxyModel->index(0,0);
    QModelIndex modelIndex = m_listProxyModel->mapToSource(index);

    if(modelIndex.isValid()) {
        ret = m_listModel->filePath(modelIndex);
    }
    qDebug() << "firstVideoPath " << ret;
    return ret;
}

void VideosList::dequeueTasks()
{
    qDebug() << "VideosList::dequeueTasks()";

    m_listModel->stopTaskQueue();
}

void VideosList::itemClickedHandler(QModelIndex index)
{
    m_list->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    if (!index.isValid())
        return;

    QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
    if (!modelIndex.isValid())
        return;

    qDebug() << "VideosList::itemClickedHandler emitting videoItemClicked signal " << index.row() << "Source index is " << modelIndex.row() << m_listModel->filePath(modelIndex);
    emit(videoItemClicked(m_listModel->filePath(modelIndex)));
}

/* TODO: I18N L10N support */
void VideosList::scrollTo(const QString &letter)
{
    int itemNumbers = m_listModel->rowCount();
    int i = 0;

    for (i = 0; i < itemNumbers; i++) {
        QModelIndex index = m_listModel->index(i, 0);

        QString displayName = index.data(VideosListModel::TitleRole).toString();
        if (!(QString::compare(displayName.at(0), letter, Qt::CaseInsensitive) < 0))
            break;
    }

    if (i == itemNumbers)
        i--;

    QModelIndex index = m_listModel->index(i, 0);
    QModelIndex modelIndex = m_listProxyModel->mapFromSource(index);

    bool needToScroll = true;
    QModelIndex firstVisibleIndex = m_list->firstVisibleItem();
    QModelIndex lastVisibleIndex = m_list->lastVisibleItem();
    int firstVisibleRow = firstVisibleIndex.row();
    int lastVisibleRow = lastVisibleIndex.row();

    for (int i = firstVisibleRow; i <= lastVisibleRow; i++)
    {
        if (modelIndex.row() == i) {
            qDebug() << "No need to Scroll " << modelIndex.row();
            needToScroll = false;
            break;
        }
    }

    if (needToScroll) {
        qDebug() << "Scroll to " << index.data(VideosListModel::TitleRole).toString();
        m_list->scrollTo(modelIndex, MList::PositionAtTopHint);
    }
}

bool VideosList::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneContextMenu) {
        QModelIndex index = m_listItems.value(static_cast<MWidget *>(watched));
        if (index.isValid()) {
            QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
            if (modelIndex.isValid()) {
                m_actionWidget = static_cast<MWidget *>(watched);

                QString path = m_listModel->filePath(modelIndex);
                const bool isFavorite = Preferences::isFavorite(path);

                if (isFavorite)
                    m_actionFavourite->setIconID("icon-m-common-checkbox-checked");
                else
                    m_actionFavourite->setIconID("icon-m-common-checkbox-unchecked");
            }
        }
    }

    return MStylableWidget::eventFilter(watched, event);
}

void VideosList::updateRecentlyViewed()
{
    m_listModel->updateRecentlyViewed();
}

void VideosList::updateFavourites()
{
    m_listModel->updateFavourites();
}

void VideosList::handleActionFavourite()
{
    QModelIndex index = m_listItems.value(m_actionWidget);

    if (index.isValid()) {
        QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
        if (modelIndex.isValid()) {

            QString path = m_listModel->filePath(modelIndex);

            bool isFavorite = !Preferences::isFavorite(path);

            if (isFavorite)
                m_actionFavourite->setIconID("icon-m-common-checkbox-checked");
            else
                m_actionFavourite->setIconID("icon-m-common-checkbox-unchecked");

            Preferences::setFavorite(path, isFavorite);

            MWidget *widget = m_listItems.key(index);
            if (!widget)
                return;

            m_list->cellCreator()->updateCell(index, widget);

            m_listModel->updateFavourites();
        }
    }
}

void VideosList::handleActionDelete()
{
    qDebug() << "VideosList::handleActionDelete ";

    QModelIndex index = m_listItems.value(m_actionWidget);

    if (index.isValid()) {
        QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
        if (modelIndex.isValid()) {
            MMessageBox errorMessage(tr("Delete"), tr("Do you want to permanently delete %1?").arg(m_listModel->fileName(modelIndex)), M::YesButton | M::NoButton);
            int ret = errorMessage.exec();
            if (ret == M::YesButton) {
                m_listModel->remove(modelIndex);
            }
        }
    }
}

void VideosList::handleActionInfo()
{
    qDebug() << "VideosList::handleActionInfo " ;

    QModelIndex index = m_listItems.value(m_actionWidget);

    if (index.isValid()) {
        QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
        if (modelIndex.isValid()) {
            emit(videoInfoClicked(modelIndex));
        }
    }
}

void VideosList::handleActionPlay()
{
    qDebug() << "VideosList::handleActionPlay ";

    QModelIndex index = m_listItems.value(m_actionWidget);

    if (index.isValid()) {
        QModelIndex modelIndex = m_listProxyModel->mapToSource(index);
        if (modelIndex.isValid()) {
            emit(videoItemClicked(m_listModel->filePath(modelIndex)));
        }
    }
}

void VideosList::handleItemCreated(const QModelIndex &index, MWidget *widget)
{
    qDebug() << "VideosList::handleItemCreated";

    if (!widget)
        return;

    widget->installEventFilter(this);

    m_listItems[widget] = index;

    QList<QAction *> actions = widget->actions();

    if (actions.indexOf(m_actionPlay) == -1)
        widget->addAction(m_actionPlay);
    if (actions.indexOf(m_actionFavourite) == -1)
        widget->addAction(m_actionFavourite);
    if (actions.indexOf(m_actionInfo) == -1)
        widget->addAction(m_actionInfo);
    if (actions.indexOf(m_actionDelete) == -1)
        widget->addAction(m_actionDelete);
}
