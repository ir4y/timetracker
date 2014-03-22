#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T11:09:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hackaton
TEMPLATE = app


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
