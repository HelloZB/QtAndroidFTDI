import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2


Item {

    property int lastDeleteIndx: -1;
    Item {
        id: itmTop
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: defHeightPushButton * 1.3


        CustomPushButton{
            id: cpbHistory
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: defMargins
            width: defHeightFontPixel * text.length * 0.7
            imgSource: "";

            text: qsTrId("Back")
            radius: defRadiusButton;
            color: defColorButtonText
            fillColor: defColorButtonFill

            onAnimationDone: {
               changeStackViewPage(1);
            }

        }

        CustomTextEdit{
            id: cteFilter
            anchors.left: cpbHistory.right
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: defMargins
            placeholderText: qsTr("Enter command")

            onCteEnter: {
                Qt.inputMethod.hide()

            }

            onCteEsc: {
                Qt.inputMethod.hide()
            }

            onTextChanged: {
                ftdiManager.setThisHistoryFilter(cteFilter.text)
            }


        }

    }

    ListView{
        id: lvHistory
        anchors.left: parent.left
        anchors.top: itmTop.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        delegate: Item{

            signal onPress();
            id: emnt1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: defMargins
            height: defHeightPushButton

            Text{
                id: txtElmnt
                anchors.verticalCenter: parent.verticalCenter
                anchors.bottomMargin: defHeightLine
                anchors.leftMargin: defMargins
                anchors.left: parent.left
                color: defColorText //defColorBackground
                text: value
                elide: Text.ElideMiddle
            }

            Rectangle{
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: defMargins
                anchors.rightMargin: defMargins
                height: defHeightLine
                color: defColorLine
            }

            MouseArea{
                anchors.fill: parent

                onClicked: {
                    changeStackViewPage(1);
                    tpPage.setThisCommandLine(txtElmnt.text)

                }


                onPressAndHold: {
                    lastDeleteIndx = index;
                    messageDialog.show(qsTr("Delete: ") + txtElmnt.text + "?");
                }
            }


        }
        model: historyModel

        Behavior on height {

            NumberAnimation {
                id: animateHeight
                property: "height"
                from: 0.9
                to: 1.0
                loops: Animation.Infinite
                duration: 200
                easing {type: Easing.OutInQuart  }
            }
        }

        states: State {
            name: "ShowBars"
            when: lstUsrView.movingVertically
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }


    ScrollBarVertical {
        target: lvHistory
    }
    MessageDialog {
        id: messageDialog
        title: qsTr("Delete")

        function show(caption) {

            messageDialog.text = caption;
            messageDialog.open();

        }

        standardButtons: StandardButton.Yes |
               StandardButton.No

        onYes: {
            ftdiManager.delThisHistoryIndx(lastDeleteIndx);
            lastDeleteIndx = -1;
        }

        onNo: {
            lastDeleteIndx = -1;
        }
    }

}

