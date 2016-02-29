import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
Item {
    id: root
    signal connectPageSettt(int dataIndx, int stopIndx, int parityIndx, int flowIndx, int baudIndx);
    signal setCurrntPortIndx(int portIndx);

    onConnectPageSettt: {
        cmbxDataBits.currentIndex = dataIndx
        cmbxStopBits.currentIndex = stopIndx
        cmbxParity.currentIndex = parityIndx
        cmbxFlow.currentIndex = flowIndx
        cmbxBaud.currentIndex = baudIndx
    }

    onSetCurrntPortIndx: {
        cmbxCurrentDevice.currentIndex = portIndx
    }

    property int  defHeightPushButtonL: (root.height / 10) > defHeightPushButton ? defHeightPushButton : (root.height / 10)

    Column{
        anchors.fill: parent
        anchors.margins: defMargins
        spacing: defMargins

        Item {
            height: defHeightPushButtonL

        }

        ComboBox{
            id: cmbxDataBits
            height: defHeightPushButtonL
            width: parent.width - 2 * defMargins
            currentIndex: 0
            activeFocusOnPress: true
            style: comboboxStyle
            model:  ListModel{
                ListElement{ value: qsTrId("Data 8")}
                ListElement{ value: qsTrId("Data 7")}
            }
        }

        ComboBox{
            id: cmbxStopBits
            height: defHeightPushButtonL
            width: cmbxDataBits.width
            currentIndex: 0
            activeFocusOnPress: true
            style: comboboxStyle
            model:  ListModel{
                ListElement{ value: qsTrId("Stop 1")}
                ListElement{ value: qsTrId("Stop 2")}
            }
        }

        ComboBox{
            id: cmbxParity
            height: defHeightPushButtonL
            currentIndex: 0
            width: cmbxDataBits.width
            activeFocusOnPress: true
            style: comboboxStyle
            model:  ListModel{
                ListElement{ value: qsTrId("Parity None")}
                ListElement{ value: qsTrId("Parity Odd")}
                ListElement{ value: qsTrId("Parity Even")}
                ListElement{ value: qsTrId("Parity Mark")}
                ListElement{ value: qsTrId("Parity Space")}

            }
        }

        ComboBox{
            id: cmbxFlow
            height: defHeightPushButtonL
            currentIndex: 0
            width: cmbxDataBits.width
            activeFocusOnPress: true
            style: comboboxStyle
            model:  ListModel{
                ListElement{ value: qsTrId("None")}
                ListElement{ value: qsTrId("CTS/RTS")}
                ListElement{ value: qsTrId("DTR/DSR")}
                ListElement{ value: qsTrId("XOFF/XON")}

            }
        }

        ComboBox{
            id: cmbxBaud
            height: defHeightPushButtonL
            currentIndex: 3
            width: cmbxDataBits.width
            activeFocusOnPress: true
            style: comboboxStyle
            model:  ListModel{
                ListElement{ value: qsTrId("1200")}
                ListElement{ value: qsTrId("2400")}
                ListElement{ value: qsTrId("4800")}
                ListElement{ value: qsTrId("9600")}
                ListElement{ value: qsTrId("19200")}
                ListElement{ value: qsTrId("38400")}
                ListElement{ value: qsTrId("57600")}
                ListElement{ value: qsTrId("115200")}

            }
        }

        ComboBox{
            id: cmbxCurrentDevice
            height: defHeightPushButtonL
            currentIndex: 0
            width: cmbxDataBits.width
            activeFocusOnPress: true
            style: comboboxStyle
            model: deviceModel
        }

        CustomPushButton{
            id: cpbConnect
            text: ftdiIsConnected ? qsTr("Disconnect") : qsTr("Connect")
            fillColor: defColorButtonFill;
            color: defColorBackground
            radius: defRadiusButton
            textSize: defHeightFontPixel
            width: cmbxDataBits.width
            height: defHeightPushButtonL * 2

            enabled: (cmbxCurrentDevice.currentIndex >= 0 && cmbxCurrentDevice.currentText.length)

            onAnimationDone: {
                if(ftdiIsConnected)
                    ftdiManager.disconnectFromPort();
                else
                    ftdiManager.connectToOnePort(cmbxCurrentDevice.currentIndex,
                                                 cmbxBaud.currentIndex,
                                                 cmbxDataBits.currentIndex,
                                                 cmbxStopBits.currentIndex,
                                                 cmbxParity.currentIndex,
                                                 cmbxFlow.currentIndex);
            }

        }

        CustomPushButton{
            id: cpbUpdate
            text: qsTr("Update List")
            fillColor: defColorButtonFill;
            color: defColorBackground
            radius: defRadiusButton
            textSize: defHeightFontPixel
            width: cmbxDataBits.width
            height: defHeightPushButtonL

            onAnimationDone: {
                ftdiManager.createDeviceInfoList(true)
            }

        }



    }
    Component{
        id: comboboxStyle
        ComboBoxStyle {
            id: comboBox
            textColor: defColorText
            //            selectedTextColor:
            //            selectionColor:

            background: Rectangle {
                id: rectCategory
                radius: defRadiusButton
                color: defColorButtonFill

                //                Image {
                //                    source: "qrc:/mainform/katynko/ComboImg.png"
                //                    anchors.verticalCenter: parent.verticalCenter
                //                    //                            anchors.bottom: parent.bottom
                //                    anchors.right: parent.right
                //                    //                            anchors.bottomMargin: 5
                //                    anchors.rightMargin: mainLeftMargin
                //                    height: parent.height * 0.8
                //                    width: height
                //                    fillMode: Image.Stretch
                //                }
            }
            label: Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: defHeightFontPixel

                color: defColorBackground
                text: control.currentText
            }
        }
    }

}

