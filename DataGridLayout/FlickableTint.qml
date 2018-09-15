import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: flickableTint
    property color tintingColor: "white"
    property real tintingSize: 100
    property Flickable flickable
    height: flickable.height
    width: flickable.width
    x: flickable.x
    y: flickable.y

    Item {
        anchors.fill: parent
        id: horizontalTinting
        property real contentLeftOfView: flickable.contentX
        property real contentRightOfView: flickable.contentWidth - flickable.contentX - flickable.width
        visible: flickable.contentWidth > flickable.width

        Item {
            id: tLeft
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            opacity: parent.contentLeftOfView / tintingSize
            width: tintingSize

            Image {
                anchors.fill: parent
                id: gradient
                source: "../../images/gradient.png"
                visible: false
            }

            Rectangle {
                anchors.fill: parent
                color: flickableTint.tintingColor
                id: color
                visible: false
            }

            OpacityMask {
                anchors.fill: parent
                maskSource: gradient
                source: color
            }
        }

        Item {
            id: tRight
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: parent.top
            opacity: parent.contentRightOfView / tintingSize
            width: tintingSize

            Image {
                anchors.fill: parent
                id: gradientRight
                source: "../../images/gradient.png"
                visible: false
            }

            Rectangle {
                anchors.fill: parent
                color: flickableTint.tintingColor
                id: colorRight
                visible: false
            }

            OpacityMask {
                anchors.fill: parent
                maskSource: gradientRight
                rotation: 180
                source: colorRight
            }
        }
    }

    Item {
        anchors.fill: parent
        id: verticalTinting
        property real contentAboveView : flickable.contentY
        property real contentBelowView : flickable.contentHeight - flickable.contentY - flickable.height
        visible: flickable.contentHeight > flickable.height

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: tintingSize
            id: tTop
            gradient: Gradient {

                GradientStop {
                    color: "#00ffffff";
                    position: 0.00;
                }

                GradientStop {
                    color: flickableTint.tintingColor;
                    position: 1.00;
                }
            }
            opacity: parent.contentAboveView / tintingSize
            rotation: 180
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: tintingSize
            id: tBottom
            gradient: Gradient {

                GradientStop {
                    color: "#00ffffff";
                    position: 0.00;
                }

                GradientStop {
                    color: flickableTint.tintingColor;
                    position: 1.00;
                }
            }
            opacity: parent.contentBelowView / tintingSize
        }
    }

    Component {
        id: tintingItem

        Rectangle {
            height: tintingSize
            gradient: Gradient {

                GradientStop {
                    position: 0.00;
                    color: "#00ffffff";
                }

                GradientStop {
                    position: 1.00;
                    color: flickableTint.tintingColor;
                }
            }
            width: 1
        }
    }
}
