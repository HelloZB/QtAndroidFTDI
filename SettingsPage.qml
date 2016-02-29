import QtQuick 2.4
import QtQuick.Window 2.2

Item {
    Column{
        anchors.fill: parent
        anchors.margins: defMargins

        spacing: defMargins

        Row{
            height: defHeightPushButton
            spacing: defMargins
            Text{ text: qsTrId("Scale"); color: defColorText; font.pixelSize: defHeightFontPixel; height: defHeightPushButton }

            CustomPushButton{
                height: defHeightPushButton
                width: height;
                imgSource: "qrc:/img/katynko/fontsizedown_koffice.png";

                text: "";
                fillColor: defColorButtonFill;
                color: defColorText
                radius: defRadiusButton
                textSize: defHeightFontPixel
                enabled: (defHeightFontPixel >  Screen.logicalPixelDensity)
                onAnimationDone: {
                    defHeightFontPixel = defHeightFontPixel * 0.95
                    console.log("minus = " + defHeightFontPixel)
                    ftdiManager.saveQmlSettRealVal(defHeightFontPixel,0)
                }
            }

            CustomPushButton{
                height: defHeightPushButton
                width: height;
                imgSource: "qrc:/img/katynko/fontsizeup_koffice.png";

                text: "";
                fillColor: defColorButtonFill;
                color: defColorText
                radius: defRadiusButton
                textSize: defHeightFontPixel

                enabled: Screen.width < Screen.height ?
                             width < (Screen.width * 0.25) :
                             height < (Screen.height * 0.25)
                onAnimationDone: {
                    defHeightFontPixel = defHeightFontPixel * 1.05
                    console.log("plus = " + defHeightFontPixel)
                    ftdiManager.saveQmlSettRealVal(defHeightFontPixel,0)


                }
            }

            CustomPushButton{
                height: defHeightPushButton
                width: height;
                imgSource: "qrc:/img/katynko/fontsizedefault_koffice.png";

                text: "";
                fillColor: defColorButtonFill;
                color: defColorText
                radius: defRadiusButton
                textSize: defHeightFontPixel

                onAnimationDone: {
                    defHeightFontPixel =  Screen.pixelDensity * 3.8;
                    ftdiManager.saveQmlSettRealVal(defHeightFontPixel,0)

                }
            }
        }


    }
}

