import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: rectangle
    width: Window.width
    height: Window.height
    opacity: 1

    GridView {
        id: hash_images
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
            width: hash_images.cellWidth
            height: hash_images.cellHeight

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

    Image {
        id: enable_backup
        width: 40
        height: 40
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.topMargin: 16
        source: "ic_enable_backup.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: share_backup
        width: 40
        height: 40
        anchors.left: enable_backup.right
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.topMargin: 16
        source: "ic_share_backup.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: get_password_help
        width: 40
        height: 40
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 16
        anchors.topMargin: 16
        source: "ic_help.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: text1
        width: implicitWidth
        height: implicitHeight
        text: qsTr("Choose image")
        anchors.left: parent.left
        anchors.bottom: hash_images.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
        font.pixelSize: 18
        font.family: "Arial"
    }

    ComboBox {
        id: selected_service
        height: 40
        anchors.left: parent.left
        anchors.right: setup_service.left
        anchors.bottom: text1.top
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

    Image {
        id: setup_service
        width: 40
        height: 40
        anchors.right: parent.right
        anchors.bottom: selected_service.bottom
        anchors.rightMargin: 16
        anchors.bottomMargin: 0
        source: "ic_setup.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: loader.source = "SetupService.qml"
        }
    }

    Text {
        id: text2
        width: implicitWidth
        height: implicitHeight
        text: qsTr("Select service")
        anchors.left: parent.left
        anchors.bottom: selected_service.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
        font.pixelSize: 18
        font.family: "Arial"
    }

    Text {
        id: text3
        width: implicitWidth
        height: implicitHeight
        text: qsTr("Your password")
        anchors.left: parent.left
        anchors.bottom: result_password_rect.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
        font.pixelSize: 18
        font.family: "Arial"
    }

    Rectangle {
            id: result_password_rect
            height: 40
            radius: 10
            border.color: "#e0e0e0"
            border.width: 4
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: copy_password.top
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            anchors.bottomMargin: 8
            TextInput {
                id: result_password
                text: ""
                anchors.fill: parent
                font.letterSpacing: 0.9
                font.pixelSize: 24
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                readOnly: true
                font.italic: false
                font.weight: Font.Normal
                rightPadding: 8
                leftPadding: 8
                font.family: "Arial"
                maximumLength: 30
                echoMode: TextInput.Password
            }
        }

    Button {
        id: copy_password
        text: qsTr("Copy password")

        background: Rectangle {
            color: "black"
            radius: 10
        }

        contentItem: Text {
            id: copy_text
            font.bold: true
            color: "white"
            text: "Copy password"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            font.weight: Font.Bold
            font.pointSize: 18
            font.family: "Arial"
            font.capitalization: Font.AllUppercase
        }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: master_password_rect.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8
        anchors.bottomMargin: 16
        wheelEnabled: false
        spacing: 7
        display: AbstractButton.TextOnly

        PropertyAnimation {
            id: pressAnim
            target: copy_password
            property: "scale"
            from: 1.0
            to: 0.95
            duration: 50
        }

        PropertyAnimation {
            id: releaseAnim
            target: copy_password
            property: "scale"
            from: 0.95
            to: 1.0
            duration: 50
        }

        Connections {
            id: connections
            target: copy_password
            function onPressed() { pressAnim.running = true }
            function onReleased() { releaseAnim.running = true }
            function onCanceled() { releaseAnim.running = true }
        }
    }
}
