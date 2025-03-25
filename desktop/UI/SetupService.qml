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
        id: cancel_confirm
        height: 40
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        Button {
            id: cancel_setup
            text: qsTr("Cancel")
            anchors.left: parent.left
            anchors.leftMargin: 0
            width: parent.width / 2 - 4
            onClicked: loader.source = "GetPassword.qml"

            background: Rectangle {
                color: "white"
                radius: 10
                border.color: "#e0e0e0"
                border.width: 4
            }

            contentItem: Text {
                id: cancel_text
                font.bold: true
                color: "black"
                text: "Cancel"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                font.weight: Font.Bold
                font.pointSize: 18
                font.family: "Arial"
                font.capitalization: Font.AllUppercase
            }

            wheelEnabled: false
            spacing: 7
            display: AbstractButton.TextOnly
        }

        Button {
            id: confirm_setup
            text: qsTr("Confirm")
            anchors.right: parent.right
            anchors.rightMargin: 0
            width: parent.width / 2 - 4

            background: Rectangle {
                color: "black"
                radius: 10
            }

            contentItem: Text {
                id: confirm_text
                font.bold: true
                color: "white"
                text: "Confirm"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                font.weight: Font.Bold
                font.pointSize: 18
                font.family: "Arial"
                font.capitalization: Font.AllUppercase
            }

            wheelEnabled: false
            spacing: 7
            display: AbstractButton.TextOnly
        }
    }
}
