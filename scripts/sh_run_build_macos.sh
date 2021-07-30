#!/bin/bash
# Smoke tests the application by simply running it.

cd $(dirname ${0})/..

QT_VERSION_UNDERSCORE=5_12_11
PATH_TO_RELEASE_DOT_APP=./build-autonomx-Desktop_Qt_${QT_VERSION_UNDERSCORE}_clang_64bit-Release/autonomx/autonomx.app
EXEC_NAME=autonomx

${PATH_TO_RELEASE_DOT_APP}/Contents/MacOS/${EXEC_NAME}

