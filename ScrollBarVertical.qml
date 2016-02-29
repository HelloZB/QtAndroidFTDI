
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1


Rectangle {
    id: scrollBar
    property variant target

    clip: true
    width: defHeightFontPixel * 0.5
    radius: defRadiusButton
    smooth: true

    anchors.top: target.top
    anchors.bottom: target.bottom
    anchors.right: target.right
    anchors.rightMargin: 0

    states: [
        State { name: "nothing"; },
        State { name: "disabled"; when: (track.height == slider.height || target.width <= slider.width || target.contentHeight <= track.height) }
    ]
    transitions: [
        Transition { to: "disabled";
            ParallelAnimation {
                NumberAnimation { target: scrollBar; property: "opacity"; to: 0; duration: 500; }
                PropertyAction { target: scrollBar; property: "visible"; value: false; }
            }
        },
        Transition { to: "*";
            PropertyAction { target: scrollBar; property: "visible"; value: true; }
            NumberAnimation { target: scrollBar; property: "opacity"; to: 0.6; duration: 500; }
        }
    ]

    Timer {
        property int scrollAmount

        id: timer
        repeat: true
        interval: 20
        onTriggered: {
            target.contentY = Math.max(0, Math.min(target.contentY + scrollAmount,
                                          target.contentHeight - target.height));
        }
    }

    Item {
        id: track
        anchors.top: parent.top
        anchors.bottom: parent.bottom;
        width: parent.width

        MouseArea {
            anchors.fill: parent
            onPressed: {
                timer.scrollAmount = target.height * (mouseY < slider.y ? -1 : 1) // scroll by a page
                timer.running = true;
            }
            onReleased: {
                timer.running = false;
            }
        }

        Rectangle {
            id:slider
            color: defColorButtonFill
            smooth: true
            width: parent.width
            radius: 6

            anchors.bottom: (target.visibleArea.yPosition > 1)? parent.bottom: undefined
            height: {
                if(visible  && target.contentHeight > 0 && target.height > 0 && target.visibleArea.yPosition > 0){
                    if (target.visibleArea.yPosition < 0){         // Oberer Rand
                        Math.max(30, Math.min(target.height / target.contentHeight * track.height, track.height-y) +target.height * target.visibleArea.yPosition)

                    }else  {                                      // Mittelbereich
                        Math.min(target.height / target.contentHeight * track.height, track.height-y)

                    }
                }
            }

            y: Math.max(0,Math.min(track.height-30, target.visibleArea.yPosition * track.height));


            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.YAxis
                drag.minimumY: 0
                drag.maximumY: track.height - height
                hoverEnabled: true
                onPositionChanged: {
                    if (pressedButtons == Qt.LeftButton) {
                        target.contentY = slider.y * target.contentHeight / track.height
                    }
                }
                onPressed: {
                    scrollBar.opacity = 1
                }

                onEntered:   {
                    scrollBar.opacity = 1
                }

                onReleased: {
                    scrollBar.opacity = 0.6
                }

                onExited: {
                    if(pressedButtons == Qt.LeftButton)
                        scrollBar.opacity = 1
                    else
                     scrollBar.opacity = 0.6
                }
                onWheel: {
                    if(slider.y + slider.height +20 >= track.height){
                       // flickTextLog.ensureVisible(textLog.cursorRectangle)
                        console.log("down1")
                    }
                }
            }
        }
    }

}
