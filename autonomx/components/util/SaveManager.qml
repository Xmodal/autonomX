import QtQuick 2.9
import Qt.labs.platform 1.0

Item {
    id: saveManager

    property string currentFileUri
    property string currentFileName: basename(currentFileUri)

    function basename(str) {
        return (str.slice(str.lastIndexOf("/")+1));
    }

    function load() {
        loadDialog.open()
    }

    function save() {
        if (!currentFileUri) saveDialog.open()
        else {
            if (appModel.saveProject(currentFileUri)) {
                // TODO: successful save animation :)
            }
        }
    }

    function saveAs() {
        saveDialog.open()
    }

    // changes lost warning
    MessageDialog {
        text: "You are about to modify the current project. Any unsaved changes will be lost."
        informativeText: "Do you want to continue?"

        buttons: MessageDialog.Ok | MessageDialog.Cancel
    }

    // save dialog
    FileDialog {
        id: saveDialog
        title: "Save project file as..."
        nameFilters: ["autonomX save files (*." + extensionName + ")"]
        defaultSuffix: extensionName
        fileMode: FileDialog.SaveFile

        onAccepted: {
            currentFileUri = currentFile
            appModel.saveProject(currentFileUri)
        }
    }

    // load dialog
    FileDialog {
        id: loadDialog
        title: "Load project file..."
        nameFilters: ["autonomX save files (*." + extensionName + ")"]
        defaultSuffix: extensionName
        fileMode: FileDialog.OpenFile

        onAccepted: {
            currentFileUri = currentFile

            if (appModel.loadProject(currentFileUri)) {
                activeGeneratorIndex = -1;
            }
        }
    }
}
