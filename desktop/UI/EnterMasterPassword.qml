

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: rectangle
    width: Window.width
    height: Window.height
    opacity: 1

    Rectangle {
        id: master_password_rect
        height: 40
        radius: 10
        border.color: "#e0e0e0"
        border.width: 4
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        TextInput {
            id: master_password
            text: ""
            anchors.fill: parent
            font.letterSpacing: 0.9
            font.pixelSize: 24
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
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
        id: confirm_master_password
        text: qsTr("Confirm")
        onClicked: loader.source = "GetPassword.qml"

        background: Rectangle {
            color: "black"
            radius: 10
        }

        contentItem: Text {
            id: text1
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

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: master_password_rect.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 8
        wheelEnabled: false
        spacing: 7
        display: AbstractButton.TextOnly

        // Анимация уменьшения при нажатии
        PropertyAnimation {
            id: pressAnim
            target: confirm_master_password
            property: "scale"
            from: 1.0
            to: 0.95
            duration: 50
        }

        // Анимация возврата при отпускании
        PropertyAnimation {
            id: releaseAnim
            target: confirm_master_password
            property: "scale"
            from: 0.95
            to: 1.0
            duration: 50
        }

        Connections {
            id: connections
            target: confirm_master_password
            function onPressed() { pressAnim.running = true }
            function onReleased() { releaseAnim.running = true }
            function onCanceled() { releaseAnim.running = true }
        }
    }

    Text {
        id: text2
        width: implicitWidth
        height: implicitHeight
        text: qsTr("Enter master password")
        anchors.left: parent.left
        anchors.bottom: master_password_rect.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
        font.pixelSize: 18
        leftPadding: 2
        font.family: "Arial"
    }
}
