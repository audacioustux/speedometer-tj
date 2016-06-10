#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T18:06:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = speedometer-tj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/qcustomplot.cpp

HEADERS  += mainwindow.h \
    lib/qcustomplot.h

FORMS    += mainwindow.ui
