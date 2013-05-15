/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef ALPHABETBAR_H
#define ALPHABETBAR_H

#include <MStylableWidget>
#include <MWidgetStyle>
#include <MList>

class AlphabetBar : public MStylableWidget
{
    Q_OBJECT;

public:
    AlphabetBar(MList *list = 0, QGraphicsItem *parent = 0);
    ~AlphabetBar();

signals:
    void clicked(const QString &button);

private slots:
    void button_clicked();

private:
    M_STYLABLE_WIDGET(MWidgetStyle);
    MList *m_scrollList;
};

#endif
