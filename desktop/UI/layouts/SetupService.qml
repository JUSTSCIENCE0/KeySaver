import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/UI/components"

Rectangle {
    id: setupService
    width: Window.width
    height: Window.height
    opacity: 1

    StackView.onActivated: {
        serviceName.text = Controller.getSetupService()

        selectedConfiguration.currentIndex = Controller.getSetupServiceConfig();
        Controller.onSelectedConfigChanged(model[selectedConfiguration.currentIndex])
    }

    HelpButton {
        id: help
        text: qsTr("setup_service_help")
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
        anchors.topMargin: 8

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

            onClicked: Controller.onEditService(
                serviceName.text,
                selectedConfiguration.model[selectedConfiguration.currentIndex]
            )
        }
    }

    StyledButton {
        id: deleteService
        text: qsTr("delete_service")
        backgroundColor: "red"
        borderColor: "red"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: cancelConfirm.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8

        onClicked: Controller.onDeleteService()
    }
}
