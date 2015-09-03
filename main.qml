import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    signal addToLog(string mess)

    onAddToLog: {
        textArea1.text = textArea1.text + mess + "\r\n"
    }

    Rectangle {
        id: root
        anchors.fill: parent
        color: "#efefef"

        Rectangle {
            id: rtConnDisconn
            width: 333
            height: 59
            color: "#32c011"
            radius: 4
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 14

            MouseArea {
                id: mouseArea1
                anchors.fill: parent

                Text {
                    id: text1
                    x: 60
                    y: 22
                    text: qsTr("Connect/Disconnect")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Times New Roman"
                    font.pixelSize: 28
                }

                onClicked: {
                    ftdiManager.connectToOnePort();
                }

                onPressAndHold: {
                    ftdiManager.deviceStatus();
                }
            }
        }

        Rectangle {
            id: rtClearLog
            x: 347
            y: 14
            width: 285
            height: 59
            color: "#32c011"
            radius: 4
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.right: parent.right
            anchors.rightMargin: 8

            MouseArea {
                id: mouseArea2
                anchors.fill: parent

                Text {
                    id: text2
                    x: 82
                    y: 20
                    text: qsTr("Clear Log")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 28
                }

                onClicked: {
                    textArea1.text = "";
                }
            }
        }

        TextEdit {
            id: textEdit1
            y: 412
            width: 396
            height: 59
            text: qsTr("+++")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            cursorVisible: false
            font.pixelSize: 28
            font.family: "Arial"
            anchors.left: parent.left
            anchors.leftMargin: 8
        }

        TextArea {
            id: textArea1
            text: "Hello"
            readOnly: true
            textColor: "#000000"
            frameVisible: true
            font.pixelSize: 28
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 85
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 79

            focus: true
            Keys.onReturnPressed: {
                ftdiManager.sendDataToPort(textEdit1.text);
            }
        }

        Rectangle {
            id: rtSendLine
            x: 410
            y: 412
            width: 209
            height: 59
            color: "#32c011"
            radius: 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            anchors.right: parent.right
            anchors.rightMargin: 21
            MouseArea {
                id: mouseArea3
                anchors.fill: parent
                Text {
                    id: text3
                    x: 82
                    y: 20
                    text: qsTr("Send")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 28
                }

                onClicked: {
                    ftdiManager.sendDataToPort(textEdit1.text);
                }

            }
        }




    }
}
