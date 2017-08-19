import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridRowItemPresenter {
    clip: true
    id: layoutRoot
    implicitHeight: childrenRect.height + 10
    implicitWidth: childrenRect.width
    Layout.leftMargin: itemX
    Layout.row: itemRow
    Layout.rowSpan: itemRowSpan
    objectName: "__DATAGRIDROWITEM__"
    onItemWidthChanged: {
        implicitWidth = itemWidth;
        layout.implicitWidth = itemWidth - 5;
    }
    visible: itemVisible

    Item {
        id: layout
        implicitHeight: childrenRect.height
        implicitWidth: childrenRect.width
        objectName: "__DATAGRIDROWITEMLAYOUT__"
        x: 5
        y: 5

        Loader {
            anchors.bottomMargin: 5
            anchors.centerIn: layoutRoot.column === null
                              ? undefined
                              : layoutRoot.column.itemAlignment === Qt.AlignHCenter
                                ? parent
                                : undefined
            anchors.left: layoutRoot.column === null
                          ? undefined
                          : layoutRoot.column.itemAlignment === Qt.AlignJustify
                            ? parent.left
                            : undefined
            anchors.right: layoutRoot.column === null
                           ? undefined
                           : layoutRoot.column.itemAlignment === Qt.AlignRight || layoutRoot.column.itemAlignment === Qt.AlignJustify
                             ? parent.right
                             : undefined
            anchors.rightMargin: 5
            function sendEvent(eventName, value) {
                layoutRoot.sendEvent(eventName, value);
            }
            function setModelData(value) {
                layoutRoot.setModelData(value);
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
                             : column.itemDelegate === null ? defaultDelegate : column.itemDelegate
        }
    }

    Component {
        id: defaultDelegate

        DataGridRowItemDelegateDefault {
        }
    }
}
