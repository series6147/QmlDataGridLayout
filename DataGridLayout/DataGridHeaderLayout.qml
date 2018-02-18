import QtQuick 2.0
import QtQuick.Layouts 1.3

GridLayout {
    function updateLayout() {
        var count = children.length;
        var h = 0, w = 0;
        var rowHeights = [];
        for (var i = 0; i < count; i++)
        {
            var child = children[i];
            var row = child.itemRow;
            var rowSpan = child.itemRowSpan;
            var avgHeight = child.childrenRect.height / rowSpan;
            for (var j = 0; j < rowSpan; j++)
            {
                if (rowHeights[row + j] === undefined)
                {
                    rowHeights[row + j] = 0;
                }
                rowHeights[row + j] = Math.max(rowHeights[row + j], avgHeight);
                w = Math.max(w, child.childrenRect.width);
            }
        }
        count = rowHeights.length;
        h = 0;
        for (i = 0; i < count; i++)
        {
            h += rowHeights[i];
        }
        if (height !== h)
            height = h;
        if (width !== w)
            width = w;
    }
    columnSpacing: 0
    columns: 1
    rowSpacing: 0
}
