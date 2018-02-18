import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridRowItemPresenter {
    id: layoutRoot
    implicitHeight: childrenRect.height > 0 ? childrenRect.height + 10 : 0
    implicitWidth: itemWidth
    objectName: "__DATAGRIDROWITEM__"
    visible: itemVisible
    x: itemX

    Item {
        anchors.left: parent.left
        anchors.margins: childrenRect.height > 0 ? 5 : 0
        anchors.right: parent.right
        anchors.top: parent.top
        clip: true
        implicitHeight: childrenRect.height
        objectName: "__DATAGRIDROWITEMLAYOUT__"

        Loader {
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
            onImplicitWidthChanged: {
                layoutRoot.contentWidthChanged(implicitWidth + 10);
            }
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
