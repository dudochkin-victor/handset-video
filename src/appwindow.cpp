/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QGraphicsLinearLayout>
#include <QStackedLayout>
#include <QUrl>
#include <MOverlay>
#include <MAction>
#include <MApplicationMenu>

#include "appwindow.h"
#include "appvideospage.h"
#include "videoviewpage.h"
#include "alphabetbar.h"
#include "preferences.h"
#include "controlpanel.h"
#include "videoinfopage.h"
#include "video_adaptor.h"

AppWindow *AppWindow::mainWindowInstance = NULL;

AppWindow *AppWindow::instance()
{
    return mainWindowInstance;
}

AppWindow::AppWindow(bool isSingleVideo, QWidget *parent) : MApplicationWindow(parent)
{
    mainWindowInstance = this;
    new VideoAdaptor(this);
    m_isSingleVideo = isSingleVideo;
    //////////////////////////
    // Create Show All
    //////////////////////////
    m_myShowallPage = new AppVideosPage(AppWindow::Showall, NULL);

    //////////////////////////
    // Create My Movies
    //////////////////////////
    m_myMoviesPage = new AppVideosPage(AppWindow::MyMovies, NULL);

    //////////////////////////
    // Create My Films
    //////////////////////////
    m_myFilmsPage = new AppVideosPage(AppWindow::Films, NULL);

    //////////////////////////
    // Create My TVs
    //////////////////////////
    m_myTVsPage = new AppVideosPage(AppWindow::TVShows, NULL);

    //////////////////////////
    // Create Recently Added
    //////////////////////////
    m_myRecentlyAddedPage = new AppVideosPage(AppWindow::RecentlyAdded, NULL);

    //////////////////////////
    // Create Recently Viewed
    //////////////////////////
    m_myRecentlyViewedPage = new AppVideosPage(AppWindow::RecentlyViewed, NULL);

    //////////////////////////
    // Create Favourites
    //////////////////////////
    m_myFavouritesPage = new AppVideosPage(AppWindow::Favourites, NULL);

    connect(m_myShowallPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myMoviesPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myFilmsPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myTVsPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myRecentlyAddedPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myRecentlyViewedPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));
    connect(m_myFavouritesPage, SIGNAL(videoItemClicked(const QString &)), this, SLOT(playVideo(const QString &)));

    connect(m_myShowallPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myMoviesPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myFilmsPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myTVsPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myRecentlyAddedPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myRecentlyViewedPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));
    connect(m_myFavouritesPage, SIGNAL(videoInfoClicked(const QModelIndex&)), this, SLOT(showVideoInfoPage(const QModelIndex&)));

    //////////////////////////
    // Create Actions
    //////////////////////////
    //% "Search"
    m_actionSearch = new MAction(qtTrId("meego_video_search"), this);
    m_actionSearch->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionSearch);
    
    //% "Show all"
    m_actionShowall = new MAction(qtTrId("meego_video_show_all"), this);
    m_actionShowall->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionShowall);
    
    //% "My Movies"
    m_actionMyMovies = new MAction(qtTrId("meego_video_my_movies"), this);
    m_actionMyMovies->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionMyMovies);

    //% "Films"
    m_actionFilms = new MAction(qtTrId("meego_video_films"), this);
    m_actionFilms->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionFilms);

    //% "TV Shows"
    m_actionTVShows = new MAction(qtTrId("meego_video_tv_shows"), this);
    m_actionTVShows->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionTVShows);
    
    //% "Recently Added"
    m_actionRecentlyAdded = new MAction(qtTrId("meego_video_recently_added"), this);
    m_actionRecentlyAdded->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionRecentlyAdded);

    //% "Recently Viewed"
    m_actionRecentlyViewed = new MAction(qtTrId("meego_video_recently_viewed"), this);
    m_actionRecentlyViewed->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionRecentlyViewed);

    //% "Favorites"
    m_actionFavourites = new MAction(qtTrId("meego_video_favorites"), this);
    m_actionFavourites->setLocation(MAction::ApplicationMenuLocation);
    addAction(m_actionFavourites);

    connect(m_actionShowall, SIGNAL(triggered(bool)), this, SLOT(showShowall()));
    connect(m_actionMyMovies, SIGNAL(triggered(bool)), this, SLOT(showMyMovies()));
    connect(m_actionFilms, SIGNAL(triggered(bool)), this, SLOT(showFilms()));
    connect(m_actionTVShows, SIGNAL(triggered(bool)), this, SLOT(showTVShows()));
    connect(m_actionRecentlyAdded, SIGNAL(triggered(bool)), this, SLOT(showRecentlyAdded()));
    connect(m_actionRecentlyViewed, SIGNAL(triggered(bool)), this, SLOT(showRecentlyViewed()));
    connect(m_actionFavourites, SIGNAL(triggered(bool)), this, SLOT(showFavourites()));

    //////////////////////////
    // Create Video Info Page
    //////////////////////////
    m_videoInfoPage = new VideoInfoPage(NULL);
    connect(m_videoInfoPage, SIGNAL(backButtonClicked ()), this, SLOT(backFromVideoInfoPage()));

    //////////////////////////
    // Create NowPlaying
    //////////////////////////
    m_nowPlayingPage = new VideoViewPage();
    //% "Video player"
    m_nowPlayingPage->setTitle(qtTrId("meego_video_title"));
    m_nowPlayingPage->setObjectName("NowPlayingPage");

    m_nowPlayingPage->setComponentsDisplayMode(MApplicationPage::AllComponents,
                                              MApplicationPageModel::AutoHide);

    m_alphabetBarOverlay = new MOverlay;
    //m_alphabetBarOverlay->setViewType("background");
    m_alphabetBarOverlay->setObjectName("AlphabetBarOverLay");

    m_alphabetBar = new AlphabetBar(NULL, NULL);
    MWidget *alphabetBarOverlayWidget = m_alphabetBarOverlay->widget();
    QGraphicsLinearLayout *alphabetBarOverlayLayout = new QGraphicsLinearLayout(Qt::Horizontal, alphabetBarOverlayWidget);
    alphabetBarOverlayLayout->setContentsMargins(0,0,0,0);
    alphabetBarOverlayLayout->setSpacing(0);
    alphabetBarOverlayLayout->addItem(m_alphabetBar);
    m_alphabetBarOverlay->appear(this);
    m_alphabetBarOverlay->setZValue(500-1); /* to avoid blocking pop down actions */

    // Create Control Panel
    m_controlPanelBar = new MOverlay;
    //m_controlPanelBar->setViewType("background");
    m_controlPanelBar->setObjectName("ControlPanelOverLay");
    m_controlPanelBar->setContentsMargins(0, 0, 0, 0);
    MWidget *controlPanelWidget = m_controlPanelBar->widget();
    QGraphicsLinearLayout *controlPanelLayout = new QGraphicsLinearLayout(Qt::Horizontal, controlPanelWidget);
    controlPanelLayout->setSpacing(0);
    controlPanelLayout->setContentsMargins(0, 0, 0, 0);

    ControlPanel *controlPanel = new ControlPanel(NULL);
    connect(controlPanel, SIGNAL(btn_clicked(const QString &)), this, SLOT(control_btn_press_handler(const QString &)));
    controlPanelLayout->addItem(controlPanel);
    m_controlPanelBar->appear(this);
    m_controlPanelBar->setZValue(500-1);

    connect(m_nowPlayingPage, SIGNAL(StateChanged(QMediaPlayer::State)), controlPanel, SLOT(control_button_update(QMediaPlayer::State)));
    connect(m_nowPlayingPage, SIGNAL(backButtonClicked ()), this, SLOT(backFromVideoviewpage()));

    AppWindow::PlayerCategory category = static_cast<AppWindow::PlayerCategory>(Preferences::lastViewedPage());
    setModeCategory(AppWindow::FileBrowse, category);
}

