TEMPLATE = app
INCLUDEPATH += ../../
QT += testlib
CONFIG += link_pkgconfig \
            meegotouch
SOURCES = PreferencesUnitTests.cpp ../../preferences.cpp ../../videofileinfo.cpp
HEADERS = PreferencesUnitTests.h
