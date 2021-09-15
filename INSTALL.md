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

### macOS app bundle

To create the macOS app bundle, run this script:

```
./scripts/sh_mac_deploy_qt.sh
```

You might need to manually edit these scripts in order to configure the version number of Qt you have installed.

# Coding style

We follow the [Qt style guide](https://wiki.qt.io/Qt_Coding_Style) for C++ and QML.

We also recommend using the [Prettier extension for VS Code](https://marketplace.visualstudio.com/items?itemName=esbenp.prettier-vscode) to quickly and automatically format large C++ files simply by **pressing Shift+Alt+F inside a document** (only works on C++ files; QML is not supported). This extension, however, will enforce its own C++ formatting style out-of-the-box, which diverges from our aforementioned preferred scheme, so here are the steps to correct it:

1. Go to `File` > `Preferences` > `Settings`
2. In the search bar, input `C_Cpp.clang_format_fallbackStyle`
3. In the appropriate setting field, replace `Visual Studio` with `{ BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 0 }`
4. Close and reopen VS Code

## Version number

Each time we do a release/tag, we update the version number.
It's located in autonomx/main.qml.
We follow Semantic Versioning 2.0.0 - See https://semver.org/ 

# Documentation

## Software Architecture

Comprehensive documentation about the software architecture can be found [here](doc/ARCHITECTURE.md).

## Generator Implementation
Comprehensive documentation about the steps necessary for implementing a new custom generator can be found [here](doc/GENERATOR.md).

## OSC Messaging System

Comprehensive documentation related to the OSC Messaging System can be found 
[here](doc/osc.md).

## Doxygen Configuration

For full class documentation, we use [Doxygen](https://www.doxygen.nl) with [GraphViz](https://graphviz.org)

### Complete explanation

1. Download and install [Doxygen](https://www.doxygen.nl/download.html)
    * Download and install [GraphViz](https://graphviz.org/download/) - required to generate UML diagrams
    * **parameters might need to be modified to specify what needs to be included (!)

2. Once Doxygen and GraphViz are installed, open Doxygen and open autonomx-doxyfile
    * On the left side menu you should see a list of topics. We will go through them in order.

3. Project
    * Specify working directory (where Doxygen will run)
    * Select the directory to scan (AutonomX source folder)
    * Select destination directory (where the folder of documents will be saved)
    * Click next

4. Mode 
    * This page should already be correct
    * “Include cross-referenced source code” 
        * **Unsure if necessary or not**
    * Click next

5. Output
    * This page should already be correct
    * Here you can choose to output different formats; we default to only HTML
    * Click next

6. Diagrams
    * **Unsure what is necessary or not but currently set to generating all options**
    * Click next
    
7. Run Page
    * Before running, click on “Expert” tab
    * Scroll to bottom of left menu and click on “Dot” in Topics
    * Scroll down in right menu / params until you see “DOT_PATH”
    * In DOT_PATH type: /usr/local/bin
        * **This may depend on where your system installed GraphViz**
    * Click on Run tab
    * Click “Run doxygen”
    * This should run and generate a folder with all the documentation
    
8. Outside Doxygen, navigate to the ouput folder, click any html file inside, and it should open the entire documentation in your default browser. Left hand side should have a navigation menu.
    * You did it! :)

### Short instructions for Ubuntu GNU/Linux

On Ubuntu, you can install Doxygen like this:

```
sudo apt install doxygen graphviz
```

Then, you can build the documentation and open the resulting HTML files:

```
doxygen
cd html/
xdg-start index.html
```