AppWindow::~AppWindow()
{
}

void AppWindow::actionSetBold(MAction *action, bool bold)
{
    if (true == bold) {
        QString text = "<b>" + action->text() + "</b>";
        action->setText(text);
    } else {
        QString text = action->text().remove("<b>").remove("</b>");
        action->setText(text);
    }
}

void AppWindow::setAllActionNormal()
{
    actionSetBold(m_actionSearch, false);
    actionSetBold(m_actionShowall, false);
    actionSetBold(m_actionMyMovies, false);
    actionSetBold(m_actionFilms, false);
    actionSetBold(m_actionTVShows, false);
    actionSetBold(m_actionRecentlyAdded, false);
    actionSetBold(m_actionRecentlyViewed, false);
    actionSetBold(m_actionFavourites, false);
}

void AppWindow::setModeCategory(PlayerMode mode, PlayerCategory category)
{
    switch(mode) {
    case AppWindow::FileBrowse:
        {
            m_nowPlayingPage->disappear();
            m_videoInfoPage->disappear();
            m_alphabetBarOverlay->appear(this);
            m_controlPanelBar->appear();
            categoryChanged(category);
            break;
        }
    case AppWindow::Search:
        {
            break;
        }
    case AppWindow::SearchResult:
        {
            break;
        }
    case AppWindow::PlayVideo:
        {
            //m_myShowallPage->disappear();
            //m_myMoviesPage->disappear();
            //m_myFilmsPage->disappear();
            //m_myTVsPage->disappear();
            //m_myRecentlyAddedPage->disappear();
            //m_myRecentlyViewedPage->disappear();
            //m_myFavouritesPage->disappear();
            m_alphabetBarOverlay->disappear();
            m_controlPanelBar->disappear();
            m_nowPlayingPage->appear(this);
            break;
        }
    default:
        {
            break;
        }
    }
    m_currentMode = mode;
    m_currentCategory = category;
}

