import com.mapal.unibase 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

DataGridRowPresenter {
    implicitHeight: Math.max(childrenRect.height, dataGrid === null ? 0 : dataGrid.itemHeight)
    implicitWidth: Math.max(childrenRect.width, dataGrid === null ? 0 : dataGrid.layoutWidth)

    DataGridRowLayout {
        id: layout
        objectName: "__DATAGRIDROWLAYOUT__"
    }
}
