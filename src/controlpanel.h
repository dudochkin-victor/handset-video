/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <MStylableWidget>
#include <MWidgetStyle>
#include <MButton>
#include <MLabel>

#include "videoviewpage.h"

class ControlPanel : public MStylableWidget
{
    Q_OBJECT;

public:
    ControlPanel(QGraphicsItem *parent = 0);
    ~ControlPanel();

public slots:
    void control_button_clicked();
    void control_button_update(QMediaPlayer::State state);

signals:
    void btn_clicked(const QString &button);
    
private:
    M_STYLABLE_WIDGET(MWidgetStyle);

    MButton *m_playButton;
    MButton *m_nextButton;
    MButton *m_previousButton;

    //Use MLabel first
    MLabel *m_info;
};

#endif
