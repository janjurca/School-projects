#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T10:12:36
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plotter
TEMPLATE = app

RC_FILE = plotter.rc
SOURCES += main.cpp\
        mainwindow.cpp \
        vlakno.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
            vlakno.h \
    qcustomplot.h

FORMS    += mainwindow.ui
