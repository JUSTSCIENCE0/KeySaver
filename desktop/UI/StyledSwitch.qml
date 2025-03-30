import QtQuick
import QtQuick.Controls


Row {
    id: styledSwitch
    spacing: 16
    height: 30

    property alias text: switchLabel.text
    property alias checked: switchButton.checked

    StyledLabel {
        id: switchLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    Switch {
        id: switchButton
        width: styledSwitch.height * 2
        height: styledSwitch.height
        checked: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        background: Rectangle {
            radius: height / 2
            color: switchButton.checked ? "black" : "white"
            border.width: 2
            border.color: switchButton.checked ? "black" : "#e0e0e0"
        }

        indicator: Rectangle {
            width: styledSwitch.height - 6
            height: width
            radius: width / 2
            color: switchButton.checked ? "white" : "#e0e0e0"
            anchors.verticalCenter: parent.verticalCenter
            x: switchButton.checked ? parent.width - width - 3 : 3

            Behavior on x { NumberAnimation { duration: 100 } }
        }
    }
}
