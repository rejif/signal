
#Ref1
QT = core widgets network networkauth
requires(qtConfig(tableview))
CONFIG -= app_bundle

HEADERS += \
    twitter.h \
    twittertimelinemodel.h

SOURCES += \
    main.cpp \
    twitter.cpp \
    twittertimelinemodel.cpp
FORMS += \
    twitterdialog.ui

#Ref2
#HEADERS += src/Twitter.h
#SOURCES += src/twitter.cpp


#QT       += core widgets network networkauth

#win32:QT += winextras

#TARGET = signal
#TEMPLATE = app

#DEFINES += QT_DEPRECATED_WARNINGS

#//include(src/src.pri)

#RESOURCES += \
#    resources/resource.qrc

#DISTFILES += \
#    appveyor.yml \
#    icon.rc \
#    app.ico \
#    README.md

#win32 {
#    RC_FILE += icon.rc
#}


#requires(qtConfig(tableview))
#CONFIG -= app_bundle

#HEADERS += \
#    twitter.h \
#    twittertimelinemodel.h

#SOURCES += \
#    main.cpp \
#    twitter.cpp \
#    twittertimelinemodel.cpp

#FORMS += \
#    twitterdialog.ui
