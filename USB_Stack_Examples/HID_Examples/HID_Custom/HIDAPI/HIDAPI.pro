#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T22:35:59
#
#-------------------------------------------------

QT       -= gui

TARGET = HIDAPI
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += hidapi.h

# -------------------------------------------------
# Add appropriate source file depending on OS
# -------------------------------------------------
macx:  SOURCES += mac/hid.c
unix: !macx:  SOURCES += linux/hid-libusb.c
win32: SOURCES += windows/hid.c

# -------------------------------------------------
# Make sure output directory for object file and
# library is in the correct subdirectory
# -------------------------------------------------
macx {
    DESTDIR = mac
    OBJECTS_DIR = mac
    MOC_DIR = mac
    UI_DIR = mac
    RCC_DIR = mac
}
unix: !macx {
    DESTDIR = linux
    OBJECTS_DIR = linux
    MOC_DIR = linux
    UI_DIR = linux
    RCC_DIR = linux
}
win32 {
    DESTDIR = windows
    OBJECTS_DIR = windows
    MOC_DIR = windows
    UI_DIR = windows
    RCC_DIR = windows
}
