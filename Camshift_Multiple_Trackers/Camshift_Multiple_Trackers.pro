#-------------------------------------------------
#
# Project created by QtCreator 2013-02-24T18:25:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Camshift_Multiple_Trackers
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcvcamshifttracker.cpp

HEADERS  += mainwindow.h \
    qcvcamshifttracker.h

FORMS    += mainwindow.ui

include(d:/opencv2/opencv_static.pri)

VERSION = 1.0
