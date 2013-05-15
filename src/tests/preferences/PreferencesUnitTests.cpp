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

#include <QApplication>
#include "PreferencesUnitTests.h"
#include "preferences.h"

void PreferencesUnitTests::initTestCase()
{
    // QSettings may not work if these values are not set.
    // We use different application name so as not to overwrite the user's real application settings.
    qApp->setOrganizationName("MeeGo");
    qApp->setOrganizationDomain("meego.com");
    qApp->setApplicationName("Video Player Test");
}

void PreferencesUnitTests::cleanupTestCase()
{
}

void PreferencesUnitTests::folderPathTest()
{
    // Basic tests of folder path setters and getters
    QString homeDir = QDir::homePath();

    // Invalid category - should return empty string
    QCOMPARE(Preferences::folderPath((Preferences::VideoFolderCategory)1234), QString(""));
    // Valid category - check for default
    QCOMPARE(Preferences::folderPath(Preferences::MyMovies), homeDir + "/Videos/My Movies");
    // Valid category - check for default
    QCOMPARE(Preferences::folderPath(Preferences::Films), homeDir + "/Videos/Films");
    // Valid category - check for default
    QCOMPARE(Preferences::folderPath(Preferences::TVShows), homeDir + "/Videos/TV Shows");

    // Try to set invalid path, should still return empty string
    Preferences::setFolderPath((Preferences::VideoFolderCategory)1234, "/bad/path");
    QCOMPARE(Preferences::folderPath((Preferences::VideoFolderCategory)1234), QString(""));
    // Test setting valid categories
    Preferences::setFolderPath(Preferences::MyMovies, "/file/path1");
    Preferences::setFolderPath(Preferences::Films, "/file/path2");
    Preferences::setFolderPath(Preferences::TVShows, "/file/path3");
    QCOMPARE(Preferences::folderPath(Preferences::MyMovies), QString("/file/path1"));
    QCOMPARE(Preferences::folderPath(Preferences::Films), QString("/file/path2"));
    QCOMPARE(Preferences::folderPath(Preferences::TVShows), QString("/file/path3"));

    // Put default values back
    Preferences::setFolderPath(Preferences::MyMovies, homeDir + "/Videos/My Movies");
    Preferences::setFolderPath(Preferences::Films, homeDir + "/Videos/Films");
    Preferences::setFolderPath(Preferences::TVShows, homeDir + "/Videos/TV Shows");
}

void PreferencesUnitTests::listSortKeyTest()
{
    // Default value
    QCOMPARE(Preferences::listSortKey(), Preferences::title);

    // Test settter
    Preferences::setListSortKey(Preferences::filename);
    QCOMPARE(Preferences::listSortKey(), Preferences::filename);
    Preferences::setListSortKey(Preferences::date);
    QCOMPARE(Preferences::listSortKey(), Preferences::date);
    Preferences::setListSortKey(Preferences::size);
    QCOMPARE(Preferences::listSortKey(), Preferences::size);
    Preferences::setListSortKey(Preferences::title);
    QCOMPARE(Preferences::listSortKey(), Preferences::title);
}


void PreferencesUnitTests::listMetaDataTest()
{
    QList<Preferences::MetaDataValue> list;
    list << Preferences::filename << Preferences::date << Preferences::title << Preferences::size << Preferences::episodeName << Preferences::showName << Preferences::director << Preferences::actor;
    
    QCOMPARE(Preferences::listMetaData(), list);

    list.clear();
    list << Preferences::title << Preferences::date;
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);

    list.clear();
    list << Preferences::title << Preferences::director;
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::Films), list);

    list.clear();
    list << Preferences::episodeName << Preferences::showName;
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::TVShows), list);

    list.clear();
    Preferences::setListMetaDataToDisplay(Preferences::MyMovies, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);
    Preferences::setListMetaDataToDisplay(Preferences::Films, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::Films), list);
    Preferences::setListMetaDataToDisplay(Preferences::TVShows, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::TVShows), list);

    list << Preferences::size;
    Preferences::setListMetaDataToDisplay(Preferences::MyMovies, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);

    list << Preferences::filename;
    Preferences::setListMetaDataToDisplay(Preferences::MyMovies, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);

    list << Preferences::filename << Preferences::date << Preferences::title << Preferences::size;
    Preferences::setListMetaDataToDisplay(Preferences::MyMovies, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);

    // Reset to defaults
    list.clear();
    list << Preferences::title << Preferences::date;
    Preferences::setListMetaDataToDisplay(Preferences::MyMovies, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::MyMovies), list);

    list.clear();
    list << Preferences::title << Preferences::director;
    Preferences::setListMetaDataToDisplay(Preferences::Films, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::Films), list);

    list.clear();
    list << Preferences::episodeName << Preferences::showName;
    Preferences::setListMetaDataToDisplay(Preferences::TVShows, list);
    QCOMPARE(Preferences::listMetaDataToDisplay(Preferences::TVShows), list);

}

void PreferencesUnitTests::fadeoutTimeTest()
{
    QCOMPARE(Preferences::controlsFadeOutTime(), 3000);
    Preferences::setControlsFadeOutTime(2000);
    QCOMPARE(Preferences::controlsFadeOutTime(), 2000);
    Preferences::setControlsFadeOutTime(3000);
    QCOMPARE(Preferences::controlsFadeOutTime(), 3000);
}

void PreferencesUnitTests::lastPlaybackPositionTest()
{
    QCOMPARE(Preferences::lastPlaybackPosition("file1.mpg"), (qlonglong)-1);
    QCOMPARE(Preferences::lastPlaybackPosition("file2.mpg"), (qlonglong)-1);
    Preferences::setLastPlaybackPosition("file1.mpg", (qlonglong)1234);
    Preferences::setLastPlaybackPosition("file2.mpg", (qlonglong)2468);
    QCOMPARE(Preferences::lastPlaybackPosition("file1.mpg"), (qlonglong)1234);
    QCOMPARE(Preferences::lastPlaybackPosition("file2.mpg"), (qlonglong)2468);
    Preferences::setLastPlaybackPosition("file1.mpg", (qlonglong)-1);
    Preferences::setLastPlaybackPosition("file2.mpg", (qlonglong)-1);
    QCOMPARE(Preferences::lastPlaybackPosition("file1.mpg"), (qlonglong)-1);
    QCOMPARE(Preferences::lastPlaybackPosition("file2.mpg"), (qlonglong)-1);
}

void PreferencesUnitTests::thumbnailSizeTest()
{

    QCOMPARE(Preferences::thumbnailSize(), QSize(110, 110));
}

QTEST_MAIN(PreferencesUnitTests)
