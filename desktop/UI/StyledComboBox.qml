import QtQuick
import QtQuick.Controls

ComboBox {
    id: styledComboBox
    height: 40

    font.pixelSize: 18
    font.family: "Arial"

    background: Rectangle {
        color: "white"
        radius: 10
        border.color: "#e0e0e0"
        border.width: 4
    }
}
