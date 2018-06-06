QT = core widgets network networkauth
win32:QT += winextras
requires(qtConfig(tableview))
CONFIG -= app_bundle

TARGET = signal
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(src/src.pri)

HEADERS += \
    twitter.h \
    ui_twitterdialog.h \
    twittertimelinemodel.h

SOURCES +=

#Ref2
#HEADERS += src/Twitter.h
#SOURCES += src/twitter.cpp

RESOURCES += \
    resources/resource.qrc

DISTFILES += \
    appveyor.yml \
    icon.rc \
    app.ico \
    README.md

win32 {
    RC_FILE += icon.rc
}
