import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridRowItemPresenter {
    id: layoutRoot
    implicitHeight: childrenRect.height > 0 ? childrenRect.height + 10 : 0
    implicitWidth: itemWidth
    Layout.alignment: Qt.AlignVCenter
    objectName: "__DATAGRIDROWITEM__"
    visible: itemVisible
    x: itemX

    Item {
        anchors.left: layoutRoot.left
        anchors.margins: itemLoader != null && itemLoader.height > 0 ? 5 : 0
        anchors.right: layoutRoot.right
        anchors.top: layoutRoot.top
        clip: true
        id: itemRoot
        implicitHeight: itemLoader == null ? 0 : itemLoader.height
        objectName: "__DATAGRIDROWITEMLAYOUT__"

        Loader {
            anchors.centerIn: layoutRoot == null || layoutRoot.column == null
                              ? undefined
                              : layoutRoot.column.itemAlignment === Qt.AlignHCenter
                                ? itemRoot
                                : undefined
            anchors.left: layoutRoot == null || layoutRoot.column == null
                          ? undefined
                          : layoutRoot.column.itemAlignment === Qt.AlignJustify
                            ? itemRoot.left
                            : undefined
            anchors.right: layoutRoot == null || layoutRoot.column == null
                           ? undefined
                           : layoutRoot.column.itemAlignment === Qt.AlignRight || layoutRoot.column.itemAlignment === Qt.AlignJustify
                             ? itemRoot.right
                             : undefined
            function sendEvent(eventName, value) {
                layoutRoot.sendEvent(eventName, value);
            }
            function setModelData(value) {
                layoutRoot.setModelData(value);
            }
            id: itemLoader
            onImplicitWidthChanged: {
                layoutRoot.contentWidthChanged(implicitWidth + 10);
            }
            property bool isSelected: layoutRoot.isSelected
            property int rowIndex: layoutRoot.rowIndex
            property var column: layoutRoot.column
            property var dataGrid: layoutRoot.dataGrid
            property var item: layoutRoot
            property var model: layoutRoot.model
            property var modelData: layoutRoot.modelData
            sourceComponent: column === null
                             ? null
                             : (column.itemDelegate === null
                                ? dataGrid.defaultItemDelegate === null ? defaultDelegate : dataGrid.defaultItemDelegate
                                : column.itemDelegate)
        }
    }

    Component {
        id: defaultDelegate

        DataGridRowItemDelegateDefault {
        }
    }
}
