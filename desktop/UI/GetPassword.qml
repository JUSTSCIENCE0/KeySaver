import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: getPassword
    width: Window.width
    height: Window.height
    opacity: 1

    ImageButton {
        id: enableBackup
        source: "ic_enable_backup.png"

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.topMargin: 16
    }

    ImageButton {
        id: shareBackup
        source: "ic_share_backup.png"

        anchors.left: enableBackup.right
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.topMargin: 16
    }

    HelpButton {
        id: getPasswordHelp
    }

    StyledLabel {
        id: selectService
        text: qsTr("Select service")

        anchors.left: parent.left
        anchors.bottom: selected_service.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    ComboBox {
        id: selected_service
        height: 40
        anchors.left: parent.left
        anchors.right: setupService.left
        anchors.bottom: chooseImage.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
        font.pixelSize: 18
        font.family: "Arial"

        background: Rectangle {
            color: "white"
            radius: 10
            border.color: "#e0e0e0"
            border.width: 4
        }

        model: ["example", "test", qsTr("Add new...")]
    }

    ImageButton {
        id: setupService
        source: "ic_setup.png"
        onButtonClick: function() { loader.source = "SetupService.qml" }

        anchors.right: parent.right
        anchors.bottom: selected_service.bottom
        anchors.rightMargin: 16
        anchors.bottomMargin: 0
    }

    StyledLabel {
        id: chooseImage
        text: qsTr("Choose image")

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
            ListElement { imagePath: "cat.jpg" }
            ListElement { imagePath: "car.jpg" }
            ListElement { imagePath: "egg.jpg" }
            ListElement { imagePath: "city.jpg" }
            ListElement { imagePath: "bot.jpg" }
            ListElement { imagePath: "girl.jpg" }
            ListElement { imagePath: "phone.jpg" }
            ListElement { imagePath: "trees.jpg" }
            ListElement { imagePath: "plane.jpg" }
        }
        delegate: Item {
            width: imageHashes.cellWidth
            height: imageHashes.cellHeight

            Image {
                width: 125
                height: 125
                sourceSize.width: 200
                sourceSize.height: 200
                source: imagePath
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
            }
        }
        cellWidth: width / 3
        cellHeight: 133
    }

    StyledLabel {
        id: yourPassword
        text: qsTr("Your password")

        anchors.left: parent.left
        anchors.bottom: resultPassword.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: resultPassword
        readOnly: true

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: copyPassword.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledButton {
        id: copyPassword
        text: qsTr("Copy password")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: master_password_rect.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8
        anchors.bottomMargin: 16
    }
}
