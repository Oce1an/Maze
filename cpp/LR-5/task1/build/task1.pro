QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = task1
TEMPLATE = app

SOURCES += ../main.cpp \
           ../mainwindow.cpp \
           ../trainer.cpp

HEADERS += ../mainwindow.h \
           ../trainer.h

FORMS   += ../mainwindow.ui
