/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOCONTROLPANEL_H
#define VIDEOCONTROLPANEL_H

#include <QMediaPlayer>
#include <MStylableWidget>
#include <MWidgetStyle>
#include <MButton>
#include <MLabel>
#include <QDebug>
#include <QGraphicsLinearLayout>
#include <MButton>
#include <MStylableWidget>
#include <MSeekBar>

#include "videoplayer.h"
#include "ticker.h"

class Ticker;

class VideoControlPanel : public MStylableWidget
{
    Q_OBJECT;

public:
    VideoControlPanel(QGraphicsItem *parent = 0);
    ~VideoControlPanel();
    qint64 value();
    void setMediaName(const QString &MediaName);
    void setMediaDate(const QString &MediaDate);
    void setDuration(qint64 duration);
	void setValue(qint64 value);
    
public slots:
    void control_button_clicked();
    void control_button_update(VideoPlayer::PlaybackState state);
    void sliderPressedHandle();
    void sliderReleasedHandle();

signals:
    // Playback btns
    void btn_clicked(const QString &button);
    
    // Seekbar
    void valueChanged(qint64);
    void sliderPressed();
    void sliderReleased();
    void outOfLoadedContentRange();
    void mousePress();
    
private:
    M_STYLABLE_WIDGET(MWidgetStyle);
    MButton *m_playButton;
    MButton *m_nextButton;
    MButton *m_nextButton2;
    MButton *m_previousButton;
    bool seekBarIsPressed;
    MSeekBar *m_videoSeekBar;
    qint64 m_duration;
    MLabel *m_durationlabel;
    MLabel *m_playingtimelabel;
    MLabel *m_videodatelabel;
    Ticker *m_ticker;    

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


#endif
