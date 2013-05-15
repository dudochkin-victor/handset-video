/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QGraphicsSceneResizeEvent>
#include <QGraphicsVideoItem>
#include <QMediaPlaylist>
#include <QDebug>
#include <QUrl>

#include <MApplication>

#include "videoplayer.h"
#include "preferences.h"

qreal VideoPlayer::Factors[] = {-8.0, -4.0, -2.0, 1.0, 2.0, 4.0, 8.0};

VideoPlayer::VideoPlayer(QGraphicsItem *parent)
    : MWidget(parent)
{
    m_videoItem = new QGraphicsVideoItem(this);
    m_videoItem->setAspectRatioMode(Qt::KeepAspectRatio);

    m_mediaPlayer = new QMediaPlayer(this);
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(handleDurationChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SIGNAL(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(mediaChanged(const QMediaContent &)), SIGNAL(mediaChanged(const QMediaContent &)));
    connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(handlePlayerStateChanged(QMediaPlayer::State)));
    connect(m_mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), SLOT(handleError(QMediaPlayer::Error)));
    connect(m_mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    
    m_playlist = new QMediaPlaylist(this);
#if (QTM_VERSION >= QTM_VERSION_CHECK(1, 0, 1))
    m_mediaPlayer->setPlaylist(m_playlist);
    m_mediaPlayer->setVideoOutput(m_videoItem);
#else
    m_videoItem->setMediaObject(m_mediaPlayer);
#endif
    
}


VideoPlayer::~VideoPlayer()
{
#if (QTM_VERSION >= QTM_VERSION_CHECK(1, 0, 1))
    m_mediaPlayer->stop();
    m_mediaPlayer->setVideoOutput(static_cast<QGraphicsVideoItem *>(0));
#else
    m_playlist->setMediaObject(0);
#endif
}

QSizeF VideoPlayer::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);

    if (which == Qt::PreferredSize) {
        MWindow* w = MApplication::activeWindow();
        return w->visibleSceneSize();
    }

    return QSizeF(-1, -1);
}

qint64 VideoPlayer::duration() const
{
    return m_mediaPlayer->duration();
}

bool VideoPlayer::setMediaContent(const QUrl &url, const QString &description)
{
    Q_UNUSED(description);

    m_errorOccurred = false;
    m_factorIndex = ZeroFactorIndex;

    if (url == m_currentmediaURL)
    	return true;

#if (QTM_VERSION >= QTM_VERSION_CHECK(1, 0, 1))
    m_playlist->clear();
    m_playlist->addMedia(QMediaContent(url));
    m_playlist->setCurrentIndex(0);
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    m_mediaPlayer->setVideoOutput(m_videoItem); // reset video output
#else
    if (m_playlist->mediaObject() == m_mediaPlayer)
        m_playlist->setMediaObject(0);

    m_mediaPlayer->setMedia(QMediaContent(url));
#endif
    m_currentmediaURL = url;

    if (m_errorOccurred)
        return false;

    return true;
}

void VideoPlayer::resizeEvent(QGraphicsSceneResizeEvent * event)
{
    MWidget::resizeEvent(event);

    m_videoItem->setSize(event->newSize());
}

void VideoPlayer::setErrorNotificationsEnabled(bool enabled)
{
    m_errorNotificationsEnabled = enabled;
}

bool VideoPlayer::isErrorNotificationsEnabled() const
{
    return m_errorNotificationsEnabled;
}

int VideoPlayer::state()
{
    return m_mediaPlayer->state();
}

void VideoPlayer::handleDurationChanged(qint64 duration)
{
    emit durationChanged(duration);

    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState && m_startFrame != -1) {
        m_mediaPlayer->setPosition(m_startFrame);
        m_startFrame = -1;
    }
}

void VideoPlayer::handlePlayerStateChanged(QMediaPlayer::State state)
{
	qDebug() << "meegovideo2: in handlePlayerStateChanged: " << state;
    VideoPlayer::PlaybackState playerState;
    switch (state) {
    case QMediaPlayer::PlayingState:
        if (m_factorIndex > ZeroFactorIndex)
            playerState = VideoPlayer::fforwardState;
        else if (m_factorIndex < ZeroFactorIndex)
            playerState = VideoPlayer::fbackwardState;
        else
        	playerState = VideoPlayer::PlayingState;
        break;
    case QMediaPlayer::PausedState:
        playerState = VideoPlayer::PausedState;
    	break;
    case QMediaPlayer::StoppedState:
    default:
        m_factorIndex = ZeroFactorIndex;
        playerState = VideoPlayer::StoppedState;
        break;
    }
    qDebug() << "meegovideo2: in handlePlayerStateChanged: " << playerState;
    emit playerStateChanged(playerState);  
}


void VideoPlayer::seek(int seconds)
{
    m_mediaPlayer->setPosition(seconds * 1000);
}

