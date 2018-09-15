import QtQuick 2.0

Item {
    height: flickable.height
    id: scrollbars
    property Flickable flickable
    property real indicatorSize: 10
    width: flickable.width
    x: flickable.x
    y: flickable.y

    Rectangle { // viewport indicator x
        color: unibaseStyle.colorScrollbar
        height: indicatorSize
        opacity: flickable.movingHorizontally ? 0.7 : 0
        visible: opacity > 0
        width: flickable.visibleArea.widthRatio * flickable.width
        x: flickable.visibleArea.xPosition * flickable.width
        y: flickable.height - height

        Behavior on opacity {
            enabled: uniBase.noAnimations;

            NumberAnimation { duration: 200}
        }
    }

    Rectangle { // viewport indicator y
        color: unibaseStyle.colorScrollbar
        height: flickable.visibleArea.heightRatio * flickable.height
        opacity: flickable.movingVertically ? 0.7 : 0
        visible: opacity > 0
        width: indicatorSize
        x: flickable.width - width
        y: flickable.visibleArea.yPosition * flickable.height

        Behavior on opacity {
            enabled: uniBase.noAnimations;

            NumberAnimation { duration: 200 }
        }
    }
}
