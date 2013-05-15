/***************************************************************************
 *
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 *
****************************************************************************/

#ifndef VIDEOSEEKBAT_H
#define VIDEOSEEKBAT_H

#include <MStylableWidget>
#include <MWidgetStyle>
#include <MSeekBar>


class VideoSeekbar : public MStylableWidget
{
    Q_OBJECT
public:
    VideoSeekbar(QGraphicsItem *parent = 0);
    ~VideoSeekbar();
	qint64 value();
	void setDuration(qint64 duration);
	void setValue(qint64 value);

private slots:
    void sliderPressedHandle();
    void sliderReleasedHandle();

signals:
    void valueChanged(qint64);
    void sliderPressed();
    void sliderReleased();
    void outOfLoadedContentRange();
    void mousePress();
    void hoverleave();
    
private:
    M_STYLABLE_WIDGET(MWidgetStyle);
    bool seekBarIsPressed;
    MSeekBar *m_videoSeekBar;
    qint64 m_duration;

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VIDEOSEEKBAT_H