void AppWindow::categoryChanged(PlayerCategory category)
{
    Preferences::setLastViewedPage(static_cast<int>(category));

    if (category == AppWindow::Showall) {
        m_myMoviesPage->disappear();
        m_myFilmsPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myShowallPage->appear(this);
        m_currentPage = m_myShowallPage;

        setAllActionNormal();
        actionSetBold(m_actionShowall, true);

        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    }
    else if (category == AppWindow::MyMovies) {
        m_myShowallPage->disappear();
        m_myFilmsPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myMoviesPage->appear(this);
        m_currentPage = m_myMoviesPage;

        setAllActionNormal();
        actionSetBold(m_actionMyMovies, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    } else if (category == AppWindow::Films) {
        m_myShowallPage->disappear();
        m_myMoviesPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myFilmsPage->appear(this);
        m_currentPage = m_myFilmsPage;

        setAllActionNormal();
        actionSetBold(m_actionFilms, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    } else if (category == AppWindow::TVShows) {
        m_myShowallPage->disappear();
        m_myMoviesPage->disappear();
        m_myFilmsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myTVsPage->appear(this);
        m_currentPage = m_myTVsPage;

        setAllActionNormal();
        actionSetBold(m_actionTVShows, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    } else if (category == AppWindow::RecentlyAdded) {
        m_myShowallPage->disappear();
        m_myMoviesPage->disappear();
        m_myFilmsPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myRecentlyAddedPage->appear(this);
        m_currentPage = m_myRecentlyAddedPage;

        setAllActionNormal();
        actionSetBold(m_actionRecentlyAdded, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    } else if (category == AppWindow::RecentlyViewed) {
        m_myShowallPage->disappear();
        m_myMoviesPage->disappear();
        m_myFilmsPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myFavouritesPage->disappear();
        m_myRecentlyViewedPage->updateRecentlyViewed();
        m_myRecentlyViewedPage->appear(this);
        m_currentPage = m_myRecentlyViewedPage;

        setAllActionNormal();
        actionSetBold(m_actionRecentlyViewed, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    } else if (category == AppWindow::Favourites) {
        m_myShowallPage->disappear();
        m_myMoviesPage->disappear();
        m_myFilmsPage->disappear();
        m_myTVsPage->disappear();
        m_myRecentlyAddedPage->disappear();
        m_myRecentlyViewedPage->disappear();
        m_myFavouritesPage->updateFavourites();
        m_myFavouritesPage->appear(this);
        m_currentPage = m_myFavouritesPage;

        setAllActionNormal();
        actionSetBold(m_actionFavourites, true);

        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myShowallPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myMoviesPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFilmsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myTVsPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyAddedPage, SLOT(scrollTo(QString)));
        disconnect(m_alphabetBar, SIGNAL(clicked(QString)), m_myRecentlyViewedPage, SLOT(scrollTo(QString)));
        connect(m_alphabetBar, SIGNAL(clicked(QString)), m_myFavouritesPage, SLOT(scrollTo(QString)));
    }
}

void AppWindow::showShowall()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::Showall);
}

void AppWindow::showMyMovies()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::MyMovies);
}

