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

#include "videoviewpage.h"
#include "controlpanel.h"
#include "videoplayer.h"
#include "thumbnailmanager.h"

VideoViewPage::VideoViewPage(QGraphicsItem *parent)
    : MApplicationPage(parent),
    m_startFrame(0),
    m_playerPreSate(VideoPlayer::StoppedState)
{
}

void VideoViewPage::createContent()
{
    MApplicationPage::createContent();

    setAutoMarginsForComponentsEnabled(false);
    //setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::AutoHide);

    QGraphicsWidget *panel = centralWidget();

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(0);
    
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(hideSeekBar())); //fixme
    
    connect(this, SIGNAL(backButtonClicked ()), this, SLOT(backToPreviousPageHandle()));
    
    
    m_videoPlayer = new VideoPlayer(panel);
    connect(m_videoPlayer, SIGNAL(playerStateChanged(VideoPlayer::PlaybackState)), SLOT(playerStateChanged(VideoPlayer::PlaybackState)));
    connect(m_videoPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_videoPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_videoPlayer, SIGNAL(mediaChanged(Const QMediaContent &)), SLOT(mediaChanged(Const QMediaContent &)));
    
    layout->addItem(m_videoPlayer);

    // Create video control Panel ... 
    m_videoControlPanelOverlay = new MOverlay;
    m_videoControlPanelOverlay->setViewType("background");
    m_videoControlPanelOverlay->setObjectName("VideoControlPanelOverLay");
    m_videoControlPanelOverlay->setContentsMargins(0, 0, 0, 0);
	
	MWidget *VideoControlPanelWidget = m_videoControlPanelOverlay->widget();
	QGraphicsLinearLayout *VideoControlPanelLayout = new QGraphicsLinearLayout(Qt::Horizontal, VideoControlPanelWidget);
    VideoControlPanelLayout->setSpacing(0);
    VideoControlPanelLayout->setContentsMargins(0, 0, 0, 0);
     
    m_videoControlPanel = new VideoControlPanel(NULL);
    connect(m_videoControlPanel, SIGNAL(btn_clicked(const QString &)), this, SLOT(control_btn_press_handler(const QString &)));
    connect(this, SIGNAL(StateChanged(VideoPlayer::PlaybackState)), m_videoControlPanel, SLOT(control_button_update(VideoPlayer::PlaybackState)));
    
    connect(m_videoControlPanel, SIGNAL(valueChanged(qint64)), this, SLOT(seekingHandle(qint64)));
    connect(m_videoControlPanel, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(m_videoControlPanel, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(m_videoControlPanel, SIGNAL(mousePress()), this, SLOT(mousePressHandle()));
    
    VideoControlPanelLayout->addItem(m_videoControlPanel);
    m_videoControlPanelOverlay->appear();
    
    // Create seekbar Panel ... 
    m_videoSeekbarOverlay = new MOverlay;
    m_videoSeekbarOverlay->setViewType("background");
    m_videoSeekbarOverlay->setObjectName("VideoSeekbarOverLay");
    m_videoSeekbarOverlay->setContentsMargins(0, 0, 0, 0);
    m_videoseekbar = new VideoSeekbar();
    MWidget *VideoSeekbarWidget = m_videoSeekbarOverlay->widget();
    QGraphicsLinearLayout *VideoSeekBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, VideoSeekbarWidget);
    VideoSeekBarLayout->addItem(m_videoseekbar);
    m_videoSeekbarOverlay->appear();
    
    connect(m_videoseekbar, SIGNAL(valueChanged(qint64)), this, SLOT(seekingHandle(qint64)));
    connect(m_videoseekbar, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(m_videoseekbar, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(m_videoseekbar, SIGNAL(mousePress()), this, SLOT(mousePressHandle()));
   
    connect(this, SIGNAL(mousePress()), this, SLOT(mousePressHandle()));
    
    panel->setLayout(layout);
    m_videoSeekbarOverlay->disappear();
    m_videoControlPanelOverlay->disappear();
}


void VideoViewPage::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit mousePress();
    MWidgetController::mousePressEvent(event);
}


void VideoViewPage::playerStateChanged(VideoPlayer::PlaybackState state)
{
	// extend status??
    emit StateChanged(state);
	
    switch (state) {
    case VideoPlayer::PlayingState:
        if (m_playerPreSate == VideoPlayer::StoppedState)
        	if (m_videoseekbar)
		        m_videoseekbar->setValue(0);
        m_playerPreSate = VideoPlayer::PlayingState;
        break;
    case VideoPlayer::PausedState:
    	m_playerPreSate = VideoPlayer::PausedState;
    	break;
    case VideoPlayer::StoppedState:
    	m_playerPreSate = VideoPlayer::StoppedState;
        break;
    default:
        break;
    }
    
    
    
    
}

void VideoViewPage::positionChanged(qint64 progress)
{
    if (m_videoControlPanel)
        m_videoControlPanel->setValue(progress);
    if (m_videoseekbar)
        m_videoseekbar->setValue(progress);
}

void VideoViewPage::durationChanged(qint64 duration)
{
    if (m_videoControlPanel)
        m_videoControlPanel->setDuration(duration);
    if (m_videoControlPanel) {
        m_videoControlPanel->setMediaName(ThumbnailManager::instance()->title( m_videoPlayer->mediaUrl().path()));
        m_videoControlPanel->setMediaDate(ThumbnailManager::instance()->date( m_videoPlayer->mediaUrl().path()).toString("MMM d yyyy"));
    } 
    
    if (m_videoseekbar)
        m_videoseekbar->setDuration(duration);
}

void VideoViewPage::mediaChanged( const QMediaContent & media ) {
   
}


void VideoViewPage::control_btn_press_handler(const QString &press_btn)
{   
    qDebug() << "meegovideo: state:" << this->player()->state();
    if (!QString::compare(press_btn, "play")) {
        if (this) {
            this->player()->play();
        }
    }
    if (!QString::compare(press_btn, "pause")) {
        if (this) {
            this->player()->pause();
        }
    }
    if (!QString::compare(press_btn, "forward_enabled")) {
        if (this) {
            if (this->player()->state() == QMediaPlayer::PlayingState)
                this->player()->windForward();
        }
    }
    if (!QString::compare(press_btn, "back_enabled"))
        if (this) {
            if (this->player()->state() == QMediaPlayer::PlayingState)
                this->player()->windBackward();
    }
}


void VideoViewPage::seekingHandle(qint64 position) {
    qDebug() << "VideoViewPage: seekingHandle" << position;  
}


void VideoViewPage::sliderPressed() {
    qDebug() << "meegovideo: sliderPressed  " << m_videoControlPanel->value();
}


void VideoViewPage::sliderReleased() {
    qDebug() << "meegovideo: sliderReleased" << m_videoseekbar->value();
    int position;
    //position = m_videoControlPanel->value() / 1000;
    position = m_videoseekbar->value() / 1000;
    if (m_videoseekbar->value() % 1000)
        position++;
    m_videoPlayer->seek(position);
}


void VideoViewPage::backToPreviousPageHandle() {
    qDebug() << "meegovideo: backToPreviousPageHandle" ;
    m_videoSeekbarOverlay->disappear();
    m_videoControlPanelOverlay->disappear();
    m_videoPlayer->closePlayer();
}


void VideoViewPage::mousePressHandle() {
    qDebug() << "meegovideo: mousePressHandle" ;
    m_videoSeekbarOverlay->appear();
    m_videoControlPanelOverlay->appear();
    //m_timer->stop();
    m_timer->start(4000);  
}

void VideoViewPage::hideSeekBar() {
    m_videoSeekbarOverlay->disappear();
    m_videoControlPanelOverlay->disappear();
}

