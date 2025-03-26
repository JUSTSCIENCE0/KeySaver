import QtQuick
import QtQuick.Controls

Button {
    id: styledButton
    property color backgroundColor : "black"
    property color borderColor : "black"
    property color textColor : "white"
    
    //text: qsTr("Confirm")
    //onClicked: loader.source = "GetPassword.qml"

    background: Rectangle {
        color: styledButton.backgroundColor
        radius: 10
        border.color: styledButton.borderColor
        border.width: 4
    }

    contentItem: Text {
        id: styledButtonText
        font.bold: true
        color: styledButton.textColor
        text: styledButton.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        font.weight: Font.Bold
        font.pointSize: 18
        font.family: "Arial"
        font.capitalization: Font.AllUppercase
    }

    spacing: 7
    display: AbstractButton.TextOnly

    PropertyAnimation {
        id: pressAnim
        target: styledButton
        property: "scale"
        from: 1.0
        to: 0.95
        duration: 50
    }

    PropertyAnimation {
        id: releaseAnim
        target: styledButton
        property: "scale"
        from: 0.95
        to: 1.0
        duration: 50
    }

    Connections {
        id: connections
        target: styledButton
        function onPressed() { pressAnim.running = true }
        function onReleased() { releaseAnim.running = true }
        function onCanceled() { releaseAnim.running = true }
    }
}
