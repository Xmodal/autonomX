Rem go back one directory
cd %~dp0..

Rem variables
set QT_VERSION_UNDERSCORE=5_12_11
set QT_VERSION_DOTTED=5.12.11
set COMPILER_BIN=mingw73_64
set COMPILER_BUILD=MinGW_64_bit

Rem paths
set PATH_TO_QT_BIN=C:/Qt/%QT_VERSION_DOTTED%/%COMPILER_BIN%
set PATH_TO_RELEASE=../build-autonomx-Desktop_Qt_%QT_VERSION_UNDERSCORE%_%COMPILER_BUILD%-Release/autonomx/release/autonomx.exe
set PATH_TO_QML_DIR=./autonomx

Rem deploy here
%PATH_TO_QT_BIN%/windeployqt %PATH_TO_RELEASE% --qmldir %PATH_TO_QML_DIR% -verbose=3
