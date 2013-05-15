/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <MWidget>

#include <QMediaPlayer>
 #include <QUrl>

#if QT_VERSION < 0x040700
QTM_BEGIN_NAMESPACE
#endif
class QGraphicsVideoItem;
class QMediaPlayer;
class QMediaPlaylist;
#if QT_VERSION < 0x040700
QTM_END_NAMESPACE
QTM_USE_NAMESPACE
#endif


//class QGraphicsVideoItem;
//class QMediaPlayer;
//class QMediaPlaylist;

class VideoPlayer : public MWidget
{
    Q_OBJECT
public:
    VideoPlayer(QGraphicsItem *parent = 0);

    ~VideoPlayer();

    /// @brief Set video to play
    /// @param url QUrl URL of video to play
    /// @param description QString Sets description for the video
    bool setMediaContent(const QUrl &url, const QString &description);

    qint64 duration() const;

    void setErrorNotificationsEnabled(bool enabled);

    bool isErrorNotificationsEnabled() const;
	
    int state();
	
	QUrl mediaUrl();
	
	enum PlaybackState {
	    StoppedState =0,
	    PlayingState,
	    PausedState,
	    fforwardState,
	    fbackwardState
    };
	
public slots:

    /// @brief Slot to start playing a video
    /// @param fromFrame qint64 The number of the start frame
    void play(qint64 fromFrame = -1);

    /// @brief Play a lsit of videos continuously in a loop
    /// @param items QList<QMediaContent>& The list of media to play
    void playInLoop(const QList<QMediaContent> & items);

    /// @brief Pause playback of the video
    void pause();

    /// @brief Stop playback of the video
    void stop();
    
    void seek(int seconds);
    
    
    /// @brief Move video playback forward to next chapter or a fixed time period if no chapters exist
    void windForward();

    /// @brief Move video playback back to previous chapter or a fixed time period if no chapters exist
    void windBackward();

    void closePlayer();

signals:
    /// @brief Signal emitted when and error occurred
    /// @param error QMediaPlayer::error Error code
    /// @param errorString QString Text description of the error
    void errorOccurred(QMediaPlayer::Error error, const QString & errorString);

    /// @brief Signal emitted when the video has completely finished
    void videoFinished();

    void playerStateChanged(VideoPlayer::PlaybackState state);

    void positionChanged(qint64);

    void durationChanged(qint64 duration);
    
    void mediaChanged(const QMediaContent & media);

protected:
    /// @brief Resize event
    /// @param event QGraphicsSceneResizeEvent The resize event to be handled
    virtual void resizeEvent(QGraphicsSceneResizeEvent * event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

private slots:
    void handleDurationChanged(qint64);
    void handlePlayerStateChanged(QMediaPlayer::State state);
    void handleError(QMediaPlayer::Error);
    void mediaStatusChanged(QMediaPlayer::MediaStatus);

private:
    static qreal Factors[];
    enum FactorIndex { MinFactorIndex = 0, ZeroFactorIndex = 3, MaxFactorIndex = 6 };

    QGraphicsVideoItem * m_videoItem;
    QMediaPlayer * m_mediaPlayer;
    QMediaPlaylist* m_playlist;

    qint64 m_startFrame;
    bool m_errorNotificationsEnabled;
    bool m_errorOccurred;
    int m_factorIndex;
    QUrl m_currentmediaURL;
};

#endif // VIDEOPLAYER_H
