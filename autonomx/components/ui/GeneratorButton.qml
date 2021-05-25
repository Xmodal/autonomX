import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/stylesheet"

Control {
    id: generatorButton

    // props
    property color backgroundColor
    property string iconSource
    property string text
    property alias indicatorState: indicator.state

    signal clicked()

    font: Stylesheet.fonts.label
    clip: true

    implicitWidth: 100
    implicitHeight: 40

    enabled: state !== "hidden"

    Rectangle {
        anchors.fill: parent
        color: Stylesheet.colors.black

        Rectangle {
            id: bg
            anchors.fill: parent
            color: backgroundColor
            opacity: mouseArea.containsMouse && !mouseArea.pressed ? 1 : 0.5
        }
    }

    Text {
        id: contentItem
        leftPadding: 15
        anchors.fill: parent
        text: generatorButton.text
        font: generatorButton.font
        color: Stylesheet.colors.white
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Canvas {
        id: indicator
        width: 15
        height: 15

        property real stickHeight: 15
        property real groundBend: 0

        onStickHeightChanged: requestPaint()
        onGroundBendChanged: requestPaint()

        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter

        transform: [
            Translate {
                y: indicator.state == "chevron" ? -2: 0

                Behavior on y {
                    NumberAnimation {
                        duration: 500
                        easing.type: Easing.OutQuad
                    }
                }
            },

            Rotation {
                origin {
                    x: 7.5
                    y: 7.5
                }

                angle: indicator.state == "chevron" ? 0 : -90

                Behavior on angle {
                    NumberAnimation {
                        duration: 500
                        easing.type: Easing.OutQuad
                    }
                }
            }
        ]

        renderStrategy: Canvas.Cooperative

        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            ctx.strokeStyle = Stylesheet.colors.white

            // vertical line
            ctx.moveTo(7.5, 15 - stickHeight)
            ctx.lineTo(7.5, 15)
            ctx.stroke()

            // horizontal line
            ctx.beginPath()
            ctx.moveTo(0, 7.5)
            ctx.lineTo(7.5, 7.5 + (groundBend / 2))
            ctx.lineTo(15, 7.5)
            ctx.stroke()
        }

        states: [
            State { name: "plus" },
            State { name: "chevron" }
        ]

        transitions: [
            Transition {
                from: "plus"
                to: "chevron"

                SequentialAnimation {
                    NumberAnimation {
                        target: indicator
                        property: "stickHeight"
                        to: 7.5
                        duration: 150
                        easing.type: Easing.InQuad
                    }

                    ParallelAnimation {
                        NumberAnimation {
                            target: indicator
                            property: "stickHeight"
                            to: 0
                            duration: 350
                            easing.type: Easing.OutCubic
                        }

                        NumberAnimation {
                            target: indicator
                            property: "groundBend"
                            to: 15
                            duration: 350
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            },
            Transition {
                from: "chevron"
                to: "plus"

                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            target: indicator
                            property: "stickHeight"
                            to: 7.5
                            duration: 150
                            easing.type: Easing.InQuad
                        }

                        NumberAnimation {
                            target: indicator
                            property: "groundBend"
                            to: 0
                            duration: 150
                            easing.type: Easing.InQuad
                        }
                    }

                    NumberAnimation {
                        target: indicator
                        property: "stickHeight"
                        to: 15
                        duration: 350
                        easing.type: Easing.OutCubic
                    }
                }
            }

        ]
    }

    states: State {
        name: "hidden"

        PropertyChanges { target: indicator; opacity: 0 }
        PropertyChanges { target: contentItem; opacity: 0 }
        PropertyChanges { target: generatorButton; implicitWidth: 0 }
    }

    transitions: Transition {
        NumberAnimation {
            target: generatorButton
            property: "implicitWidth"
            duration: 500
            easing.type: Easing.OutCubic
        }

        NumberAnimation {
            targets: [indicator, contentItem]
            property: "opacity"
            duration: 150
            easing.type: Easing.Linear
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onReleased: if (containsMouse) generatorButton.clicked()
    }
}
