/***************************************************************************
 *
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */
 
 
#include "videoseekbar.h"

#include <QTimer>
#include <QDebug>
#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MSeekBar>
#include <MLayout>
#include <MLocale>

VideoSeekbar::VideoSeekbar(QGraphicsItem *parent) : MStylableWidget(parent)
{
    qDebug() << "VideoSeekbar::VideoSeekbar";
    setObjectName("VideoSeekbarWidget");
    setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *videoseekbaLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    videoseekbaLayout->setSpacing(0);
    videoseekbaLayout->setContentsMargins(0, 0, 0, 0);
    
    m_videoSeekBar = new MSeekBar;
    m_videoSeekBar->setObjectName("VideoSeekBar");
    connect(m_videoSeekBar, SIGNAL(valueChanged(qint64)), SIGNAL(valueChanged(qint64)));
    connect(m_videoSeekBar, SIGNAL(sliderPressed()), this, SLOT(sliderPressedHandle()));
    connect(m_videoSeekBar, SIGNAL(sliderReleased()), this, SLOT(sliderReleasedHandle()));
    //connect(m_videoSeekBar, SIGNAL(outOfLoadedContentRange()), SIGNAL(outOfLoadedContentRange()));
    
    m_videoSeekBar->setMinLabelVisible(true);
    m_videoSeekBar->setMaxLabelVisible(true);
    m_videoSeekBar->setHandleLabelVisible(true); //?????????
    videoseekbaLayout->addItem(m_videoSeekBar);
    
    videoseekbaLayout->setAlignment(m_videoSeekBar, Qt::AlignHCenter);
    
    this->setAcceptHoverEvents(true);
    //qDebug() << "meegovideo acceptHoverEvents: " << this->acceptHoverEvents();
    //MWidgetController::setAcceptHoverEvents(true);
    //qDebug() << "meegovideo acceptHoverEvents: " << this->acceptHoverEvents();
    
    m_duration = 0;
    seekBarIsPressed = false;
}


qint64 VideoSeekbar::value() {
	return m_videoSeekBar->value();
}

void VideoSeekbar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit mousePress();
    MWidgetController::mousePressEvent(event);
}


void VideoSeekbar::hoverLeaveEvent(QGraphicsSceneMouseEvent *event) {
    emit hoverleave();
    //MWidgetController::hoverLeaveEvent(event);
}


void VideoSeekbar::setDuration(qint64 duration) {
    m_duration = duration;
    m_videoSeekBar->setMinimum(0);
    m_videoSeekBar->setMaximum(duration);
    m_videoSeekBar->setMinLabel("0:00");
    int hours   = (duration / 1000) / 3600;
    int minutes = ((duration / 1000) % 3600) / 60;
    int seconds = ((duration / 1000) % 3600) % 60;
    if (hours == 0)
        m_videoSeekBar->setMaxLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
    else
        m_videoSeekBar->setMaxLabel(QString("%1:%2:%3").arg(hours).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}

void VideoSeekbar::setValue(qint64 value) {
    if (seekBarIsPressed == false)
        m_videoSeekBar->setValue(value);
    if (value % 1000)
        value += 1000;
    if (value > m_duration)
        value = m_duration;
    int hours   = (value / 1000) / 3600;     
    int minutes = ((value / 1000) % 3600) / 60;
    int seconds = ((value / 1000) % 3600) % 60;
    if (hours == 0)
        m_videoSeekBar->setMinLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
    else
        m_videoSeekBar->setMinLabel(QString("%1:%2:%3").arg(hours).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}



void VideoSeekbar::sliderPressedHandle() {
    seekBarIsPressed = true;
    emit sliderPressed();
}
 
void VideoSeekbar::sliderReleasedHandle() {
    seekBarIsPressed = false;
    emit sliderReleased();
}



VideoSeekbar::~VideoSeekbar()
{
    delete m_videoSeekBar;
}
 
 
 
 
 
 
 
 
