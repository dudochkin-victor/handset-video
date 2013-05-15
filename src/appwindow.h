/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <QWidget>
#include <QModelIndex>

#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MOverlay>
#include <MAction>

class AppVideosPage;
class VideoViewPage;
class AlphabetBar;
class VideoInfoPage;

class M_EXPORT AppWindow : public MApplicationWindow
{
    Q_OBJECT

public:
    enum PlayerMode {
        FileBrowse = 0,
        PlayVideo,
        Search,
        SearchResult
    };

    enum PlayerCategory {
        Showall = 0,
        MyMovies,
        Films,
        TVShows,
        RecentlyAdded,
        RecentlyViewed,
        Favourites
    };

    AppWindow(bool isSingleVideo, QWidget *parent = 0);
    virtual ~AppWindow();

    static AppWindow *instance();

private:
    void setModeCategory(PlayerMode mode, PlayerCategory category);
    void categoryChanged(PlayerCategory category);

public slots:
    void showShowall();
    void showMyMovies();
    void showFilms();
    void showTVShows();
    void showRecentlyAdded();
    void showRecentlyViewed();
    void showFavourites();
    void playVideo(const QString &filePath);
    void control_btn_press_handler(const QString &button);
    void backFromVideoviewpage();
    void showVideoInfoPage(const QModelIndex &index);
    void backFromVideoInfoPage();
    void viewSingleVideo(const QString &);

private:
    void actionSetBold(MAction *action, bool bold);
    void setAllActionNormal();

private:
    static AppWindow *mainWindowInstance;

    AppVideosPage *m_myShowallPage;
    AppVideosPage *m_myMoviesPage;
    AppVideosPage *m_myFilmsPage;
    AppVideosPage *m_myTVsPage;
    AppVideosPage *m_myRecentlyAddedPage;
    AppVideosPage *m_myRecentlyViewedPage;
    AppVideosPage *m_myFavouritesPage;
    VideoViewPage *m_nowPlayingPage;
    VideoInfoPage *m_videoInfoPage;
    AppVideosPage *m_currentPage;

    MOverlay *m_alphabetBarOverlay;
    MOverlay *m_controlPanelBar;

    AlphabetBar *m_alphabetBar;

    MAction *m_actionSearch;
    MAction *m_actionShowall;
    MAction *m_actionMyMovies;
    MAction *m_actionFilms;
    MAction *m_actionTVShows;
    MAction *m_actionRecentlyAdded;
    MAction *m_actionRecentlyViewed;
    MAction *m_actionFavourites;

    PlayerMode m_currentMode;
    PlayerCategory m_currentCategory;
    bool m_isSingleVideo;
};
#endif
