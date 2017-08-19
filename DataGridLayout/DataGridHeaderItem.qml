import com.mapal.unibase 1.0
import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridHeaderItemPresenter {
    id: layoutRoot
    implicitHeight: childrenRect.height + 10
    implicitWidth: childrenRect.width + 5
    Layout.leftMargin: itemX
    Layout.row: itemRow
    Layout.rowSpan: itemRowSpan
    objectName: "__DATAGRIDHEADERITEM__"
    onItemWidthChanged: {
        implicitWidth = itemWidth;
        rowLayout.implicitWidth = itemWidth - 5;
    }
    visible: itemVisible

    Item {
        clip: true
        id: rowLayout
        implicitHeight: childrenRect.height
        implicitWidth: childrenRect.width
        x: 5
        y: 5

        Item {
            id: layout
            implicitHeight: childrenRect.height
            implicitWidth: childrenRect.width
            objectName: "__DATAGRIDHEADERITEMLAYOUT__"

            Loader {
                property var column: layoutRoot.column
                property var item: layoutRoot
                property var model: layoutRoot.model
                property var modelData: layoutRoot.modelData
                sourceComponent: column === null
                                 ? null
                                 : column.headerDelegate === null ? defaultDelegate : column.headerDelegate
            }
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
            anchors.fill: imageSort
            source: imageSort
            color: layoutRoot.column.sortActive ? "#ff999999" : "#ffdddddd"
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: parent.top
            color: layoutRoot.color
            id: resize
            implicitWidth: 10
            visible: column.sizeMode === DataGridColumn.FixedSize && typeof column.width === "number"

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
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
