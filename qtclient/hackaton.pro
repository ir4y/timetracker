#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T11:09:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hackaton
TEMPLATE = app

INCLUDEPATH += /usr/include/libappindicator-0.1 \
        /usr/include/gtk-2.0 \
        /usr/lib/gtk-2.0/include \
        /usr/include/glib-2.0 \
        /usr/lib/glib-2.0/include \
        /usr/include/cairo \
        /usr/include/atk-1.0 \
        /usr/include/pango-1.0


SOURCES += main.cpp \
    client.cpp \
    mainwindow.cpp \
    taskselectdialog.cpp \
    authorizationdialog.cpp

HEADERS  += \
    client.h \
    mainwindow.h \
    taskselectdialog.h \
    authorizationdialog.h

FORMS += \
    taskselectdialog.ui \
    authorizationdialog.ui

LIBS += -L/usr/lib -lappindicator -lnotify

CONFIG += link_pkgconfig c++11

PKGCONFIG = gtk+-2.0

OTHER_FILES += \
    icon.svg

RESOURCES += \
    resources.qrc
