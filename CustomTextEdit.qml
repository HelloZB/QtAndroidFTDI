import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.1

Rectangle{
    id: root
    property bool firstValBool: false
    signal customTextChanged
    signal cteEnter()
    signal cteEsc()
    property bool _readOnly: false

    property string  placeholderText: qsTr("Type Here")
    property string imgSource: "qrc:/img/katynko/dashed_elmnt.png"
    property string imgSourceTextNotValid: "qrc:/img/katynko/dashed_elmnt_red.png"

    property string text: textField1.text
    property int cursorPosition: 0
    property int length: textField1.length
    property bool isValid: true
    property real scaleCoef: 1



    onEnabledChanged:  enabled ? opacity = 1 : opacity = 0.4


    function setTextIsValid(_isValid){
        isValid = _isValid       
    }

    function setText(txt){
        textField1.text = txt
        if(textField1.text.length)
            pbClearTextField1.state = "State0"
        else
            pbClearTextField1.state = "clear"
    }

    function setFocus(isTrue){
        textField1.focus = isTrue
    }

    color: defColorBackground


    height: 22
    width: 50
    x: 5
    y: 5

    TextField {
        id: textField1
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height * 0.9
        width: _readOnly ?  parent.width : parent.width - height

        placeholderText: root.placeholderText
        text: root.text
        readOnly: _readOnly
        cursorPosition: root.cursorPosition

        onTextChanged:  {
            root.text = text
            if(textField1.text.length > 0 && !_readOnly){
                if(!pbClearTextField1.visible){
                    firstValBool = true
                    pbClearTextField1.state = "State0"
                }
            }else{
                if(pbClearTextField1.visible)
                    pbClearTextField1.state = "clear"
            }
            if(!_readOnly)
                root.customTextChanged()
        }

        Keys.onEscapePressed: cteEsc()

        Keys.onReturnPressed: cteEnter()

        style: customTextEditStyle

    }

    Rectangle{
        id: pbClearTextField1
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height - textField1.height

        width: (!firstValBool) ? 0 : parent.height
        visible: firstValBool
        color: defColorBackground//"transparent"

        MouseArea{
            id: maPbClearTextField1
            anchors.fill: parent

            Image{
                id: image111
                anchors.fill: parent
                source: "qrc:/img/katynko/clear_left.png";
            }

            onClicked: {
                Qt.inputMethod.hide()

                textField1.text = "";
                textField1.cursorPosition = 0
                textField1.focus = true

            }
        }
        states: State {
            name: "clear"
            PropertyChanges { target: pbClearTextField1; visible: false; width: 0 }
        }

        transitions: Transition {
            NumberAnimation { properties: "width"; duration: 150; easing.type: Easing.Linear }

        }

    }

    Rectangle{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: pbClearTextField1.width
        color: "transparent"
        height: parent.height - textField1.height

        Image{id: imgValid; source: (isValid) ? imgSource : imgSourceTextNotValid; fillMode: Image.Tile; anchors.fill: parent; scale: scaleCoef }

    }

    Component{
        id: customTextEditStyle
        TextFieldStyle{
            textColor: defColorText
//            selectedTextColor: mainSelectedTextColor
//            selectionColor: mainSelectionColor
//            font.pixelSize: mainDefaultFontPixelSize
            background: Rectangle{
                color: defColorBackground
            }
        }
    }
}


