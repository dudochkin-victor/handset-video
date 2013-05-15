/*
 * MeeGo Handset Video Player
 * Copyright (c) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "ticker.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QGraphicsLinearLayout>


Ticker::Ticker(QGraphicsItem *parent)
    : MLabel(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(moving()));
     
}

Ticker::~Ticker()
{
    delete m_timer;
}


void Ticker::startScrolling()
{
    m_timer->start(300);  
}

void Ticker::stop()
{
  
}

void Ticker::setText2(const QString & text)
{
    QFontMetrics fm(this->font());
    QSizeF textSize = fm.boundingRect(text).size();
    qDebug() << "moving: textSize: " << textSize;
    qDebug() << "moving: fm.width(): " << fm.width(text); 
    
    qDebug() << "moving:m_tickertext().rect() " << this->rect();
    qDebug() << "moving:boundingRect().width() " << boundingRect().width();
    qDebug() << "moving: m_tickertext->size()" << this->size();
    
    qDebug() << "moving:style->paddingLeft():" << this->style()->paddingLeft();
    qDebug() << "moving:style->paddingRight():" << this->style()->paddingRight();
    
    m_text = text;
    qint64 width;
    width = boundingRect().width() - style()->paddingLeft() - style()->paddingRight();
    if (fm.width(text) > width) {
        m_text = QString(m_text + "    ");
    	startScrolling();
    }
    
    MLabel::setText(text);
}


#if 0
void Ticker::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    //m_tickertext->setPaintOffset(QPointF::QPointF(shift, 0));
    //shift--;
}
#endif

void Ticker::moving() {
    //m_tickertext->setText(m_text);
    QChar tmpchar;
    tmpchar = m_text.at(0);
    m_text.remove(0, 1);
    m_text.append(tmpchar);
    MLabel::setText(m_text);
   
    m_timer->start(300);
}

