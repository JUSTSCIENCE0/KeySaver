import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: rectangle
    width: Window.width
    height: Window.height
    opacity: 1

    Row {
        id: cancelConfirm
        height: 40
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        StyledButton {
            id: cancelSetup
            text: qsTr("Cancel")
            backgroundColor: "white"
            borderColor: "#e0e0e0"
            textColor: "black"

            anchors.left: parent.left
            anchors.leftMargin: 0
            width: parent.width / 2 - 4

            onClicked: loader.source = "GetPassword.qml"
        }

        StyledButton {
            id: confirmSetup
            text: qsTr("Confirm")

            anchors.right: parent.right
            anchors.rightMargin: 0
            width: parent.width / 2 - 4
        }
    }
}
