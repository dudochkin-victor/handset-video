/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>
#include <QSize>
#include <QScopedPointer>
#include <QSettings>


/// @class PreferencesPrivate
/// @brief A helper class that creates QSettings instance, keeps it
/// during the application life time and destroys at the end

class MetaInfo;
class XDGPath;

class PreferencesPrivate
{
public:
    /// @brief Return a pointer to QSettings
    /// @return QSettings.
    QSettings* settings();
private:
    QScopedPointer<QSettings> m_settings;
};


///
/// @class Preferences
/// @brief Preferences for video player
///
/// Class to handle preference/settings for video player.
///
/// Internally it uses QSettings and the settings are typically found
/// (on Linux systems) in ~/.config/MeeGo/Video\ Player.conf
///

class Preferences {

public:

    /// Specifies a category of video files.
    enum VideoFolderCategory { Error = -1 , MyMovies, Films, TVShows };

    /// @brief Supported modes: normal, looping over all videos, and looping over items from a list
    enum LaunchMode { NormalMode, LoopMode, FromListMode };

    /// @brief Return path to folder containing videos of a given category
    /// @param type videoFolderCategory The category
    /// @return The path to the folder.
    static QString folderPath(VideoFolderCategory type);

    /// @brief Set the path to a folder containing videos of a given category
    /// @param type videoFolderCategory The category
    /// @param path QString The file system path to the folder
    static void setFolderPath(VideoFolderCategory type, const QString &path);

    /// Specifies video metadata that can be displayed in the list view
    /// @todo These values are not yet finalized.
    enum MetaDataValue { filename, date, title, size, episodeName, showName, director, actor };

    /// @brief Return key for sort order when listing videos.
    /// @return The key to sort by
    static MetaDataValue listSortKey();

    /// @brief Set key for sort order when listing videos.
    /// @param key MetaDataValue The key to sort by
    static void setListSortKey(MetaDataValue key);

    /// @brief Return list of available metadata. This is a fixed list. Individual media files may or may not contain every item.
    /// @return List of available metadata.
    static QList<MetaDataValue> listMetaData();

    /// @brief Return list metadata to be displayed in list view for a given category.
    /// @param type The category
    /// @return List of metadata to be displayed
    static QList<MetaDataValue> listMetaDataToDisplay(VideoFolderCategory type);

    /// @brief Set list metadata to be displayed in list view for a given category.
    /// @param type The category
    /// @param values QList<metaDataValue> List of values to be displayed
    static void setListMetaDataToDisplay(VideoFolderCategory type, const QList<MetaDataValue> &values);

    /// @brief Return time after which media player control fade out when idle. The default is 3000 ms (3 seconds).
    /// @return Fade out time in milliseconds
    static int controlsFadeOutTime();

    /// @brief Set time after which media player control fade out when idle.
    /// @param ms int Time in milliseconds
    static void setControlsFadeOutTime(int ms);

    /// @brief Return last playback position for a given video file.
    /// @param filename QString video filename (less the path)
    /// @return Position of last playback, as a frame number. Returns -1 if video has not been played before.
    static qlonglong lastPlaybackPosition(const QString &filename);

    /// @brief Set last playback position for a given video file.
    /// @param filename QString video filename (less the path)
    /// @param frame long Position of last playback, as a frame number. Use -1 to mark video as not having been played before.
    static void setLastPlaybackPosition(const QString &filename, qlonglong frame);

    /// @brief Return size of video preview thumbnails. This is currently hardcoded to a specific size.
    /// @return Size of the thumbnail in pixels
    static QSize thumbnailSize();

    /// @brief Return screen size, in pixels.
    /// @return Screen size as a QSize.
    static QSize screenSize();

    /// @brief Set the active video category
    /// @param category VideoFolderCategory The category to select
    static void setCurrentCategory(VideoFolderCategory category);

    /// @brief Return currently active video category
    /// @return The current category
    static VideoFolderCategory currentCategory();

    /// @brief Sets the current filter.
    /// @param filter int Specifies the show filter.
    static void setCurrentFilter(int filter);

    /// @brief Return currently set show filter
    /// @return The current show filter
    static int currentFilter();

    /// @brief Set last played date and time for a given video file.
    /// @param filePath QString video file path
    /// @param date QDateTime Date and time of last playback. Use QDateTime() to mark video as not having been played before.
    static void setLastPlayedDate(const QString &filePath, const QDateTime & date);

    /// @brief Return last played date and time for a given video file.
    /// @param filePath QString video file path
    /// @return last played date and time as QDateTime. Returns invalid QDateTime if video has not been played before.
    static QDateTime lastPlayedDate(const QString &filePath);

    /// @brief Sets a given video file as favorite or not.
    /// @param filePath QString video file path
    /// @param favorite bool Defines whether set the file as favorite
    static void setFavorite(const QString &filePath, bool favorite);

    /// @brief Returns whether a given video file is favorite
    /// @param filePath QString video file path
    /// @return bool The value is true if a file is favorite, otherwise - false
    static bool isFavorite(const QString &filePath);

    /// @brief Set added date and time for a given video file.
    /// @param filePath QString video file path
    /// @param date QDateTime Date and time when file was added. Use QDateTime() to mark video as not having been added before.
    static void setDateAdded(const QString &filePath, const QDateTime & date);

    /// @brief Return added date and time for a given video file.
    /// @param filePath QString video file path
    /// @return added date and time as QDateTime. Returns invalid QDateTime if video has not been added before.
    static QDateTime dateAdded(const QString &filePath);

    /// @brief Sets the delay time before context menu is shown.
    /// @param delay int The delay time before context menu is shown.
    static void setShowContextMenuDelay(int delay);

    /// @brief Return the delay time before context menu is shown.
    /// @return The delay time before context menu is shown.
    static int showContextMenuDelay();

    /// @brief Sets the last search term.
    /// @param term QString The search term.
    static void setLastSearchTerm(const QString &term);

    /// @brief Return the last search term.
    /// @return The last search term.
    static QString lastSearchTerm();

    /// @brief set command line arguments
    /// @param argList QStringList The list of command line arguments
    static void loadArguments(const QStringList &argList);

    /// @brief Return list of files passed on the command line
    /// @return QStringList The files as a QStringList
    static QStringList filesList();

    /// @brief Return program launch mode, as set by command line arguments
    /// @return LaunchMode The selected launch mode
    static LaunchMode launchMode();

    /// @brief Save file metainfo to preferences
    /// @param mi QHash<QString, MetaInfo *> &mi) The metainfo
    static void saveMetaInfo(const QHash<QString, MetaInfo *> &mi);

    /// @brief Load metainfo from preferences file
    /// @param mi QHash<QString, MetaInfo *> &mi) The metainfo
    static bool loadMetaInfo(QHash<QString, MetaInfo *> &mi);

    /// @brief Return whether a message box which tells that MPlayer is missing should be displayed or not
    /// @return Bool value that indicates whether MPlayer missing dialog should be displayed or not
    static bool isDisplayMPlayerMessageEnabled();

    /// @brief Sets whether a message box which tells that MPlayer is missing should be displayed or not
    /// @param enabled bool The bool value that indicates whether MPlayer missing dialog should be displayed or not
    static void setDisplayMPlayerMessageEnabled(bool enabled);

    static void setLastViewedPage(const int page);

    static int lastViewedPage();

    static QString xdgVideoFolder();

private:

    static PreferencesPrivate m_private;

    static LaunchMode m_launchMode;

    static QStringList m_filesList;

    static XDGPath *m_xdgPath;
};

#endif // PREFERENCES_H
