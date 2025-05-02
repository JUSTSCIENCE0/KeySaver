import QtQuick
import QtQuick.Controls

ImageButton {
    id: helpButton
    source: "qrc:/UI/pictures/ic_help.png"

    property string text: ""

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.rightMargin: 16
    anchors.topMargin: 16

    Dialog {
        id: messageDialog
        title: qsTr("info")
        width: 400
        standardButtons: DialogButtonBox.Ok

        Text {
            text: helpButton.text
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }

        onOpened: {
            messageDialog.x = -1 * messageDialog.width
        }
    }

    onButtonClick: function() {
        messageDialog.open()
    }
}
