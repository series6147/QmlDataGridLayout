import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

DataGrid {
    clip: true
    id: layoutRoot
    objectName: "__DATAGRID__"

    DataGridHeaderPresenter {
        enabled: !isReadOnly
        implicitHeight: childrenRect.height
        implicitWidth: Math.max(Math.max(layoutRoot.width, childrenRect.width), layoutRoot.layoutWidth)
        id: header
        objectName: "__DATAGRIDHEADER__"
        x: -scrollBar.contentX

        DataGridHeaderLayout {
            id: layout
            objectName: "__DATAGRIDHEADERROWLAYOUT__"
        }
    }

    ScrollBarPage {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        clip: true
        contentWidth: grid.childrenRect.width
        id: scrollBar
        objectName: "__DATAGRIDSCROLLBAR__"
        onContentYChanged: {
            layoutRoot.updateVisibleRange();
        }

        Item {
            enabled: !isReadOnly
            id: grid
            objectName: "__DATAGRIDLAYOUT__"
        }
    }
}
