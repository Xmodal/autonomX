import QtQuick 2.9

ListModel {
    // inputs
    ListElement {
        colX: 1
        colY: 3
        colW: 3
        colH: 3
        type: 0
    }
    ListElement {
        colX: 6
        colY: 3
        colW: 3
        colH: 3
        type: 0
    }
    ListElement {
        colX: 11
        colY: 3
        colW: 3
        colH: 3
        type: 0
    }
    ListElement {
        colX: 16
        colY: 3
        colW: 3
        colH: 3
        type: 0
    }

    // outputs
    ListElement {
        colX: 1
        colY: 14
        colW: 3
        colH: 3
        type: 1
    }
    ListElement {
        colX: 6
        colY: 14
        colW: 3
        colH: 3
        type: 1
    }
    ListElement {
        colX: 11
        colY: 14
        colW: 3
        colH: 3
        type: 1
    }
    ListElement {
        colX: 16
        colY: 14
        colW: 3
        colH: 3
        type: 1
    }
}
