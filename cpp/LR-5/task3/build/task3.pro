QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = task3
TEMPLATE = app

SOURCES += ../main.cpp \
           ../mainwindow.cpp \
           ../mybitset.cpp

HEADERS += ../mainwindow.h \
           ../mybitset.h

FORMS   += ../mainwindow.ui
