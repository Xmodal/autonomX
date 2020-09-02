# Developer notes

Here are the developer notes for this project.

## Building

You need Qt 5 to build this project, including QtQuick.

Open the .pro file in Qt Creator. Build it.

Another option is to do:

```
mkdir build
cd build
qmake ..
make
```

# Coding style

We follow the Qt style guide for C++ and QML.


## Version number

Each time we do a release/tag, we update the version number.
It's located in autonomx/main.qml.
We follow Semantic Versioning 2.0.0 - See https://semver.org/ 

