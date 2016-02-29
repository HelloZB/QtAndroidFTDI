import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("QtAndroidFTDI Terminal")
    width: 640
    height: 480
    visible: true
    property bool ftdiIsConnected: false

    signal addToLog(string mess)
    signal connStateChanged(bool isConnected)

    signal connectPageSettt(int dataIndx, int stopIndx, int parityIndx, int flowIndx, int baudIndx);

    signal terminalPageSettt(int endLineIndx, bool hexOut);

    signal settPage(real defH);

    signal setCurrntPortIndx(int portIndx);

    signal sendLineHexIsValid(bool isValid);


    onSetCurrntPortIndx: {
        cpPage.setCurrntPortIndx(portIndx);
    }


    onConnectPageSettt: {
        cpPage.connectPageSettt(dataIndx, stopIndx, parityIndx, flowIndx, baudIndx);
    }

    onTerminalPageSettt: {
        tpPage.terminalPageSettt(endLineIndx, hexOut);
    }

    onSettPage: {
        if(defH > Screen.logicalPixelDensity && (Screen.width < Screen.height ?
                    width < (Screen.width * 0.25) :
                    height < (Screen.height * 0.25)))
            defHeightFontPixel = defH
    }

    onSendLineHexIsValid: {
        tpPage.sendLineHexIsValid(isValid);
    }

    property real defHeightFontPixel: Screen.pixelDensity  * 3.8;
    property real defHeightPushButton: defHeightFontPixel * 1.6
    property real defHeightPushButtonSmll: defHeightFontPixel * 1.4
    property real defHeightFontPixelLog: defHeightFontPixel * 0.6;
    property real defHeightLine: defHeightFontPixel * 0.1


    property string defColorBackground: "#efefef"
    property string defColorTop: "#aaaaaa"
    property string defColorText: "black"
    property string defColorButtonFill: "#0d670d"
    property string defColorButtonText: defColorBackground
    property string defColorLine: "#gray"

    property int defRadiusButton: 4;

    property real defMargins: defHeightFontPixel * 0.2


    onConnStateChanged: {
        ftdiIsConnected = isConnected
        if(isConnected){
           changeStackViewPage(1)
        }
    }

    onAddToLog: {
        tpPage.appendPlainText(mess)
    }


    function changeStackViewPage(indx){
        switch(indx){
        case 0: stackView.push({item: cpPage, visible: true}); break;
        case 1: stackView.push({item: tpPage, visible: true}); break;
        case 2: stackView.push({item: spPage, visible: true});break;
        case 3: stackView.push({item: hpPage, visible: true});break;

        default: break;
        }
    }

    Rectangle {
        id: root
        anchors.fill: parent
        color: defColorBackground


        Row{
            id: rtTop

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: defHeightPushButton

            anchors.margins: defMargins

            spacing: defMargins

            CustomPushButton{
                id: cpbConnectPage
                height: parent.height
                text: qsTr("Device")

                width: (parent.width / 3 ) - parent.spacing

                radius: defRadiusButton
                color: defColorButtonText
                fillColor: defColorButtonFill
                textSize: defHeightFontPixel
                onAnimationDone: {
                    changeStackViewPage(0);
                }
                enabled: false
            }

            CustomPushButton{
                id: cpbTerminalPage
                height: parent.height
                text: qsTr("Terminal")
                width: cpbConnectPage.width
                radius: defRadiusButton
                color: defColorButtonText
                fillColor: defColorButtonFill
                textSize: defHeightFontPixel
                onAnimationDone: {
                    changeStackViewPage(1)
                }
                enabled: true
            }

            Item{
                height: parent.height
                width: parent.width - cpbConnectPage.width * 2 - cpbSettingsPage.width  - 3 * parent.spacing - 2 * defMargins
            }

            CustomPushButton{
                id: cpbSettingsPage
                height: parent.height
                text: ""
                imgSource: "qrc:/img/katynko/settings.png"
                radius: defRadiusButton
                color: defColorButtonText
                fillColor: defColorButtonFill
                textSize: defHeightFontPixel
                width: height//cpbConnectPage.width
                //                    onIAmReleased: videoS.refreshDevList();
                onAnimationDone: {
                    changeStackViewPage(2)
                }
                enabled: true
            }

        }


        StackView{
            id: stackView
            anchors.left: parent.left
            anchors.top: rtTop.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            visible: true
            anchors.margins: defMargins
            initialItem: cpPage;

            focus: true
            Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                                 stackView.pop();
                                 event.accepted = true;
                             }

            onCurrentItemChanged: {
                cpbSettingsPage.enabled = !(currentItem === spPage);
                cpbTerminalPage.enabled = !((currentItem === tpPage) || (currentItem === hpPage));
                cpbConnectPage.enabled = !(currentItem === cpPage);


            }

        }

        ConnectPage{
            id: cpPage
            visible: true
        }

        TerminalPage{
            id: tpPage
            visible: false
        }

        SettingsPage{
            id: spPage
            visible: false
        }

        HistoryPage{
            id: hpPage
            visible: false
        }

    }
}
