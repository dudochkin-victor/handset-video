/*
 * Copyright (C) 2010 Intel Corporation.
 *
 * Author:  
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "VideoFileInfoUnitTests.h"
#include "videofileinfo.h"


void VideoFileInfoUnitTests::initTestCase()
{
}

void VideoFileInfoUnitTests::cleanupTestCase()
{
}

void VideoFileInfoUnitTests::setterGetterTest()
{
    VideoFileInfo info;

    QDateTime dateTime1 = QDateTime::currentDateTime();
    QDateTime dateTime2 = dateTime1.addDays(1);

    info.setLastPlayedDate(dateTime1);
    QCOMPARE(info.lastPlayedDate(), dateTime1);

    QCOMPARE(info.isFavorite(), false);
    info.setFavorite(true);
    QCOMPARE(info.isFavorite(), true);
    info.setFavorite(false);
    QCOMPARE(info.isFavorite(), false);

    info.setDateAdded(dateTime2);
    QCOMPARE(info.dateAdded(), dateTime2);
    QCOMPARE(info.lastPlayedDate(), dateTime1);
}

void VideoFileInfoUnitTests::toFromStringTest()
{
    VideoFileInfo info;
    QDateTime dateTime1 = QDateTime::currentDateTime();
    QDateTime dateTime2 = dateTime1.addDays(1);

    info.setFavorite(true);
    info.setLastPlayedDate(dateTime1);
    info.setDateAdded(dateTime2);

    QString expected = QString("1;%1;%2").arg(dateTime1.toString()).arg(dateTime2.toString());
    QCOMPARE(info.toString(), expected);

    info.setFavorite(false);
    expected = QString("0;%1;%2").arg(dateTime1.toString()).arg(dateTime2.toString());
    QCOMPARE(info.toString(), expected);
}

QTEST_MAIN(VideoFileInfoUnitTests)
