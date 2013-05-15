/*
 * MeeGo Handset Video Player
 * Copyright (c) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TICKER_H
#define TICKER_H

#include <QGraphicsWidget>
#include <MStylableWidget>
#include <QTimeLine>
#include <MLabel>


class Ticker : public MLabel
{
    Q_OBJECT
public:
  
    Ticker(QGraphicsItem *parent = 0);

    ~Ticker();

    void setText(const QString & text);
    int scrollingStatus();

public slots:
   
    void startScrolling();
    
    void stop();
    
    void setText2(const QString & text);
	

private slots:
    void moving();

protected:
    //virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
   

private:
    int m_scrollOffset;
    int m_oldOffset;
    qreal m_textOffset;
    bool m_needsScrolling;
    int shift;
    static const int k_textMargin = 10;
    QTimer *m_timer;
    QString m_text;
};

#endif
