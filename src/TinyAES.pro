#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T11:56:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TinyAES
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

DESTDIR = ../bin
win32:CONFIG(release, debug|release): DESTDIR = ../Releasebin
else:win32:CONFIG(debug, debug|release): DESTDIR = ../Debugbin



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../External/tinyAES/lib/release/ -ltiny-aes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../External/tinyAES/lib/debug/ -ltiny-aes

INCLUDEPATH += $$PWD/../External/tinyAES/include
DEPENDPATH += $$PWD/../External/tinyAES/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../External/tinyAES/lib/release/libtiny-aes.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../External/tinyAES/lib/debug/libtiny-aes.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../External/tinyAES/lib/release/tiny-aes.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../External/tinyAES/lib/debug/tiny-aes.lib
