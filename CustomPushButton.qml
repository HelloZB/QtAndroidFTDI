//import QtQuick 2.0

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1


FocusScope {
    id: container
    signal clicked
    signal iAmReleased
    signal animationDone
    signal iAmPressAndHold
    property string imgSource: ""
    property string text: ""
    property string color: "#ffffff"
    property string fillColor: "#00000000"
    property int textSize: height * 0.6
    property string borderColor: "#00000000"
    property int borderWidth: 0
    property int radius: 6
    property int marginss: 0
//    property bool checable: false
    width: 40
    height: 20

    onEnabledChanged: enabled ? opacity = 1 : opacity = 0.4

    Rectangle {
        id: buttonRectangle
        anchors.fill: parent
        color: fillColor
        radius: container.radius
        border.width: container.borderWidth
        border.color: container.borderColor

        Image {
            id: image
            smooth: true

//            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 0.8
            width: (imgSource.length > 0) ? height : 0
            anchors.margins: container.marginss

            anchors.rightMargin: (container.text.length < 1) ? (parent.height - height) / 2 : container.marginss

            source: imgSource

        }

        Item {
            anchors.left: parent.left
            anchors.right: image.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: container.marginss * 2

            Text {
                color: container.color
                anchors.centerIn: parent
                font.pixelSize: container.textSize
                text: container.text
                font.bold: true
            }
        }

        MouseArea {
            id: mouseArea;
            anchors.fill: parent

            onPressed:{
                buttonRectangle.state = "pressed"
                container.clicked()
            }

            onClicked: {
                buttonRectangle.state = "pressed"

                clkTmr.start()
//                container.clicked()
            }
            onReleased: {
                stateTimer.start()
                container.iAmReleased()
            }

            onPressAndHold: {
                buttonRectangle.state = "pressed"
                container.iAmPressAndHold()
                 stateTimer.start()

            }
        }

        states: State {
            name: "pressed"
            PropertyChanges { target: /*image*/buttonRectangle; scale: 0.85 }
        }

        Timer{
            id: clkTmr
            interval: stateTimer.interval
            repeat: false
            onTriggered: container.animationDone()
        }

        Timer {
            id: stateTimer
            interval: 150;
            repeat: false
            onTriggered:{ buttonRectangle.state = 'State0';

            }
        }

        transitions: Transition {
            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad /*InCubic*/} //.InOutQuad }
        }
    }
}
