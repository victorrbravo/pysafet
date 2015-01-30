#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T14:51:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = safet
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../src \
                ../inflow

SOURCES += main.cpp

QT += core \
    sql \
    xml \
    gui \
    svg \
    network \
    webkit

LIBS += -L../lib \
      -L../inflow \
    -ltar \
    -lsafet


