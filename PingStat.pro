#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T08:28:08
#
#-------------------------------------------------

CODECFORTR      = UTF-8
CODECFORSRC     = UTF-8

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PingStat
TEMPLATE = app


SOURCES += main.cpp\
    StatsDisplayWidget.cpp \
    PlotWidget.cpp \
    MainWindow.cpp \
    HostChecker.cpp \
    Logger.cpp \
    Constants.cpp

HEADERS  += \
    StatsDisplayWidget.h \
    PlotWidget.h \
    MainWindow.h \
    HostChecker.h \
    Logger.h \
    Constants.h

FORMS    += \
    StatsDisplayWidget.ui \
    MainWindow.ui
