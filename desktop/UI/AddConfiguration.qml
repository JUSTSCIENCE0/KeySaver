import QtQuick
import QtQuick.Controls

Rectangle {
    id: addConfiguration
    width: Window.width
    height: Window.height
    opacity: 1

    HelpButton {
        id: help
    }

    StyledLabel {
        id: configurationNameLabel
        text: qsTr("Configuration name")

        anchors.left: parent.left
        anchors.bottom: configurationName.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: configurationName

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: passwordLengthLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: passwordLengthLabel
        text: qsTr("Password length")

        anchors.left: parent.left
        anchors.bottom: passwordLength.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: passwordLength
        maxLength: 2

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: characterSet.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: characterSet
        text: qsTr("Character set")

        anchors.left: parent.left
        anchors.bottom: cancelConfirm.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }



    Row {
        id: cancelConfirm
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8

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