void AppWindow::showFilms()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::Films);
}

void AppWindow::showTVShows()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::TVShows);
}

void AppWindow::showRecentlyAdded()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::RecentlyAdded);
}

void AppWindow::showRecentlyViewed()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::RecentlyViewed);
}

void AppWindow::showFavourites()
{
    setModeCategory(AppWindow::FileBrowse, AppWindow::Favourites);
}

void AppWindow::playVideo(const QString &filePath)
{
    qDebug() << "AppWindow::playVideo " << filePath;

    setModeCategory(AppWindow::PlayVideo, m_currentCategory);

    if (filePath.isEmpty())
        return;

    QString text = QString(filePath);

    m_nowPlayingPage->player()->setErrorNotificationsEnabled(false);
    if (!m_nowPlayingPage->player()->setMediaContent(QUrl::fromLocalFile(filePath), text)) {
        m_nowPlayingPage->player()->setErrorNotificationsEnabled(true);
        return;
    }

    m_nowPlayingPage->player()->setErrorNotificationsEnabled(true);
    m_nowPlayingPage->player()->play(Preferences::lastPlaybackPosition(filePath));
}

void AppWindow::control_btn_press_handler(const QString &press_btn)
{   
    qDebug() << "ControlPanel AppWindow::control_btn_press_handler";
    if (!QString::compare(press_btn, "play")) {
        if (m_nowPlayingPage) {
            QString filePath = m_currentPage->firstVideoPath();
            playVideo(filePath);
        }
    }
#if 0
    if (!QString::compare(press_btn, "pause")) {
        if (m_nowPlayingPage) {
            m_nowPlayingPage->player()->pause();
        }
    }
    if (!QString::compare(press_btn, "forward_enabled")) {
        if (m_nowPlayingPage) {
            if (m_nowPlayingPage->player()->state() == QMediaPlayer::PlayingState)
                m_nowPlayingPage->player()->windForward();
        }
    }
    if (!QString::compare(press_btn, "back_enabled"))
        if (m_nowPlayingPage) {
            if (m_nowPlayingPage->player()->state() == QMediaPlayer::PlayingState)
                m_nowPlayingPage->player()->windBackward();
    }
#endif
}


void AppWindow::backFromVideoviewpage() {
    m_controlPanelBar->appear(this);
    m_alphabetBarOverlay->appear();
}

void AppWindow::showVideoInfoPage(const QModelIndex &index)
{
    m_videoInfoPage->showVideoInfo(index);
    m_controlPanelBar->disappear();
    m_alphabetBarOverlay->disappear();
    m_videoInfoPage->appear(this);
}

void AppWindow::backFromVideoInfoPage()
{
    setModeCategory(AppWindow::FileBrowse, m_currentCategory);
}

void AppWindow::viewSingleVideo(const QString &filePath)
{
    playVideo(filePath);
}


