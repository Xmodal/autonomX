Rem go back one directory
cd %~dp0..

Rem variables
set QT_VERSION_UNDERSCORE=5_12_5
set QT_VERSION_DOTTED=5.12.5
set COMPILER_BIN=mingw73_64
set COMPILER_BUILD=MinGW_64_bit

Rem paths
set PATH_TO_QT_BIN=C:\Qt\%QT_VERSION_DOTTED%\%COMPILER_BIN%\bin
set PATH_TO_RELEASE=..\build-autonomx-Desktop_Qt_%QT_VERSION_UNDERSCORE%_%COMPILER_BUILD%-Release\autonomx\release
set PATH_TO_QML_DIR=.\autonomx

Rem deploy here
%PATH_TO_QT_BIN%\windeployqt %PATH_TO_RELEASE%\autonomx.exe --qmldir %PATH_TO_QML_DIR% -verbose=3

Rem remove all .cpp and .o files at root (they're literally unnecessary once the project is built, makes it less of a chore to package the app hehe)
echo Deleting .cpp files...
del %PATH_TO_RELEASE%\*.cpp \q
echo Deleting .h files...
del %PATH_TO_RELEASE%\*.h \q
echo Deleting .o files...
del %PATH_TO_RELEASE%\*.o \q
echo Done!
