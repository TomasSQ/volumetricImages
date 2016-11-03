#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T22:28:01
#
#-------------------------------------------------

CONFIG += static
QT       += core gui
QMAKE_CXXFLAGS+= -std=c++11
QMAKE_CXXFLAGS+= -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teste2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    libscn.cpp \
    libscnqt.cpp \
    colorizedialog.cpp

HEADERS  += mainwindow.h \
    prettyprint.hpp \
    libscn.hpp \
    libscnqt.h \
    colorizedialog.h

FORMS    += mainwindow.ui \
    colorizedialog.ui
