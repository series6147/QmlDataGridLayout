import com.mapal.unibase 1.0
import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQml 2.2

Window {
    height: 480
    title: qsTr("Hello World")
    visible: true
    width: 640

    DataGridLayout {
        anchors.fill: parent
        itemHeight: 50
        objectName: "datagrid"
        selectionMode: DataGrid.SingleRow

        DataGridColumn {
            column: 0
            columnSpan: 3
            minWidth: 100
            role: "Name"
            sizeMode: DataGridColumn.FixedSize
            width: 310
        }

        DataGridColumn {
            column: 1
            columnSpan: 2
            itemAlignment: Qt.AlignLeft
            itemDelegate: numberDelegate
            role: "Price"
            row: 1
            sizeMode: DataGridColumn.FixedSize
            width: 160
        }

        DataGridColumn {
            column: 3
            itemAlignment: Qt.AlignJustify
            itemDelegate: numberDelegate
            minWidth: 200
            role: "Cost"
            row: 1
            sizeMode: DataGridColumn.RemainingSize
        }

        Component {
            id: numberDelegate

            TextField {
                text: modelData
                onEditingFinished: {
                    setModelData(Number(text));
                }
                validator: IntValidator { }
            }
        }
    }
}
