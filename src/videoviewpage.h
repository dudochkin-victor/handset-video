/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOVIEWPAGE_H
#define VIDEOVIEWPAGE_H

#include <QTimer>
#include <QMediaPlayer>
#include <MOverlay>
#include <MApplicationPage>
#include <MWidgetController>
#include <QUrl>

#include "videoplayer.h"
#include "videoseekbar.h"
#include "videocontrolpanel.h"

#if QT_VERSION < 0x040700
QTM_BEGIN_NAMESPACE
#endif
class QMediaPlayer;
class QMediaPlaylist;
#if QT_VERSION < 0x040700
QTM_END_NAMESPACE
QTM_USE_NAMESPACE
#endif

class VideoPlayer;
class ControlPanel;

class VideoViewPage : public MApplicationPage
{
    Q_OBJECT
public:
    VideoViewPage(QGraphicsItem *parent = 0);

    virtual void createContent();

    inline VideoPlayer *player() const {return m_videoPlayer;}

Q_SIGNALS:
    void StateChanged(VideoPlayer::PlaybackState state);
    void mousePress();
    //void hoverleave();
	
private slots:
    void playerStateChanged(VideoPlayer::PlaybackState state);
    void positionChanged(qint64 progress);
    void durationChanged(qint64 duration);
    void seekingHandle(qint64 position);
    void sliderPressed();
    void sliderReleased();
    void backToPreviousPageHandle();
    void mousePressHandle();
    void hideSeekBar();
    void control_btn_press_handler(const QString &button);
    void mediaChanged(const QMediaContent & media);

private:
    VideoPlayer *m_videoPlayer;
    ControlPanel *m_controlPanel;
    VideoControlPanel *m_videoControlPanel;
    MOverlay *m_videoControlPanelOverlay;
    MOverlay *m_videoSeekbarOverlay;
    VideoSeekbar *m_videoseekbar;
    VideoPlayer::PlaybackState m_playerPreSate;
    qint64 m_startFrame;
    QTimer *m_timer;
    
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VIDEOVIEWPAGE_H
