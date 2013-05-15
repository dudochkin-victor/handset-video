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

#include <MButton>
#include <MStylableWidget>

#include "controlpanel.h"

ControlPanel::ControlPanel(QGraphicsItem *parent) : MStylableWidget(parent)
{
    qDebug() << "ControlPanel::ControlPanel";

    setObjectName("ControlPanelWidget");
    setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *controlPanelLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    controlPanelLayout->setSpacing(0);
    controlPanelLayout->setContentsMargins(0, 0, 0, 0);

    m_playButton = new MButton(this);
    m_playButton->setIconID("play");
    m_playButton->setObjectName("ControlPanelPlayButton");
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));

    m_nextButton = new MButton(this);
    m_nextButton->setIconID("forward_disabled");
    m_nextButton->setObjectName("ControlPanelNextButton");
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));

    m_previousButton = new MButton(this);
    m_previousButton->setIconID("back_disabled");
    m_previousButton->setObjectName("ControlPanelPreviousButton");
    connect(m_previousButton, SIGNAL(clicked()), this, SLOT(control_button_clicked()));


    //Use MLabel first
    m_info = new MLabel(this);
    m_info->setText(qtTrId("Start playing movies"));
    m_info->setObjectName("ControlPanelInfoLabel");

    controlPanelLayout->addItem(m_previousButton);
    controlPanelLayout->addItem(m_playButton);
    controlPanelLayout->addItem(m_nextButton);
    controlPanelLayout->addItem(m_info);
    //controlPanelLayout->addStretch();
}

ControlPanel::~ControlPanel()
{
    delete m_playButton;
    delete m_nextButton;
    delete m_previousButton;
    delete m_info;
}

void ControlPanel::control_button_clicked()
{
    MButton *btn = static_cast<MButton *>(sender());
    qDebug() << "meegovideo: ControlPanel btn clicked " << btn->iconID();
    emit btn_clicked(btn->iconID());
}

void ControlPanel::control_button_update(QMediaPlayer::State state) {

	switch (state) {
    case QMediaPlayer::PlayingState:
    	this->m_playButton->setIconID("pause");
        this->m_nextButton->setIconID("forward_enabled");
        this->m_previousButton->setIconID("back_enabled");
        break;
    case QMediaPlayer::PausedState:
    	this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_disabled");
        this->m_previousButton->setIconID("back_disabled");
    	break;
    case QMediaPlayer::StoppedState:
        this->m_playButton->setIconID("play");
        this->m_nextButton->setIconID("forward_disabled");
        this->m_previousButton->setIconID("back_disabled");
        break;
    default:
        break;
    }
}
