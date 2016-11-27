#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T22:28:01
#
#-------------------------------------------------


QT       += core gui
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teste2
TEMPLATE = app
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp


SOURCES += main.cpp\
        mainwindow.cpp \
    libscn.cpp \
    libscnqt.cpp \
    colorizedialog.cpp \
    tmat.cpp \
    rendering.cpp

HEADERS  += mainwindow.h \
    prettyprint.hpp \
    libscn.hpp \
    libscnqt.h \
    colorizedialog.h \
    tmat.hpp \
    rendering.h

FORMS    += mainwindow.ui \
    colorizedialog.ui \
    rendering.ui
