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
#include <MImageWidget>
#include <MStylableWidget>

#include "alphabetbar.h"

AlphabetBar::AlphabetBar(MList *list, QGraphicsItem *parent) : MStylableWidget(parent)
{
    qDebug() << "AlphabetBar::AlphabetBar";

    setObjectName("AlphabetBar");
    setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *alphabetLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    alphabetLayout->setSpacing(0);
    alphabetLayout->setContentsMargins(0, 0, 0, 0);

    MButton *b = new MButton("#");
    b->setObjectName("AlphabetItem");
    connect(b, SIGNAL(clicked()), this, SLOT(button_clicked()));
    alphabetLayout->addItem(b);

    for (char i='A'; i<='Z'; i++)
    {
        MButton *b = new MButton(QString("%1 %2").arg((char)0xc2b7).arg(i));
        b->setObjectName("AlphabetItem");
        connect(b, SIGNAL(clicked()), this, SLOT(button_clicked()));
        alphabetLayout->addItem(b);
    }
    if(list)
        m_scrollList = list;
}

AlphabetBar::~AlphabetBar()
{

}

void AlphabetBar::button_clicked()
{
    MButton *btn = static_cast<MButton *>(sender());
    qDebug() << "AlphabetBar clicked " << btn->text().at(2);
    emit clicked(btn->text().at(2));
}
