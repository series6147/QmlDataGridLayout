import QtQuick 2.0

Text {
    color: (dataGrid === null ? false : dataGrid.focus) && isSelected ? "#fff" : "#000"
    text: modelData === undefined ? "" : modelData
}
