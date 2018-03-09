DataGridLayout.qml has the main control, it depends on a couple other files.

<img width="642" alt="DataGridLayout" src="https://raw.githubusercontent.com/series6147/QmlDataGridLayout/master/DataGridLayout/Images/2017_08_20_10_37_23_DataGridLayout.png?_sm_au_=iFVFD40DZD2rvn86">

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
