#!/bin/bash
# Builds the app with macdeployqt
# before running this script, build the app in Qt Creator with the Release configuration.
# (or with qmake)
# Next, run this script in order to include all the frameworks it needs into the .app bundle.

cd $(dirname ${0})/..

QT_VERSION_UNDERSCORE=5_12_11
QT_VERSION_DOTTED=5.12.11
PATH_TO_QT_BIN=~/Qt/${QT_VERSION_DOTTED}/clang_64/bin
PATH_TO_RELEASE_DOT_APP=../build-autonomx-Desktop_Qt_${QT_VERSION_UNDERSCORE}_clang_64bit-Release/autonomx/autonomx.app
PATH_TO_QML_DIR=./autonomx

${PATH_TO_QT_BIN}/macdeployqt ${PATH_TO_RELEASE_DOT_APP} -qmldir=${PATH_TO_QML_DIR} -verbose=3

