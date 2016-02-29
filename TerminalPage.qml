import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2

Item {


    signal appendPlainText(string plainTxt);
    signal terminalPageSettt(int endLineIndx, bool _hexOut);
    signal setThisCommandLine(string line)
    signal sendLineHexIsValid(bool isValid);

    property bool hexOut: false



    onAppendPlainText: {
        textArea1.append(plainTxt /*+ "\r\n"*/);
    }

    onTerminalPageSettt: {
        cmbxEndSymb.currentIndex = endLineIndx
        hexOut = _hexOut
        if(hexOut)
            ftdiManager.sendLineHexValidator(cteSend.text)
    }

    onSetThisCommandLine: {
        cteSend.setText(line)
    }

    onSendLineHexIsValid: {
        if(hexOut){
            cteSend.setTextIsValid(isValid)
        }
    }

    function sendLineNow(){
        if(hexOut && !cteSend.isValid)
            return;

        ftdiManager.sendDataToPort(cteSend.text , hexOut, cmbxEndSymb.currentIndex);
        cteSend.setText("");

    }

    Row{
        id: rwTop
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        spacing: defMargins

        height: defHeightPushButtonSmll

        ComboBox{
            id: cmbxEndSymb
            width: parent.width * 0.27
            height: defHeightPushButtonSmll
            currentIndex: 0
            activeFocusOnPress: true
            style: comboboxStyle
            model:  lstModelDN

            ListModel{
                id: lstModelDN
                ListElement{ value: qsTrId("CR+LF")}
                ListElement{ value: qsTrId("CR")}
                ListElement{ value: qsTrId("LF")}
                ListElement{ value: qsTrId("None")}

            }

            onCurrentIndexChanged: {
                ftdiManager.saveQmlSett(currentIndex, 0)
            }
        }

        CustomPushButton{
            id: cpbHex
            width: parent.width * 0.25
            height: defHeightPushButtonSmll
            radius: defRadiusButton
            color: defColorButtonText
            fillColor: defColorButtonFill
            textSize: defHeightFontPixel
            text: hexOut ? "Hex" : "String"
            onAnimationDone: {
                if(hexOut)
                    cteSend.setTextIsValid(true)
                hexOut = !hexOut
                ftdiManager.saveQmlSett(hexOut,0);

                if(hexOut)
                    ftdiManager.sendLineHexValidator(cteSend.text)
            }
        }

        CustomPushButton{
            id: cpbConnectPage
            height: defHeightPushButtonSmll

            width: text.length * defHeightFontPixel * 0.7
            text: qsTr("Clear")

            radius: defRadiusButton
            color: defColorButtonText
            fillColor: defColorButtonFill
            textSize: defHeightFontPixel
            onAnimationDone: {
                textArea1.text = "";
            }
            enabled: true
        }
    }



    TextArea {
        id: textArea1
        text: ""
        readOnly: true
        textColor: defColorText
        frameVisible: true
        font.pixelSize: defHeightFontPixelLog
        anchors.bottom: itmBottom.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: rwTop.bottom
        anchors.margins: defMargins

    }

    Item {
        id: itmBottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: defHeightPushButton * 1.3
        CustomPushButton{
            id: cpbSend
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: defMargins
            width: height
            imgSource: "qrc:/img/katynko/key_enter.png";

            text: ""
            radius: defRadiusButton;

            enabled: (ftdiIsConnected && !sendTmr.running && cteSend.isValid)

            color: defColorButtonText
            fillColor: defColorButtonFill

            onAnimationDone: {
                Qt.inputMethod.hide()
                sendTmr.start();
            }

        }

        CustomPushButton{
            id: cpbHistory
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: defMargins
            width: height
            imgSource: "qrc:/img/katynko/history.png";

            text: ""
            radius: defRadiusButton;
            color: defColorButtonText
            fillColor: defColorButtonFill

            onAnimationDone: {
                 changeStackViewPage(3);
            }

        }

        CustomTextEdit{
            id: cteSend
            anchors.left: cpbHistory.right
            anchors.top: parent.top
            anchors.right: cpbSend.left
            anchors.bottom: parent.bottom
            anchors.margins: defMargins
            enabled: ftdiIsConnected

            text: "+++"
            placeholderText: qsTr("Enter command")

//            scaleCoef: defHeightFontPixel / (Screen.pixelDensity )

            onCteEnter: {
                Qt.inputMethod.hide()
                sendTmr.restart();
//               sendLineNow();
            }

            onCteEsc: {
                Qt.inputMethod.hide()
            }

            onTextChanged: {
                if(hexOut)
                    ftdiManager.sendLineHexValidator(cteSend.text)

            }


        }


        Timer{
            id: sendTmr
            repeat: false
            interval: 50
            onTriggered: {
                 sendLineNow();

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

