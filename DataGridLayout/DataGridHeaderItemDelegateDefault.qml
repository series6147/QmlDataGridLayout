import QtQuick 2.0

Text {
    font.pointSize: 14
    text: modelData === undefined ? "" : modelData
}
