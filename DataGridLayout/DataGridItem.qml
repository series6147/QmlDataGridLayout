import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    property int itemRow: 0
    property int itemRowSpan: 1
    Layout.fillHeight: true
    Layout.preferredHeight: childrenRect.height
    Layout.row: itemRow
    Layout.rowSpan: itemRowSpan
    onChildrenRectChanged: {
        parent.updateLayout();
    }
}
