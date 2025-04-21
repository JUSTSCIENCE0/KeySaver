import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ImageButton {
    source: "qrc:/UI/pictures/ic_help.png"

    property alias text: messageDialog.text

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.rightMargin: 16
    anchors.topMargin: 16

    MessageDialog {
        id: messageDialog
        title: qsTr("info")
    }

    onButtonClick: function() {
        messageDialog.open()
    }
}
