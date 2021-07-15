import QtQuick 2.11

Item {
    // app state
    Shortcut {
        sequence: "Esc"
        onActivated: toggleFullscreen()
    }
    Shortcut {
        sequences: [StandardKey.Quit, "Ctrl+Q"]
        onActivated: quitThisApp()
    }

    // navigation
    Shortcut {
        sequence: "PgUp"
        onActivated: {
            if (activeGeneratorIndex < 0) return;
            var i = activeGeneratorIndex - 1;
            if (i < 0) i++;
            activeGeneratorIndex = i;
        }
    }
    Shortcut {
        sequence: "PgDown"
        onActivated: {
            var i = activeGeneratorIndex + 1;
            if (i >= generatorModel.rowCount()) i--;
            activeGeneratorIndex = i;
        }
    }

    // zoom
    Shortcut {
        sequence: "Ctrl+="
        onActivated: {
            latticeView.zoomIn();
        }
    }

    Shortcut {
        sequence: "Ctrl+-"
        onActivated: {
            latticeView.zoomOut();
        }
    }

    // lattice view
    Shortcut {
        sequence: "Alt+I"
        onActivated: latticeView.inputsVisible = !latticeView.inputsVisible
    }
    Shortcut {
        sequence: "Alt+O"
        onActivated: latticeView.outputsVisible = !latticeView.outputsVisible
    }

    Shortcut {
        sequence: "Shift+I"
        onActivated: latticeView.addNewInput()
    }
    Shortcut {
        sequence: "Shift+O"
        onActivated: latticeView.addNewOutput()
    }

    // serialization
    Shortcut {
        id: saveSC
        sequences: [StandardKey.Save]

        onActivated: saveManager.save()
    }
    Shortcut {
        id: saveAsSC
        sequences: [StandardKey.SaveAs, "Ctrl+Shift+S"]

        onActivated: saveManager.saveAs()
    }
    Shortcut {
        id: loadSC
        sequences: [StandardKey.Open, "Ctrl+L"]

        onActivated: saveManager.load()
    }
    Shortcut {
        id: newSC
        sequences: [StandardKey.New]

        onActivated: console.log("new")
    }
    Shortcut {
        id: undoSC
        sequences: [StandardKey.Undo]

        onActivated: console.log("undo");
    }
    Shortcut {
        id: redoSC
        sequences: [StandardKey.Redo, "Ctrl+Shift+Z"]

        onActivated: console.log("redo");
    }
}
