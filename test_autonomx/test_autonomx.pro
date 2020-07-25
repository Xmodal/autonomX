QT += testlib
QT += core
QT -= gui

CONFIG += sdk_no_version_check
CONFIG += c++11

TARGET = test_autonomx
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    TestAutonomX.cpp

HEADERS += \
    TestAutonomX.h

INCLUDEPATH += $$PWD/../autonomx/

# LIBS += -L$$PWD/../autonomx -lautonomx

