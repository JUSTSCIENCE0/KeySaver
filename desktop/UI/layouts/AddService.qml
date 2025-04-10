import QtQuick
import QtQuick.Controls

Rectangle {
    id: addService
    width: Window.width
    height: Window.height
    opacity: 1

    HelpButton {
        id: help
    }

    StyledLabel {
        id: serviceUrlLabel
        text: qsTr("service_url")

        anchors.left: parent.left
        anchors.bottom: serviceURL.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: serviceURL
        placeholderText: "https://example.com"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: serviceDisplay.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: serviceDisplay
        text: qsTr("service_display_name")

        anchors.left: parent.left
        anchors.bottom: serviceName.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: serviceName
        placeholderText: "example"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: configuration.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: configuration
        text: qsTr("configuration")

        anchors.left: parent.left
        anchors.bottom: selectedConfiguration.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledComboBox {
        id: selectedConfiguration
        model: ["example", "test", qsTr("add_smth")]

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
    }

    Row {
        id: cancelConfirm
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: selectedConfiguration.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 24

        StyledButton {
            id: cancelSetup
            text: qsTr("cancel")
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
            text: qsTr("confirm")

            anchors.right: parent.right
            anchors.rightMargin: 0
            width: parent.width / 2 - 4
        }
    }
}
