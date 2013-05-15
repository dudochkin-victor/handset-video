include (../common.pri)
TEMPLATE = app

contains(QT_CONFIG, opengl) : QT += opengl
QT += dbus
CONFIG += qttracker \
    mobility \
    qdbus

MOBILITY = multimedia
LIBS += -lqttracker

DBUS_ADAPTORS += dbus/com.meego.video.xml
dbusservice.files += dbus/com.meego.video.service
dbusservice.path = /usr/share/dbus-1/services/

system(qdbusxml2cpp -a video_adaptor.h: dbus/com.meego.video.xml)

SOURCES += main.cpp \
    appwindow.cpp \
    appvideospage.cpp \
    alphabetbar.cpp \
    videoslist.cpp \
    videoslistmodel.cpp \
    videoslistitemcreator.cpp \
    videoslistproxymodel.cpp \
    thumbnailer.cpp \
    thumbnailmanager.cpp \
    preferences.cpp \
    errormessage.cpp \
    videoproperty.cpp \
    videofileinfo.cpp \
    controlpanel.cpp \
    videoplayer.cpp \
    videoviewpage.cpp \
    videoseekbar.cpp \
    videocontrolpanel.cpp \
    ticker.cpp \
    videoinfowidget.cpp \
    videoinfopage.cpp \
    xdgpath.cpp \
    $$DBUS_ADAPTOR_SOURCES

HEADERS += appwindow.h \
    appvideospage.h \
    alphabetbar.h \
    videoslist.h \
    videoslistmodel.h \
    videoslistitemcreator.h \
    videoslistproxymodel.h \
    thumbnailer.h \
    thumbnailmanager.h \
    preferences.h \
    metainfo.h \
    errormessage.h \
    videoproperty.h \
    videofileinfo.h \
    controlpanel.h \
    videoplayer.h \
    videoviewpage.h \
    videoseekbar.h \
    videocontrolpanel.h \
    ticker.h \
    videoinfowidget.h \
    videoinfopage.h \
    xdgpath.h \
    $$DBUS_ADAPTOR_HEADERS

TRANSLATIONS += translations/videoplayer_fr.ts \
    translations/videoplayer_ru.ts \
    translations/videoplayer_zh_CN.ts

CONFIG(debug, debug|release) {
    MOC_DIR = tmp/debug/moc
    OBJECTS_DIR = tmp/debug/obj
    UI_DIR = tmp/debug/ui
    RCC_DIR = tmp/debug/rcc
    #unix:TARGET = $$join(TARGET,,,_debug)
    #else:TARGET = $$join(TARGET,,,d)
    DEFINES += DEBUG_VERSION
}
else {
    MOC_DIR = tmp/release/moc
    OBJECTS_DIR = tmp/release/obj
    UI_DIR = tmp/release/ui
    RCC_DIR = tmp/release/rcc
}

MGEN_OUTDIR = tmp
mgen.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
mgen.clean += $$GEN_DIR/gen_*

target.path = $$M_INSTALL_BIN

INSTALLS += target dbusservice
