import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/UI/components"

Rectangle {
    id: addService
    width: Window.width
    height: Window.height
    opacity: 1

    StackView.onActivated: {
        selectedConfiguration.currentIndex = 0;
        Controller.onSelectedConfigChanged(model[0])
    }

    HelpButton {
        id: help
        text: qsTr("add_service_help")
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
        model: Controller.configsList

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        onCurrentIndexChanged: Controller.onSelectedConfigChanged(model[currentIndex])
    }

    RowLayout {
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

            Layout.fillWidth: true
            Layout.preferredHeight: parent.height

            onClicked: stackView.pop()
        }

        StyledButton {
            id: confirmSetup
            text: qsTr("confirm")

            Layout.fillWidth: true
            Layout.preferredHeight: parent.height

            onClicked: Controller.onAddService(
                serviceURL.text, serviceName.text,
                selectedConfiguration.model[selectedConfiguration.currentIndex]
            )
        }
    }
}
