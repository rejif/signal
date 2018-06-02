QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = signal
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
include(src/src.pri)

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
