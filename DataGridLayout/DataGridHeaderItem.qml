import com.mapal.unibase 1.0
import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridHeaderItemPresenter {
    id: layoutRoot
    height: visible && itemRoot.height > 0 ? itemRoot.height + 10 : 0
    objectName: "__DATAGRIDHEADERITEM__"
    visible: itemVisible
    width: itemWidth
    x: itemX
    z: 100

    MouseArea {
        anchors.fill: parent
        onPositionChanged: {
            layoutRoot.touchMoved(mouse.x, mouse.y);
        }
        onPressed: {
            layoutRoot.touchStarted(mouse.x, mouse.y);
        }
        onReleased: {
            layoutRoot.touchReleased();
        }
    }

    Rectangle {
        anchors.left: layoutRoot.left
        anchors.margins: 5
        anchors.right: layoutRoot.right
        anchors.top: layoutRoot.top
        clip: true
        color: layoutRoot.color
        id: itemRoot
        implicitHeight: childrenRect.height

        Loader {
            property var column: layoutRoot.column
            property var item: layoutRoot
            property var model: layoutRoot.model
            property var modelData: layoutRoot.modelData
            onImplicitWidthChanged: {
                layoutRoot.contentWidthChanged(implicitWidth + 10 + imageSort.implicitWidth + resize.implicitWidth);
            }
            sourceComponent: column == null
                             ? defaultDelegate
                             : (column.headerDelegate == null
                                ? dataGrid.defaultHeaderDelegate == null ? defaultDelegate : dataGrid.defaultHeaderDelegate
                                : column.headerDelegate)
        }

        //TODO: FS Use TouchIcon!!!!
        Image {
            anchors.right: resize.left
            id: imageSort
            source: "qrc:/Images/selectArrows.svg"
            visible: layoutRoot != null && layoutRoot.column != null && layoutRoot.dataGrid != null && layoutRoot.dataGrid.sortEnabled && layoutRoot.column.sortEnabled
            sourceSize: "14 x 14"

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
            color: layoutRoot != null && layoutRoot.column != null && layoutRoot.column.sortActive ? "#ff999999" : "#ffdddddd"
            source: imageSort
        }

        Rectangle {
            anchors.bottom: itemRoot.bottom
            anchors.right: itemRoot.right
            anchors.top: itemRoot.top
            color: layoutRoot.color
            id: resize
            implicitWidth: visible ? 10 : 0
            visible: column != null && column.sizeMode === DataGridColumn.FixedSize && typeof column.width === "number"

            Rectangle {
                anchors.bottom: resize.bottom
                anchors.right: resize.right
                anchors.top: resize.top
                color: "#ccc"
                width: 1
            }

            MouseArea {
                anchors.fill: resize
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