void VideoPlayer::closePlayer()
{
    qint64 pos = -1;

    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState ||
        m_mediaPlayer->state() == QMediaPlayer::PausedState) {
        m_mediaPlayer->pause(); // we pause instead of stop to do not reset the current postion (even if we are in pause)
        if (pos != m_mediaPlayer->duration()) // save only if it's not the end of the file. otherwise write -1
            pos = m_mediaPlayer->position();
    }

    Preferences::setLastPlaybackPosition(m_mediaPlayer->media().canonicalUrl().toLocalFile(), pos); // qlonglong and qint64 are fine

    if (m_mediaPlayer->error() == QMediaPlayer::NoError)
        m_mediaPlayer->pause(); // due to a bug in QtMobility stop player explicitly

}

void VideoPlayer::handleError(QMediaPlayer::Error error)
{
    if (Preferences::launchMode() != Preferences::NormalMode &&
        m_playlist->mediaObject() == m_mediaPlayer) {
        // ignore error
        m_playlist->next();
        return ;
    }

    m_errorOccurred = true;

    if (m_errorNotificationsEnabled)
        emit errorOccurred(error, m_mediaPlayer->errorString());
}

void VideoPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        //
    }
}


void VideoPlayer::play(qint64 startFrame)
{ 
    qDebug() << "meegovideo2: calling VideoPlayer::play(): m_mediaPlayer state" << m_mediaPlayer->state() <<"start frame:"<<startFrame;
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState) {
        //if (m_mediaPlayer->playbackRate() != Factors[ZeroFactorIndex]) {
    		// Set the playback rate to normal speed
    	    m_factorIndex = ZeroFactorIndex;
    	    m_mediaPlayer->setPlaybackRate(Factors[m_factorIndex]);
    	    emit playerStateChanged(VideoPlayer::PlayingState);
        //}
        //else
        //    m_mediaPlayer->pause();
    } 
    else if (m_mediaPlayer->state() == QMediaPlayer::StoppedState) {
        m_startFrame = startFrame;
        m_factorIndex = ZeroFactorIndex;
    	m_mediaPlayer->setPlaybackRate(Factors[m_factorIndex]);
        m_mediaPlayer->play();
    }
    else if (m_mediaPlayer->state() == QMediaPlayer::PausedState) {
        m_startFrame = startFrame;
        // To reset the speed to normal
    	m_factorIndex = ZeroFactorIndex;
    	m_mediaPlayer->setPlaybackRate(Factors[m_factorIndex]);
        m_mediaPlayer->play();
    }
}

void VideoPlayer::playInLoop(const QList<QMediaContent> & items)
{
#if (QTM_VERSION <= QTM_VERSION_CHECK(1, 0, 0))
    if (m_playlist->mediaObject() != m_mediaPlayer)
        m_playlist->setMediaObject(m_mediaPlayer);
#endif
    
    m_playlist->clear();
    m_playlist->addMedia(items);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
   
#if (QTM_VERSION >= QTM_VERSION_CHECK(1, 0, 1))
    m_mediaPlayer->setVideoOutput(m_videoItem); // reset video output
#endif
    m_mediaPlayer->play();
}

void VideoPlayer::pause()
{
    qDebug() << "meegovideo2: calling VideoPlayer::pause()";
    m_mediaPlayer->pause();
}

void VideoPlayer::stop()
{
    qDebug() << "meegovideo2: calling VideoPlayer::stop()";
    m_mediaPlayer->stop();
}

void VideoPlayer::windForward()
{
    // since invalid index may cause a crash, lets double check that
    if (m_factorIndex < MinFactorIndex || m_factorIndex > MaxFactorIndex) {
        qCritical("Player::windForward: Invalid factor array index");
        Q_ASSERT(false);
        return ;
    }

    if (m_factorIndex == MaxFactorIndex)
        return ;

    m_mediaPlayer->setPlaybackRate(Factors[++m_factorIndex]);
    if (m_factorIndex == ZeroFactorIndex)
        emit playerStateChanged(VideoPlayer::PlayingState);
    else
        emit playerStateChanged(VideoPlayer::fforwardState);
}

void VideoPlayer::windBackward()
{
    // since invalid index may cause a crash, lets double check that
    if (m_factorIndex < MinFactorIndex || m_factorIndex > MaxFactorIndex) {
        qCritical("Player::windForward: Invalid factor array index");
        Q_ASSERT(false);
        return ;
    }

    if (m_factorIndex == MinFactorIndex)
        return ;

    m_mediaPlayer->setPlaybackRate(Factors[--m_factorIndex]);
    
    if (m_factorIndex == ZeroFactorIndex)
        emit playerStateChanged(VideoPlayer::PlayingState);
    else
        emit playerStateChanged(VideoPlayer::fbackwardState);
}


QUrl VideoPlayer::mediaUrl()
{
   
   return m_currentmediaURL;
}


