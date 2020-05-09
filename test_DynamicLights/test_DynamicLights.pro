QT += testlib
QT += core
QT -= gui

CONFIG += sdk_no_version_check
CONFIG += c++11

TARGET = test_DynamicLights
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    TestDynamicLights.cpp

HEADERS += \
    TestDynamicLights.h

INCLUDEPATH += $$PWD/../DynamicLights/

# LIBS += -L$$PWD/../DynamicLights -lDynamicLights

