import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

DataGrid {
    clip: true
    id: layoutRoot
    objectName: "__DATAGRID__"
    onScrollIncrementally: {
        flickable.flick(xVelocity, yVelocity);
    }

    Rectangle {
        anchors.fill: parent
        border.color: borderColor
        border.width: 1
        visible: showBorder
    }

    DataGridHeaderPresenter {
        enabled: !isReadOnly
        implicitHeight: headerLayout.height
        implicitWidth: Math.max(Math.max(layoutRoot.width, childrenRect.width), layoutRoot.layoutWidth)
        id: header
        objectName: "__DATAGRIDHEADER__"
        x: -flickable.contentX

        DataGridHeaderLayout {
            id: headerLayout
            objectName: "__DATAGRIDHEADERROWLAYOUT__"
        }
    }

    Flickable {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        clip: true
        contentWidth: grid.childrenRect.width
        id: flickable
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

    Item {
        anchors.fill: parent
        objectName: "__DATAGRIDDRAGLAYOUT__"
        opacity: .5
    }

    FlickableTint {
        flickable: flickable
    }

    ScrollBars {
        flickable: flickable
    }
}
