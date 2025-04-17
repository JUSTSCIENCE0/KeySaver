import QtQuick
import QtQuick.Controls
import "qrc:/UI/components"

Rectangle {
    id: getPassword
    width: Window.width
    height: Window.height
    opacity: 1

    StackView.onActivated: {
        selectedService.currentIndex = 0;
        selectedService.onCurrentIndexChanged()
    }

    ImageButton {
        id: enableBackup
        source: "qrc:/UI/pictures/ic_enable_backup.png"

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.topMargin: 16
    }

    ImageButton {
        id: shareBackup
        source: "qrc:/UI/pictures/ic_share_backup.png"

        anchors.left: enableBackup.right
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.topMargin: 16
    }

    HelpButton {
        id: help
    }

    StyledLabel {
        id: selectService
        text: qsTr("select_service")

        anchors.left: parent.left
        anchors.bottom: selectedService.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledComboBox {
        id: selectedService
        model: Controller.servicesList

        anchors.left: parent.left
        anchors.right: setupService.left
        anchors.bottom: chooseImage.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8

        onCurrentIndexChanged: Controller.onSelectedServiceChanged(model[currentIndex])
    }

    ImageButton {
        id: setupService
        source: "qrc:/UI/pictures/ic_setup.png"
        //onButtonClick: function() { loader.source = "qrc:/UI/layouts/SetupService.qml" }

        anchors.right: parent.right
        anchors.bottom: selectedService.bottom
        anchors.rightMargin: 16
        anchors.bottomMargin: 0
    }

    StyledLabel {
        id: chooseImage
        text: qsTr("select_image")

        anchors.left: parent.left
        anchors.bottom: imageHashes.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    GridView {
        id: imageHashes
        height: 399
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        model: ListModel {
            ListElement {
                elementId: 0
                imagePath: "qrc:/UI/pictures/cat.jpg" }
            ListElement {
                elementId: 1
                imagePath: "qrc:/UI/pictures/car.jpg" }
            ListElement {
                elementId: 2
                imagePath: "qrc:/UI/pictures/egg.jpg" }
            ListElement {
                elementId: 3
                imagePath: "qrc:/UI/pictures/city.jpg" }
            ListElement {
                elementId: 4
                imagePath: "qrc:/UI/pictures/bot.jpg" }
            ListElement {
                elementId: 5
                imagePath: "qrc:/UI/pictures/girl.jpg" }
            ListElement {
                elementId: 6
                imagePath: "qrc:/UI/pictures/phone.jpg" }
            ListElement {
                elementId: 7
                imagePath: "qrc:/UI/pictures/trees.jpg" }
            ListElement {
                elementId: 8
                imagePath: "qrc:/UI/pictures/plane.jpg" }
        }
        delegate: Item {
            width: imageHashes.cellWidth
            height: imageHashes.cellHeight

            ImageButton {
                width: 125
                height: 125
                sourceSize.width: 200
                sourceSize.height: 200
                source: imagePath

                anchors.centerIn: parent

                onButtonClick: function() {
                    Controller.onGeneratePassword(
                        selectedService.model[selectedService.currentIndex], elementId)
                }
            }
        }
        cellWidth: width / 3
        cellHeight: 133
    }

    StyledLabel {
        id: yourPassword
        text: qsTr("your_password")

        anchors.left: parent.left
        anchors.bottom: resultPassword.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: resultPassword
        readOnly: true
        text: Controller.servicePassword

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: copyPassword.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledButton {
        id: copyPassword
        text: qsTr("copy_password")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8
        anchors.bottomMargin: 16
    }
}
