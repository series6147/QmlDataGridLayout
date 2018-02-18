import com.mapal.unibase 1.0
import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridHeaderItemPresenter {
    id: layoutRoot
    height: visible ? childrenRect.height + 10 : 0
    objectName: "__DATAGRIDHEADERITEM__"
    visible: itemVisible
    width: itemWidth
    x: itemX
    z: 100

    Item {
        anchors.left: parent.left
        anchors.margins: 5
        anchors.right: parent.right
        anchors.top: parent.top
        clip: true
        implicitHeight: childrenRect.height

        Loader {
            property var column: layoutRoot.column
            property var item: layoutRoot
            property var model: layoutRoot.model
            property var modelData: layoutRoot.modelData
            onImplicitWidthChanged: {
                layoutRoot.contentWidthChanged(implicitWidth + 10 + imageSort.implicitWidth + resize.implicitWidth);
            }
            sourceComponent: column === null
                             ? defaultDelegate
                             : column.headerDelegate === null ? defaultDelegate : column.headerDelegate
        }

        Image {
            anchors.right: resize.left
            id: imageSort
            source: "qrc:/Images/selectArrows.svg"
            sourceSize: "14 x 14"
            visible: layoutRoot.dataGrid.sortEnabled && layoutRoot.column.sortEnabled

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                objectName: "__DATAGRIDHEADERITEMSORTAREA__"
                onPressed: {
                    layoutRoot.sortColumn();
                }
            }
        }

        ColorOverlay {
            anchors.fill: imageSort.visible ? imageSort : null
            color: layoutRoot.column.sortActive ? "#ff999999" : "#ffdddddd"
            source: imageSort
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: parent.top
            color: layoutRoot.color
            id: resize
            implicitWidth: visible ? 10 : 0
            visible: column.sizeMode === DataGridColumn.FixedSize && typeof column.width === "number"

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.top: parent.top
                color: "#ccc"
                width: 1
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SplitHCursor
                objectName: "__DATAGRIDHEADERITEMRESIZEAREA__"
                onPressed: {
                    layoutRoot.startResize();
                }
                onReleased: {
                    layoutRoot.releseResize();
                }
            }
        }
    }

    Component {
        id: defaultDelegate

        DataGridHeaderItemDelegateDefault {
        }
    }
}
