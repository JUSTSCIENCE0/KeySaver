import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: enterMasterPassword
    width: Window.width
    height: Window.height
    opacity: 1

    StyledInput {
        id: masterPassword
        mode: TextInput.Password

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
    }

    StyledButton {
        id: confirmMasterPassword
        text: qsTr("Confirm")
        onClicked: loader.source = "GetPassword.qml"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: masterPassword.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8
    }

    StyledLabel {
        id: enterMasterPasswordLabel
        text: qsTr("Enter master password")

        anchors.left: parent.left
        anchors.bottom: masterPassword.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }
}
