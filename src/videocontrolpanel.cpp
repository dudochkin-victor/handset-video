/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "videocontrolpanel.h"

VideoControlPanel::VideoControlPanel(QGraphicsItem *parent) : MStylableWidget(parent)
{
    qDebug() << "VideoControlPanel::VideoControlPanel";

    setObjectName("VideoControlPanelWidget");
    setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *controlPanelLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    controlPanelLayout->setSpacing(0);
    controlPanelLayout->setContentsMargins(0, 0, 0, 0);
    
    // Control btns
    m_playButton = new MButton(this);
    m_playButton->setIconID("play");
    m_playButton->setObjectName("ControlPanelPlayButton");
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));
    m_nextButton = new MButton(this);
    m_nextButton->setIconID("forward_disableed");
    m_nextButton->setObjectName("ControlPanelNextButton");
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));
    m_previousButton = new MButton(this);
    m_previousButton->setIconID("back_enabled");
    m_previousButton->setObjectName("ControlPanelPreviousButton");
    connect(m_previousButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));

    QGraphicsLinearLayout *controlPanelLayout2 = new QGraphicsLinearLayout(Qt::Vertical);
    controlPanelLayout2->setSpacing(0);
    controlPanelLayout2->setContentsMargins(0, 0, 0, 0);
    // Horizontal Layout for duration label and ticker
    QGraphicsLinearLayout *controlPanelLayout3 = new QGraphicsLinearLayout(Qt::Horizontal);
    controlPanelLayout3->setSpacing(0);
    controlPanelLayout3->setContentsMargins(0, 0, 0, 0);
    
    //Duration label
    //m_durationlabel = new MLabel(this);
    //m_durationlabel->setObjectName("DurationLabel");
    
    // Video Date label
    m_videodatelabel = new MLabel(this);
    m_videodatelabel->setObjectName("VideoDateLabel");
    
    //Seekbar
    m_duration = 0;
    seekBarIsPressed = false;
    m_videoSeekBar = new MSeekBar;
    m_videoSeekBar->setObjectName("VideoSeekBar");
    connect(m_videoSeekBar, SIGNAL(valueChanged(qint64)), SIGNAL(valueChanged(qint64)));
    connect(m_videoSeekBar, SIGNAL(sliderPressed()), this, SLOT(sliderPressedHandle()));
    connect(m_videoSeekBar, SIGNAL(sliderReleased()), this, SLOT(sliderReleasedHandle()));
    //m_videoSeekBar->setMinLabelVisible(true);
    //m_videoSeekBar->setMaxLabelVisible(true);
    m_videoSeekBar->setHandleLabelVisible(true);
    m_duration = 0;
    seekBarIsPressed = false;
    
    //Ticker
    m_ticker = new Ticker(this);
    m_ticker->setObjectName("TickerText");
        
    controlPanelLayout3->addItem(m_ticker);
    controlPanelLayout3->addItem(m_videodatelabel);
    //controlPanelLayout3->addItem(m_durationlabel);
    
    controlPanelLayout2->addItem(controlPanelLayout3);
    //controlPanelLayout2->addItem(m_videoSeekBar);
   
    controlPanelLayout->addItem(m_previousButton);
    controlPanelLayout->addItem(m_playButton);
    controlPanelLayout->addItem(m_nextButton);
    controlPanelLayout->addItem(controlPanelLayout2);
    //controlPanelLayout->addStretch();
}


VideoControlPanel::~VideoControlPanel()
{
    delete m_playButton;
    delete m_nextButton;
    delete m_previousButton;
    delete m_videodatelabel;
    //delete m_durationlabel;
    delete m_videoSeekBar;
    delete m_ticker;
}


void VideoControlPanel::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit mousePress();
    MWidgetController::mousePressEvent(event);
}


void VideoControlPanel::control_button_clicked()
{
    MButton *btn = static_cast<MButton *>(sender());
    //qDebug() << "meegovideo: ControlPanel btn clicked " << btn->iconID();
    emit btn_clicked(btn->iconID());
}


void VideoControlPanel::control_button_update(VideoPlayer::PlaybackState state) {
	switch (state) {
    case VideoPlayer::PlayingState:
    	this->m_playButton->setIconID("pause");
        this->m_nextButton->setIconID("forward_enabled");
        this->m_previousButton->setIconID("back_enabled");
        break;
    case VideoPlayer::PausedState:
    	this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_disabled");
        this->m_previousButton->setIconID("back_disabled");
    	break;
    case VideoPlayer::StoppedState:
        this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_disabled");
        this->m_previousButton->setIconID("back_disabled");
        break;
    case VideoPlayer::fforwardState:
    	this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_enabled");
        this->m_previousButton->setIconID("back_enabled");
        break;
    case VideoPlayer::fbackwardState:
    	this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_enabled");
        this->m_previousButton->setIconID("back_enabled");
        break;
    default:
        break;
    }
}


qint64 VideoControlPanel::value() {
	return m_videoSeekBar->value();
}


void VideoControlPanel::setMediaName(const QString &MediaName) {
     m_ticker->setText2(MediaName);
}


void VideoControlPanel::setMediaDate(const QString &MediaDate) {
    m_videodatelabel->setText(MediaDate);
}


void VideoControlPanel::setDuration(qint64 duration) {
	m_duration = duration;
	m_videoSeekBar->setMinimum(0);
    m_videoSeekBar->setMaximum(duration);
    m_videoSeekBar->setMinLabel("0:00");
    int minutes = (duration / 1000) / 60;
    int seconds = (duration / 1000) % 60;
    //m_videoSeekBar->setMaxLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
    //m_durationlabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}


void VideoControlPanel::setValue(qint64 value) {
	if (seekBarIsPressed == false)
		m_videoSeekBar->setValue(value);
	
	if (value % 1000)
		value += 1000;
	if (value > m_duration)
        value = m_duration;
	int minutes = (value / 1000) / 60;
    int seconds = (value / 1000) % 60;
	//m_videoSeekBar->setMinLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
	 //m_durationlabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}


void VideoControlPanel::sliderPressedHandle() {
    seekBarIsPressed = true;
    emit sliderPressed();
}
 
void VideoControlPanel::sliderReleasedHandle() {
    seekBarIsPressed = false;
    emit sliderReleased();
}











